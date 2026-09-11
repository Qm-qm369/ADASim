#include "MainWindow.h"
#include "View2D.h"
#include "backend/DataLoader.h"
#include "backend/DataManager.h"
#include "SensorView.h"
#include "communication/Socket.h"

#include <QThread>
#include <QDebug>
#include <QCloseEvent>
#include <QMetaObject>
#include <QToolBar>
#include <QStatusBar>
#include <cmath>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(const QString &configPath,
                       const QString &dataPath,
                       QWidget *parent)
    : QMainWindow(parent), configPath_(configPath), dataPath_(dataPath)
{
    setWindowTitle("ADASim - 自动驾驶算法仿真平台 v0.2");
    resize(1600, 900);

    setStyleSheet(
        "QMainWindow { background-color: #050811; }");

    setupUI();
    setupToolBar();
    setupStatusBar();

    // 创建并启动后台线程
    startBackend();
    // 建立数据连接
    setupConnections();

    // V0.10网络与规划
    setupNetwork();
}

MainWindow::~MainWindow()
{
    if (socketServer_)
    {
        socketServer_->stop();
    }

    stopBackend();
}

void MainWindow::startBackend()
{
    qDebug() << "MainWindow thread ="
             << QThread::currentThread();
    // 创建后台线程
    backendThread_ = new QThread(this);

    /*
     * 注意：
     * DataLoader 这里不能传 this 作为 parent。
     * Qt 要保证：父对象和子对象必须属于同一个线程。
     * 因为有 parent 的 QObject
     * 不能被 moveToThread() 移到其他线程。
     */
    dataLoader_ = new DataLoader(dataPath_);
    // 将 DataLoader 的线程归属移动到后台线程
    dataLoader_->moveToThread(backendThread_);

    dataManager_ = new DataManager();
    dataManager_->moveToThread(backendThread_);

    // 后台线程结束时，自动删除 DataLoader
    connect(backendThread_, &QThread::finished,
            dataLoader_, &QObject::deleteLater);

    connect(backendThread_, &QThread::finished,
            dataManager_, &QObject::deleteLater);

    // 启动后台线程事件循环
    backendThread_->start();
}

void MainWindow::stopBackend()
{
    if (backendThread_ &&
        backendThread_->isRunning())
    {
        /*
         * 先让 DataLoader 在自己的线程中停掉 QTimer。
         *
         * BlockingQueuedConnection：
         * 主线程会等待 stop() 真正执行完再继续。
         */
        QMetaObject::invokeMethod(
            dataLoader_,
            "stop",
            Qt::BlockingQueuedConnection);

        // 通知后台线程退出事件循环
        backendThread_->quit();

        // 等待后台线程真正结束
        backendThread_->wait(2000);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 关闭窗口前先停止后台线程
    stopBackend();
    // 接受这个事件，事件不再继续向上传递。
    event->accept();
}

// 建立真正的数据连接
void MainWindow::setupConnections()
{
    /*
     * DataLoader产生车辆数据  DataManager接收并保存
     */

    connect(dataLoader_, &DataLoader::vehiclePositionReady, this, &MainWindow::onVehicleDataUpdated);
    connect(this, &MainWindow::trueVehiclePositionReady, dataManager_, &DataManager::onVehiclePositionReceived);

    // DataLoader -> 状态栏
    connect(dataLoader_, &DataLoader::statusUpdate,
            this, &MainWindow::onStatusUpdate);

    connect(dataLoader_, &DataLoader::totalFramesLoaded,
            this, [this](int total)
            {
                int maxFrame = (total > 0) ? total - 1 : 0;
                // FIFO最多100帧，所以最大索引99
                if (maxFrame > 99)
                    maxFrame = 99;
                timeSlider_->setMaximum(maxFrame); });

    connect(dataLoader_, &DataLoader::currentFrameUpdated,
            this, [this](int current)
            {
            /*
             * 程序主动移动滑块时，
             * 不允许触发 valueChanged，
             * 否则会反过来调用 seekToFrame。 
             * blockSignals() 是 Qt 的 QObject 自带函数 临时阻止一个 Qt 对象发出信号。
             */
            timeSlider_->blockSignals(true);

            timeSlider_->setValue(current);

            timeSlider_->blockSignals(false);

            timeLabel_->setText(QString("%1 / %2").arg(current). arg(timeSlider_->maximum())); });

    connect(timeSlider_, &QSlider::valueChanged,
            this,
            [this](int value)
            {
                // 更新显示
                timeLabel_->setText(QString("%1 / %2").arg(value).arg(timeSlider_->maximum()));

                // 通知后台回放指定帧
                QMetaObject::invokeMethod(dataLoader_, "seekToFrame",
                                          Qt::QueuedConnection, Q_ARG(int, value));
            }); // QMetaObject::invokeMethod() 跨线程传递任务。

    // 原始点云进入DataManager
    connect(
        dataLoader_,
        &DataLoader::pointCloudReady,
        dataManager_,
        &DataManager::onPointCloudReceived);

    // 最终点云给SensorView
    connect(
        dataManager_,
        &DataManager::mergedPointCloudReady,
        sensorView_,
        &SensorView::updatePointCloud);

    // 检测出的障碍物给View2D
    connect(
        dataManager_,
        &DataManager::obstaclesDetected,
        view2D_,
        &View2D::updateObstacles);

    // View2D右键添加障碍物
    connect(
        view2D_,
        &View2D::userObstacleAdded,
        dataManager_,
        &DataManager::onUserObstacleAdded);

    connect(dataManager_, &DataManager::pathPredicted,
            this, [this](const QVector<QPointF> &path)
            {
            if (algoValue_)
            {
                algoValue_->setText(QString("预测 %1 点").arg(path.size()));
            } });

    connect(dataManager_, &DataManager::pathPredicted,
            view2D_, &View2D::updatePredictedPath);
}

void MainWindow::onStartSimulation()
{
    // 跨线程调用对象的 `start()` 槽函数，使用队列方式投递事件，不阻塞当前线程。
    QMetaObject::invokeMethod(
        dataLoader_,
        "start",
        Qt::QueuedConnection);
}

void MainWindow::onPauseSimulation()
{
    QMetaObject::invokeMethod(
        dataLoader_,
        "pause",
        Qt::QueuedConnection);
}

void MainWindow::onStopSimulation()
{
    QMetaObject::invokeMethod(dataLoader_, "stop", Qt::QueuedConnection);

    totalDistance_ = 0.0;
    lastX_ = 0.0;
    lastY_ = 0.0;

    targetLateralOffset_ = 0.0;
    currentLateralOffset_ = 0.0;

    planStartX_ = 0.0;
    planStartOffset_ = 0.0;
    lateralPlanActive_ = false;

    plannedTrajectory_.clear();

    if (speedValue_)
    {
        speedValue_->setText("0.0 km/h");
    }

    if (distanceValue_)
    {
        distanceValue_->setText("0.0 m");
    }

    if (view2D_)
    {
        view2D_->updateVehiclePosition(0.0, 0.0, 0.0);
        view2D_->updatePlannedTrajectory(QVector<QPointF>());
    }
}

void MainWindow::onStatusUpdate(const QString &status)
{
    statusBar()->showMessage(status);
}

// 每收到一帧车辆基础位置，就让车辆的 Y 方向逐渐靠近 Python 选中的 Lattice 横向目标，然后更新车辆显示，同时计算速度和累计里程。
void MainWindow::onVehicleDataUpdated(double x, double y, double yaw)
{
    if (lateralPlanActive_)
    {
        currentLateralOffset_ = calculatePlannedOffset(x);

        if (x >= planStartX_ + planningDistance_)
        {
            currentLateralOffset_ = targetLateralOffset_;
            lateralPlanActive_ = false;
        }
    }
    // DataLoader 的 y 是基础位置，叠加规划产生的横向偏移
    double trueY = y + currentLateralOffset_;

    // 更新真实车辆显示位置
    view2D_->updateVehiclePosition(x, trueY, yaw);

    // 把真实车辆位置重新送回 DataManager
    emit trueVehiclePositionReady(x, trueY, yaw);

    // 从第二帧开始计算真实行驶距离
    if (lastX_ != 0.0 || lastY_ != 0.0)
    {
        double dx = x - lastX_;
        double dy = trueY - lastY_;
        double distance = std::sqrt(dx * dx + dy * dy);

        totalDistance_ += distance;

        // 每帧0.1秒
        double speed = distance / 0.1;
        double speedKmH = speed * 3.6;

        if (speedValue_)
        {
            speedValue_->setText(QString::number(speedKmH, 'f', 1) + " km/h");
        }

        if (distanceValue_)
        {
            distanceValue_->setText(QString::number(totalDistance_, 'f', 1) + " m");
        }
    }

    lastX_ = x;
    lastY_ = trueY;
}

/**
 * 创建 V0.2 的静态主界面
 */
void MainWindow::setupUI()
{
    // QMainWindow 的中央区域
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 整个页面采用上下排列
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // =========================
    // 1. 顶部信息区域
    // =========================

    QFrame *infoPanel = new QFrame(this); // QFrame非常适合当卡片 面板 边框区域 分组容器
    infoPanel->setFixedHeight(100);

    infoPanel->setStyleSheet(
        "QFrame {"
        "background-color: #111827;"
        "border: 1px solid #2d3748;"
        "border-radius: 8px;"
        "}");

    // 四张卡片左右排列
    QHBoxLayout *infoLayout = new QHBoxLayout(infoPanel);
    infoLayout->setSpacing(15);

    createInfoCard(infoLayout,
                   "当前速度",
                   "0 km/h",
                   "speedValue");

    createInfoCard(infoLayout,
                   "行驶里程",
                   "0.0 m",
                   "distanceValue");

    createInfoCard(infoLayout,
                   "渲染帧率",
                   "60 FPS",
                   "fpsValue");

    createInfoCard(infoLayout,
                   "算法状态",
                   "就绪",
                   "algoValue");

    mainLayout->addWidget(infoPanel);

    // =========================
    // 2. 中间 2D 仿真区域
    // =========================

    QHBoxLayout *viewsLayout =
        new QHBoxLayout();

    view2D_ = new View2D(this);

    sensorView_ = new SensorView(this);

    // 左边主视图更大
    viewsLayout->addWidget(view2D_, 2);

    // 右边雷达视图
    viewsLayout->addWidget(sensorView_, 1);

    mainLayout->addLayout(viewsLayout, 1);

    // =========================
    // 时间轴回放区域
    // =========================

    QFrame *playbackPanel = new QFrame(this);

    playbackPanel->setFixedHeight(55);

    playbackPanel->setStyleSheet(
        "QFrame {"
        "background-color: #111827;"
        "border: 1px solid #2d3748;"
        "border-radius: 6px;"
        "}");

    QHBoxLayout *playbackLayout =
        new QHBoxLayout(playbackPanel);

    QLabel *playbackTitle =
        new QLabel("时光回放：", playbackPanel);

    playbackTitle->setStyleSheet(
        "color: #8b949e;"
        "font-weight: bold;");

    // 横向滑动条
    timeSlider_ = new QSlider(Qt::Horizontal, playbackPanel);

    timeSlider_->setMinimum(0);
    timeSlider_->setMaximum(0);

    // 当前帧 / 最大帧
    timeLabel_ =
        new QLabel("0 / 0", playbackPanel);

    timeLabel_->setMinimumWidth(80);

    timeLabel_->setAlignment(
        Qt::AlignRight |
        Qt::AlignVCenter);

    timeLabel_->setStyleSheet(
        "color: #8b949e;");

    playbackLayout->addWidget(playbackTitle);

    playbackLayout->addWidget(timeSlider_, 1);

    playbackLayout->addWidget(timeLabel_);

    mainLayout->addWidget(playbackPanel);

    // =========================
    // 3. 底部状态区域
    // =========================

    QFrame *statusPanel = new QFrame(this);
    statusPanel->setFixedHeight(50);

    statusPanel->setStyleSheet(
        "QFrame {"
        "background-color: #111827;"
        "border: 1px solid #2d3748;"
        "border-radius: 6px;"
        "}");

    QHBoxLayout *statusLayout =
        new QHBoxLayout(statusPanel);

    QLabel *statusLabel =
        new QLabel("系统状态：就绪", statusPanel);

    statusLabel->setStyleSheet(
        "color: #00ff88;"
        "font-size: 14px;");

    statusLayout->addWidget(statusLabel);

    // 把后面的空白撑开，让文字留在左侧
    statusLayout->addStretch();

    mainLayout->addWidget(statusPanel);
}

/**
 * 创建顶部信息卡片
 */
void MainWindow::createInfoCard(QLayout *layout,
                                const QString &title,
                                const QString &value,
                                const QString &objectName)
{
    QFrame *card = new QFrame(this);

    card->setStyleSheet(
        "QFrame {"
        "background-color: #161b22;"
        "border: 1px solid #30363d;"
        "border-radius: 6px;"
        "}");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(
        "color: #8b949e;"
        "font-size: 13px;"
        "border: none;");

    QLabel *valueLabel = new QLabel(value, card);
    valueLabel->setObjectName(objectName);
    // objectName有两个用途
    // 1.这个 QLabel 设置 Qt 对象名。
    // 2.利用这个名字判断：这张卡片到底是速度、里程、FPS 还是算法状态。

    valueLabel->setStyleSheet(
        "color: #00ff88;"
        "font-size: 22px;"
        "font-weight: bold;"
        "border: none;");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(valueLabel);

    layout->addWidget(card);

    // 保存数值 QLabel，后续版本需要动态修改
    if (objectName == "speedValue")
        speedValue_ = valueLabel;
    else if (objectName == "distanceValue")
        distanceValue_ = valueLabel;
    else if (objectName == "fpsValue")
        fpsValue_ = valueLabel;
    else if (objectName == "algoValue")
        algoValue_ = valueLabel;
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("主工具栏");

    toolBar->setMovable(false);

    /*
    这里 addAction() 帮我们直接把：
    点击工具栏按钮 然后调用槽函数 连接起来。
    */
    toolBar->addAction(
        "▶ 启动",
        this,
        &MainWindow::onStartSimulation);

    toolBar->addAction(
        "⏸ 暂停",
        this,
        &MainWindow::onPauseSimulation);

    toolBar->addAction(
        "⏹ 停止",
        this,
        &MainWindow::onStopSimulation);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("ADASim 引擎就绪");
}

void MainWindow::setupNetwork()
{
    // =====================================
    // 1. 创建TCP服务器
    // =====================================

    socketServer_ = new SocketServer(this);

    // =====================================
    // 2. DataManager -> Python
    // =====================================

    connect(dataManager_, &DataManager::plannerDataReady,
            socketServer_, &SocketServer::sendToClient);

    // =====================================
    // 3. Python -> DataManager
    // =====================================

    connect(socketServer_, &SocketServer::dataReceived,
            dataManager_, &DataManager::onPlannerDataReceived);

    // =====================================
    // 4. 规划结果 -> View2D
    // =====================================
    connect(dataManager_, &DataManager::lateralControlReceived, this, &MainWindow::onLateralControlReceived);

    // =====================================
    // 5. 显示规划结果
    // =====================================

    connect(dataManager_, &DataManager::lateralControlReceived,
            this, [this](double offset)
            {
            if (algoValue_)
            {
                algoValue_->setText( QString( "规划偏移 %1 m").arg(offset, 0, 'f', 2));
            } });

    // =====================================
    // 6. 网络状态
    // =====================================

    connect(socketServer_, &SocketServer::clientConnected,
            this, [this]()
            { statusBar()->showMessage(
                  "Python Planner 已连接"); });

    connect(socketServer_, &SocketServer::clientDisconnected,
            this, [this]()
            { statusBar()->showMessage(
                  "Python Planner 已断开"); });

    // =====================================
    // 7. 开始监听8080
    // =====================================

    bool success = socketServer_->startTcpServer(8080);

    if (success)
    {
        statusBar()->showMessage("Planner TCP Server：8080");
    }
    else
    {
        statusBar()->showMessage("TCP Server启动失败");
    }
}

void MainWindow::onLateralControlReceived(double offset)
{
    // Python还是返回同一个目标，就继续执行当前轨迹，不重新规划
    if (std::abs(offset - targetLateralOffset_) < 0.01)
    {
        return;
    }

    qDebug() << "新的Lattice目标:"
             << offset
             << "当前offset:"
             << currentLateralOffset_
             << "当前X:"
             << lastX_;

    // 目标真的发生变化，开始生成新的可执行横向轨迹
    startLateralPlan(offset);
}

double MainWindow::calculatePlannedOffset(double x) const
{
    if (!lateralPlanActive_)
    {
        return targetLateralOffset_;
    }

    double u = (x - planStartX_) / planningDistance_;

    if (u <= 0.0)
    {
        return planStartOffset_;
    }

    if (u >= 1.0)
    {
        return targetLateralOffset_;
    }

    double smooth = 3.0 * u * u - 2.0 * u * u * u;

    return planStartOffset_ + (targetLateralOffset_ - planStartOffset_) * smooth;
}

void MainWindow::rebuildPlannedTrajectory()
{
    plannedTrajectory_.clear();

    const int pointCount = 40;

    for (int i = 0; i <= pointCount; ++i)
    {
        double u = static_cast<double>(i) / pointCount;
        double smooth = 3.0 * u * u - 2.0 * u * u * u;

        double x = planStartX_ + planningDistance_ * u;
        double offset = planStartOffset_ + (targetLateralOffset_ - planStartOffset_) * smooth;

        plannedTrajectory_.append(QPointF(x, offset));
    }

    view2D_->updatePlannedTrajectory(plannedTrajectory_);
}

void MainWindow::startLateralPlan(double targetOffset)
{
    planStartX_ = lastX_;
    planStartOffset_ = currentLateralOffset_;
    targetLateralOffset_ = targetOffset;
    lateralPlanActive_ = true;

    qDebug() << "开始横向规划:"
             << "startX =" << planStartX_
             << "startOffset =" << planStartOffset_
             << "targetOffset =" << targetLateralOffset_;

    rebuildPlannedTrajectory();
}