#include "MainWindow.h"

#include <QThread>
#include <QDebug>
#include <QCloseEvent>
#include <QMetaObject>
#include <QToolBar>
#include <QStatusBar>
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(const QString &configPath,
                       const QString &dataPath,
                       QWidget *parent)
    : QMainWindow(parent), configPath_(configPath), dataPath_(dataPath)
{
    LinuxLogger::init();

    LinuxLogger::info("ADASim application started");

    setWindowTitle("ADASim - 自动驾驶算法仿真平台 v0.2");
    resize(1600, 900);

    setStyleSheet("QMainWindow { background-color: #050811; }");

    // V1.9
    loadConfig();
    applyConfig();

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

    LinuxLogger::info("ADASim application stopped");

    LinuxLogger::shutdown();
}

void MainWindow::loadConfig()
{
    QString errorMessage;

    bool success = ConfigManager::load(configPath_, appConfig_, errorMessage);

    if (success)
    {
        LinuxLogger::info(QString("Config loaded: %1").arg(configPath_));
    }
    else
    {
        // 配置加载失败仍然继续运行
        // 使用AppConfig默认值
        LinuxLogger::warning(errorMessage);
    }
}

void MainWindow::applyConfig()
{
    targetVehicleSpeed_ = appConfig_.targetSpeed;

    lookAheadDistance_ = appConfig_.lookAheadDistance;

    planningDistance_ = appConfig_.planningDistance;

    plannerPort_ = appConfig_.plannerPort;

    trajectoryController_.setGains(appConfig_.headingGain, appConfig_.lateralGain);

    if (appConfig_.controllerMode == "PurePursuit")
    {
        controllerMode_ = ControllerMode::PurePursuit;
    }
    else
    {
        controllerMode_ = ControllerMode::DualError;
    }
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
    // V1.9：退出前保存当前参数
    saveConfig();

    // 关闭窗口前先停止后台线程
    stopBackend();
    // 接受这个事件，事件不再继续向上传递。
    event->accept();
}

// 建立真正的数据连接
void MainWindow::setupConnections()
{

    connect(dataLoader_, &DataLoader::simulationTick, this, &MainWindow::onSimulationTick);

    connect(this, &MainWindow::simulationFrameReady, dataManager_, &DataManager::onSimulationFrame);

    // DataLoader -> 状态栏
    connect(dataLoader_, &DataLoader::statusUpdate, this, &MainWindow::onStatusUpdate);

    connect(timeSlider_, &QSlider::valueChanged, this, &MainWindow::onReplayFrameSelected);

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

    // 下拉框 `controllerCombo_` 选中项发生变化时，触发 lambda 函数，修改成员变量 `controllerMode_`，切换控制器模式
    connect(controllerCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index)
            {
            if (index == 0)
            {
                controllerMode_ =ControllerMode::DualError;
            }
            else
            {
                controllerMode_ =ControllerMode::PurePursuit;
            } });

    connect(speedSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value)
            { targetVehicleSpeed_ = value; }); // 如果安全，我希望车达到5m/s。

    connect(lookAheadSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value)
            { lookAheadDistance_ = value; });

    connect(headingGainSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double)
            { trajectoryController_.setGains(
                  headingGainSpin_->value(),
                  lateralGainSpin_->value()); });

    connect(lateralGainSpin_, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double)
            { trajectoryController_.setGains(
                  headingGainSpin_->value(),
                  lateralGainSpin_->value()); });

    connect(dataManager_, &DataManager::frontObstacleDistanceUpdated,
            this, &MainWindow::onFrontObstacleDistanceUpdated);
}

void MainWindow::onStartSimulation()
{
    if (!vehicleModelInitialized_)
    {
        vehicleModel_.setState(0.0, 0.0, 0.0);
        vehicleModelInitialized_ = true;
    }

    if (replayMode_)
    {
        replayMode_ = false;

        VehicleState state = vehicleModel_.state();

        view2D_->showReplayFrame(
            state.x,
            state.y,
            state.yaw);
    }

    LinuxLogger::info(QString("Simulation started, target speed=%1 m/s").arg(targetVehicleSpeed_, 0, 'f', 1));

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

    currentVehicleSpeed_ = 0.0;
    frontObstacleDistance_ = -1.0;
    emergencyBrakeActive_ = false;

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

    vehicleModel_.reset();
    vehicleModelInitialized_ = false;

    simulationRecorder_.clear();

    replayMode_ = false;

    timeSlider_->blockSignals(true);
    timeSlider_->setMinimum(0);
    timeSlider_->setMaximum(0);
    timeSlider_->setValue(0);
    timeSlider_->blockSignals(false);

    timeLabel_->setText("0 / 0");

    if (view2D_)
    {
        view2D_->updateVehiclePosition(0.0, 0.0, 0.0);
        view2D_->updatePlannedTrajectory(QVector<QPointF>());
        view2D_->clearTrackingDebug();
    }

    if (controlMonitor_)
    {
        controlMonitor_->clear();
    }

    LinuxLogger::info("Simulation stopped");
}

void MainWindow::onStatusUpdate(const QString &status)
{
    statusBar()->showMessage(status);
}

/*
每一帧仿真的总入口

轨迹跟踪
→ 控制器
→ 车辆模型
→ 速度/里程
→ 感知数据
→ 历史录像
→ 时间轴
→ UI显示
*/
void MainWindow::onSimulationTick(const QVector<QPointF> &points)
{
    // 两种情况下不允许继续推进车辆  车辆模型还没初始化 或者现在正在历史回放
    if (!vehicleModelInitialized_ || replayMode_)
    {
        return;
    }

    VehicleState currentState = vehicleModel_.state();

    // =====================================================
    // 1. 判断SmoothStep过渡段是否结束
    // =====================================================

    if (lateralPlanActive_ &&
        currentState.x >= planStartX_ + planningDistance_)
    {
        lateralPlanActive_ = false;
    }

    // =====================================================
    // 2. 当前参考横向位置
    // =====================================================

    double targetY = calculatePlannedOffset(currentState.x);

    // =====================================================
    // 3. 前视参考航向
    // =====================================================

    double previewX = currentState.x + lookAheadDistance_;

    double targetYaw = 0.0;

    if (lateralPlanActive_)
    {
        targetYaw = calculatePlannedYaw(previewX);
    }

    // =====================================================
    // 4. V1.8：纵向速度控制
    // =====================================================

    LongitudinalControlOutput longitudinalControl =
        longitudinalController_.compute(
            targetVehicleSpeed_,
            currentVehicleSpeed_,
            frontObstacleDistance_);

    // ================================
    // 这里添加 Linux日志
    // ================================

    if (longitudinalControl.emergencyBrake &&
        !emergencyBrakeActive_)
    {
        LinuxLogger::warning(
            QString(
                "Emergency brake triggered, distance=%1 m, TTC=%2 s")
                .arg(frontObstacleDistance_, 0, 'f', 1)
                .arg(longitudinalControl.ttc, 0, 'f', 2));
    }

    if (!longitudinalControl.emergencyBrake &&
        emergencyBrakeActive_)
    {
        LinuxLogger::info(
            "Emergency brake released");
    }

    emergencyBrakeActive_ =
        longitudinalControl.emergencyBrake;

    // v = v0 + a * dt
    currentVehicleSpeed_ +=
        longitudinalControl.acceleration *
        simulationDt_;

    if (currentVehicleSpeed_ < 0.0)
    {
        currentVehicleSpeed_ = 0.0;
    }

    // =====================================================
    // 4. 双误差控制器计算前轮转角 横向控制器
    // =====================================================

    ControlOutput control;

    if (controllerMode_ == ControllerMode::DualError)
    {
        control = trajectoryController_.compute(
            currentState,
            targetY,
            targetYaw,
            currentVehicleSpeed_);
    }
    else
    {
        double pursuitX = currentState.x + lookAheadDistance_;

        double pursuitY = calculatePlannedOffset(pursuitX);

        QPointF pursuitTarget(pursuitX, pursuitY);

        control.steeringAngle = purePursuitController_.computeSteering(
            currentState,
            pursuitTarget);

        control.lateralError = targetY - currentState.y;

        double pursuitYaw = std::atan2(
            pursuitY - currentState.y,
            pursuitX - currentState.x);

        control.headingError = normalizeAngle(pursuitYaw - currentState.yaw);
    }

    // =====================================================
    // 5. VehicleModel执行这一帧运动
    // =====================================================

    VehicleState newState = vehicleModel_.update(
        currentVehicleSpeed_,
        control.steeringAngle,
        simulationDt_);

    currentLateralOffset_ = newState.y;

    // =====================================================
    // 6. 更新后的车辆重新计算参考误差
    // =====================================================

    double newTargetY = calculatePlannedOffset(newState.x);
    double currentLateralError = newTargetY - newState.y;

    // V1.7：把这一帧控制数据送给控制曲线监控器
    if (controlMonitor_)
    {
        controlMonitor_->appendSample(
            currentLateralError,
            control.steeringAngle);
    }

    // 判断车辆是不是真的稳定了
    bool targetSettled =
        !lateralPlanActive_ &&
        std::abs(currentLateralError) < 0.05 &&
        std::abs(newState.yaw) < 0.02;

    // =====================================================
    // 7. 根据这一帧实际运动距离统计速度、里程
    // =====================================================

    double dx = newState.x - currentState.x;
    double dy = newState.y - currentState.y;

    double distance = std::sqrt(dx * dx + dy * dy);

    totalDistance_ += distance;

    double speedKmH = currentVehicleSpeed_ * 3.6;

    // =====================================================
    // 8. 显示真实车辆状态
    // =====================================================

    view2D_->updateVehiclePosition(
        newState.x,
        newState.y,
        newState.yaw);

    view2D_->updateTrackingDebug(
        QPointF(newState.x, newTargetY),
        currentLateralError,
        control.steeringAngle);

    // =====================================================
    // 9. 当前完整仿真帧交给DataManager
    // =====================================================

    emit simulationFrameReady(
        newState.x,
        newState.y,
        newState.yaw,
        points);

    /*
    =====================================================
    SimulationFrame 10. 保存真实历史帧

    车辆状态
    ├─ x
    ├─ y
    └─ yaw

    规划状态
    ├─ targetY
    └─ targetYaw

    控制状态
    ├─ lateralError
    ├─ headingError
    └─ steeringAngle

    统计状态
    ├─ speedKmH
    └─ totalDistance
    =====================================================
    */
    SimulationFrame frame;

    frame.vehicle = newState;
    frame.targetY = newTargetY;
    frame.targetYaw = targetYaw;

    frame.lateralError = currentLateralError;
    frame.headingError = control.headingError;
    frame.steeringAngle = control.steeringAngle;

    frame.speedKmH = speedKmH;
    frame.totalDistance = totalDistance_;

    frame.targetSpeedKmH = longitudinalControl.safeTargetSpeed * 3.6;

    frame.acceleration = longitudinalControl.acceleration;

    frame.frontObstacleDistance = frontObstacleDistance_;

    frame.ttc = longitudinalControl.ttc;

    frame.emergencyBrake = longitudinalControl.emergencyBrake;

    // V1.7：记录这一帧使用的是哪个控制器
    frame.controllerMode =
        controllerMode_ == ControllerMode::DualError
            ? 0
            : 1;

    simulationRecorder_.append(frame);

    // =====================================================
    // 11. 时间轴自动跟随最新真实帧
    // =====================================================

    int currentIndex = simulationRecorder_.size() - 1;

    timeSlider_->blockSignals(true); // 先禁止滑块发信号。避免进入回放

    timeSlider_->setMaximum(currentIndex);
    timeSlider_->setValue(currentIndex);

    timeSlider_->blockSignals(false);

    timeLabel_->setText(
        QString("%1 / %2")
            .arg(currentIndex)
            .arg(currentIndex));

    // =====================================================
    // 12. 更新界面数据
    // =====================================================

    if (speedValue_)
    {
        speedValue_->setText(
            QString::number(speedKmH, 'f', 1) + " km/h");
    }

    if (distanceValue_)
    {
        distanceValue_->setText(
            QString::number(totalDistance_, 'f', 1) + " m");
    }

    if (algoValue_)
    {
        double steeringDegree =
            control.steeringAngle * 180.0 / M_PI;

        if (targetSettled)
        {
            algoValue_->setText(
                QString("已稳定 %1 m")
                    .arg(newState.y, 0, 'f', 2));
        }
        else
        {
            QString controllerName =
                controllerMode_ == ControllerMode::DualError ? "双误差" : "PurePursuit";

            algoValue_->setText(
                QString("%1  eY=%2m  steer=%3°")
                    .arg(controllerName)
                    .arg(currentLateralError, 0, 'f', 2)
                    .arg(steeringDegree, 0, 'f', 1));
        }
    }

    lastX_ = newState.x;
    lastY_ = newState.y;
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
    // 1.5. 控制面板
    // =========================

    QFrame *controlPanel = new QFrame(this);
    controlPanel->setObjectName("controlPanel");
    controlPanel->setFixedHeight(110);

    controlPanel->setStyleSheet(
        "QFrame#controlPanel {"
        "    background-color: #0f172a;"
        "    border: 1px solid #243041;"
        "    border-radius: 10px;"
        "}"
        "QWidget#paramCard {"
        "    background-color: #111827;"
        "    border: 1px solid #2d3748;"
        "    border-radius: 8px;"
        "}"
        "QLabel#paramTitle {"
        "    color: #94a3b8;"
        "    font-size: 13px;"
        "    font-weight: 600;"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QComboBox, QDoubleSpinBox {"
        "    background-color: #0b1220;"
        "    color: #f8fafc;"
        "    border: 1px solid #3b475a;"
        "    border-radius: 6px;"
        "    padding: 6px 10px;"
        "    min-height: 32px;"
        "    font-size: 14px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 24px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    width: 0px;"
        "    height: 0px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: #0b1220;"
        "    color: #f8fafc;"
        "    border: 1px solid #3b475a;"
        "    selection-background-color: #1d4ed8;"
        "    selection-color: #ffffff;"
        "    outline: 0;"
        "    padding: 4px;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "    min-height: 28px;"
        "    padding: 6px 10px;"
        "}"
        "QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {"
        "    width: 18px;"
        "}");

    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(12, 10, 12, 10);
    controlLayout->setSpacing(12);

    // 创建单个参数卡片的小工具函数
    auto createParamCard = [&](const QString &title, QWidget *editor) -> QWidget *
    {
        QFrame *card = new QFrame(controlPanel);
        card->setObjectName("paramCard");
        card->setMinimumWidth(180);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(12, 8, 12, 8);
        cardLayout->setSpacing(6);

        QLabel *titleLabel = new QLabel(title, card);
        titleLabel->setObjectName("paramTitle");

        editor->setParent(card);

        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(editor);

        return card;
    };

    // ===== 创建控件 =====
    controllerCombo_ = new QComboBox();
    controllerCombo_->addItem("双误差控制");
    controllerCombo_->addItem("Pure Pursuit");

    if (controllerMode_ == ControllerMode::PurePursuit)
    {
        controllerCombo_->setCurrentIndex(1);
    }
    else
    {
        controllerCombo_->setCurrentIndex(0);
    }

    speedSpin_ = new QDoubleSpinBox();
    speedSpin_->setRange(1.0, 15.0);
    speedSpin_->setSingleStep(0.5);
    speedSpin_->setValue(targetVehicleSpeed_);
    speedSpin_->setSuffix(" m/s");

    lookAheadSpin_ = new QDoubleSpinBox();
    lookAheadSpin_->setRange(1.0, 10.0);
    lookAheadSpin_->setSingleStep(0.5);
    lookAheadSpin_->setValue(lookAheadDistance_);
    lookAheadSpin_->setSuffix(" m");

    headingGainSpin_ = new QDoubleSpinBox();
    headingGainSpin_->setRange(0.0, 5.0);
    headingGainSpin_->setSingleStep(0.1);
    headingGainSpin_->setValue(appConfig_.headingGain);

    lateralGainSpin_ = new QDoubleSpinBox();
    lateralGainSpin_->setRange(0.0, 5.0);
    lateralGainSpin_->setSingleStep(0.1);
    lateralGainSpin_->setValue(appConfig_.lateralGain);

    // ===== 放入面板 =====
    controlLayout->addWidget(createParamCard("控制器", controllerCombo_), 1);
    controlLayout->addWidget(createParamCard("速度", speedSpin_), 1);
    controlLayout->addWidget(createParamCard("前视距离", lookAheadSpin_), 1);
    controlLayout->addWidget(createParamCard("航向增益", headingGainSpin_), 1);
    controlLayout->addWidget(createParamCard("横向增益", lateralGainSpin_), 1);

    mainLayout->addWidget(controlPanel);
    // =========================
    // 2. 中间 2D 仿真区域
    // =========================

    QHBoxLayout *viewsLayout =
        new QHBoxLayout();

    view2D_ = new View2D(this);

    sensorView_ = new SensorView(this);

    controlMonitor_ = new ControlMonitor(this);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    rightLayout->addWidget(sensorView_, 2);

    rightLayout->addWidget(controlMonitor_, 1);

    viewsLayout->addWidget(view2D_, 2);

    viewsLayout->addLayout(rightLayout, 1);

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
    QStatusBar *bar = statusBar();

    // 底部状态栏高度
    bar->setMinimumHeight(32);

    // 单独设置状态栏样式，避免深色背景下文字看不清
    bar->setStyleSheet(
        "QStatusBar {"
        " background-color: #0b1220;"
        " color: #e2e8f0;"
        " border-top: 1px solid #1e293b;"
        " font-size: 13px;"
        " font-weight: 500;"
        " padding-left: 10px;"
        "}"
        "QStatusBar::item {"
        " border: none;"
        "}");

    bar->showMessage("ADASim 引擎就绪");
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

    bool success = socketServer_->startTcpServer(plannerPort_);

    if (success)
    {
        statusBar()->showMessage(QString("Planner TCP Server：%1").arg(plannerPort_));
    }
    else
    {
        statusBar()->showMessage("TCP Server启动失败");
    }
}

void MainWindow::saveConfig()
{
    // 从当前程序状态重新整理配置
    appConfig_.targetSpeed = targetVehicleSpeed_;

    appConfig_.lookAheadDistance = lookAheadDistance_;

    appConfig_.planningDistance = planningDistance_;

    appConfig_.headingGain = headingGainSpin_->value();

    appConfig_.lateralGain = lateralGainSpin_->value();

    appConfig_.plannerPort = plannerPort_;

    if (controllerMode_ == ControllerMode::PurePursuit)
    {
        appConfig_.controllerMode = "PurePursuit";
    }
    else
    {
        appConfig_.controllerMode = "DualError";
    }

    QString errorMessage;

    bool success = ConfigManager::save(configPath_, appConfig_, errorMessage);

    if (success)
    {
        LinuxLogger::info(QString("Config saved: %1").arg(configPath_));
    }
    else
    {
        LinuxLogger::warning(errorMessage);
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

// 平滑横向偏移过渡函数
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

    // 三阶多项式平滑（S型曲线，hermite平滑）
    double smooth = 3.0 * u * u - 2.0 * u * u * u;

    return planStartOffset_ + (targetLateralOffset_ - planStartOffset_) * smooth;
}

double MainWindow::calculatePlannedYaw(double x) const
{
    if (!lateralPlanActive_)
    {
        return 0.0;
    }

    const double sampleDistance = 0.1;

    double y1 = calculatePlannedOffset(x);
    double y2 = calculatePlannedOffset(x + sampleDistance);

    double dx = sampleDistance;
    double dy = y2 - y1;

    return std::atan2(dy, dx);
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

double MainWindow::normalizeAngle(double angle) const
{
    while (angle > M_PI)
    {
        angle -= 2.0 * M_PI;
    }

    while (angle < -M_PI)
    {
        angle += 2.0 * M_PI;
    }

    return angle;
}

void MainWindow::onReplayFrameSelected(int index)
{
    SimulationFrame frame;

    // 根据下标 index，取出仿真录制的第 index 帧数据，存入传入的 frame 引用；下标非法就返回 false，成功取出返回 true。
    if (!simulationRecorder_.frameAt(index, frame))
    {
        return;
    }

    // 拖时间轴时暂停实时仿真
    QMetaObject::invokeMethod(
        dataLoader_,
        "pause",
        Qt::QueuedConnection);

    replayMode_ = true;

    // 这里只显示历史状态，不改变VehicleModel真正的实时状态
    view2D_->showReplayFrame(
        frame.vehicle.x,
        frame.vehicle.y,
        frame.vehicle.yaw);

    view2D_->updateTrackingDebug(
        QPointF(frame.vehicle.x, frame.targetY),
        frame.lateralError,
        frame.steeringAngle);

    if (speedValue_)
    {
        speedValue_->setText(
            QString::number(frame.speedKmH, 'f', 1) + " km/h");
    }

    if (distanceValue_)
    {
        distanceValue_->setText(
            QString::number(frame.totalDistance, 'f', 1) + " m");
    }

    if (algoValue_)
    {
        double steeringDegree =
            frame.steeringAngle * 180.0 / M_PI;

        double headingDegree =
            frame.headingError * 180.0 / M_PI;

        algoValue_->setText(
            QString("回放 eY=%1m eYaw=%2° steer=%3°")
                .arg(frame.lateralError, 0, 'f', 2)
                .arg(headingDegree, 0, 'f', 1)
                .arg(steeringDegree, 0, 'f', 1));
    }

    timeLabel_->setText(
        QString("%1 / %2")
            .arg(index)
            .arg(simulationRecorder_.size() - 1));
}

void MainWindow::onFrontObstacleDistanceUpdated(
    double distance)
{
    frontObstacleDistance_ = distance;
}