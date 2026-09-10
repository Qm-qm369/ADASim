#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QByteArray>
#include <QtGlobal>

class QTcpServer;
class QTcpSocket;

/**
 * V0.10
 *
 * 简单 TCP Server：
 * C++ 负责监听
 * Python 负责连接
 */
class SocketServer : public QObject
{
    Q_OBJECT

public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();

    // 启动 TCP Server
    bool startTcpServer(quint16 port);

    // 停止服务器
    void stop();

public slots:

    // C++ -> Python
    void sendToClient(const QByteArray &data);

signals:

    // Python -> C++
    void dataReceived(const QByteArray &data);
    void clientConnected();
    void clientDisconnected();

private slots:

    // Python连接进来了
    void onNewConnection();

    // Python发数据过来了
    void onReadyRead();

    void onClientDisconnected();

private:
    QTcpServer *server_ = nullptr; // 监听端口、等待客户端连接。

    QTcpSocket *client_ = nullptr;
};

#endif