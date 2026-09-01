#include "MainWindow.h"
#include "View2D.h"
#include "backend/DataLoader.h"

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

    // V0.4 暂时让 DataLoader 工作在 GUI 主线程
    dataLoader_ = new DataLoader(dataPath_, this);

    // 建立数据连接
    setupConnections();
}

MainWindow::~MainWindow()
{
}

// 建立真正的数据连接
void MainWindow::setupConnections()
{
    // DataLoader -> MainWindow
    connect(dataLoader_,
            &DataLoader::vehiclePositionReady,
            this,
            &MainWindow::onVehicleDataUpdated);

    // DataLoader -> 状态栏
    connect(dataLoader_,
            &DataLoader::statusUpdate,
            this,
            &MainWindow::onStatusUpdate);
}

void MainWindow::onStartSimulation()
{
    dataLoader_->start();
}

void MainWindow::onPauseSimulation()
{
    dataLoader_->pause();
}

void MainWindow::onStopSimulation()
{
    dataLoader_->stop();

    // 重置界面数据
    totalDistance_ = 0.0;
    lastX_ = 0.0;
    lastY_ = 0.0;

    if (speedValue_)
        speedValue_->setText("0.0 km/h");

    if (distanceValue_)
        distanceValue_->setText("0.0 m");

    // 重置视图
    if (view2D_)
        view2D_->updateVehiclePosition(0.0, 0.0, 0.0);
}

void MainWindow::onStatusUpdate(const QString &status)
{
    statusBar()->showMessage(status);
}

void MainWindow::onVehicleDataUpdated(double x,
                                      double y,
                                      double yaw)
{
    // 更新二维视图
    view2D_->updateVehiclePosition(x, y, yaw);

    // 从第二帧开始计算位移
    if (lastX_ != 0.0 || lastY_ != 0.0)
    {
        double dx = x - lastX_;
        double dy = y - lastY_;

        double distance =
            std::sqrt(dx * dx + dy * dy);

        totalDistance_ += distance;

        // 每帧间隔 0.1 秒
        double speed =
            distance / 0.1;

        // m/s -> km/h
        double speedKmH =
            speed * 3.6;

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
    }

    // 保存这一帧的位置
    lastX_ = x;
    lastY_ = y;
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

    view2D_ = new View2D(this);

    view2D_->setStyleSheet(
        "View2D {"
        "border: 2px solid #26364d;"
        "border-radius: 8px;"
        "}");

    // 让 View2D 占据主要空间
    mainLayout->addWidget(view2D_, 1);

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