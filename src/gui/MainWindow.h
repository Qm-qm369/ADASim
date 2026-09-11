#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QSlider>
#include <QVector>
#include <QPointF>

class QLayout;
class View2D;
class DataLoader;
class QThread;
class QCloseEvent;
class DataManager;
class SensorView;
class SocketServer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &configPath,
                        const QString &dataPath,
                        QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // 规划执行完以后，这是车辆真正的位置
    void trueVehiclePositionReady(double x, double y, double yaw);

private:
    void setupUI();

    /*
     ┌─────────────────────────────┐
    │      menuBar() 菜单栏        │ ← 最顶部，文件、编辑
    ├─────────────────────────────┤
    │ addToolBar() 工具栏区域      │ ← 可以顶部/左边/右边
    ├─────────────────────────────┤
    │                             │
    │     centralWidget()         │ ← 【中心部件】你的绘图画布放这里！最重要
    │      (你的绘图widget)        │
    │                             │
    ├─────────────────────────────┤
    │     statusBar() 状态栏       │ ← 死死固定在窗口最底部
    └─────────────────────────────┘

    */

    void setupToolBar();   // 创建顶部工具栏
    void setupStatusBar(); // 创建底部状态栏
    void setupConnections();

    // 创建顶部信息卡片
    void createInfoCard(QLayout *layout,
                        const QString &title,
                        const QString &value,
                        const QString &objectName);

    // 启动后台线程
    void startBackend();
    // 安全停止后台线程
    void stopBackend();

protected:
    // 窗口关闭时安全结束后台线程
    void closeEvent(QCloseEvent *event) override;

    // V0.10
    void setupNetwork();

private slots:

    void onLateralControlReceived(double offset);

    void onStartSimulation();
    void onPauseSimulation();
    void onStopSimulation();
    void onStatusUpdate(const QString &status);
    void onVehicleDataUpdated(double x,
                              double y,
                              double yaw);

    void startLateralPlan(double targetOffset);    // 创建一轮新的横向规划
    double calculatePlannedOffset(double x) const; // 根据当前X计算车辆现在应该处于什么横向位置
    void rebuildPlannedTrajectory();               // 提前生成一些轨迹点供View2D显示

private:
    // 行车记录仪时间轴
    QSlider *timeSlider_ = nullptr;
    // 显示 当前帧 / 最大帧
    QLabel *timeLabel_ = nullptr;

    // 顶部数据显示控件
    QLabel *speedValue_ = nullptr;
    QLabel *distanceValue_ = nullptr;
    QLabel *fpsValue_ = nullptr;
    QLabel *algoValue_ = nullptr;

    // 后台数据线程
    QThread *backendThread_ = nullptr;
    // 数据生成器
    DataLoader *dataLoader_ = nullptr;

    DataManager *dataManager_ = nullptr;

    SensorView *sensorView_ = nullptr;

    double totalDistance_ = 0.0;

    double lastX_ = 0.0;
    double lastY_ = 0.0;

    View2D *view2D_ = nullptr;

    QString configPath_;
    QString dataPath_;

    // V0.10 TCP服务器
    SocketServer *socketServer_ = nullptr;

    double targetLateralOffset_ = 0.0;  // 算法想让我去哪
    double currentLateralOffset_ = 0.0; // 车辆现在实际移动到哪

    double planStartX_ = 0.0;        // 这一轮变道从哪个X开始
    double planStartOffset_ = 0.0;   // 开始变道时车辆真实横向位置
    double planningDistance_ = 20.0; // 准备在多少米纵向距离内完成变道

    bool lateralPlanActive_ = false; // 当前有没有正在执行的横向轨迹

    QVector<QPointF> plannedTrajectory_; // 真正准备执行并显示的轨迹点
};

#endif
