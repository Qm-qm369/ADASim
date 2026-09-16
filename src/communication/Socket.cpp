#include "Socket.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

SocketServer::SocketServer(
    QObject *parent)
    : QObject(parent)
{
    // 创建 TCP 服务器对象。
    server_ = new QTcpServer(this);

    // Qt 的 TCP 服务器发现有新客户端连接时，自动执行我们自己写的 onNewConnection()。
    connect(server_, &QTcpServer::newConnection,
            this, &SocketServer::onNewConnection);
}

SocketServer::~SocketServer()
{
    stop();
}

bool SocketServer::startTcpServer(
    quint16 port)
{
    if (server_->isListening())
    {
        return true;
    }

    bool success = server_->listen(QHostAddress::LocalHost, port);

    if (!success)
    {
        // server_->errorString() Qt 网络类给你的真正错误描述
        emit networkError(QString("TCP listen failed: %1").arg(server_->errorString()));
    }

    return success;
}

void SocketServer::stop()
{
    if (client_)
    {
        client_->disconnectFromHost();
        client_ = nullptr;
    }

    if (server_->isListening())
    {
        server_->close();
    }
}

// C++ 端向已经连接的 Python 客户端发送数据。
void SocketServer::sendToClient(
    const QByteArray &data)
{
    if (!client_)
    {
        return;
    }

    if (client_->state() != QTcpSocket::ConnectedState)
    {
        return;
    }

    qint64 written = client_->write(data);

    if (written < 0)
    {
        emit networkError(QString("TCP send failed: %1").arg(client_->errorString()));

        return;
    }

    client_->flush();
}

// 只允许一个客户端接入
void SocketServer::onNewConnection()
{
    if (!server_->hasPendingConnections())
    {
        return;
    }

    QTcpSocket *newClient = server_->nextPendingConnection();

    // 已经存在Planner连接
    if (client_ && client_->state() == QTcpSocket::ConnectedState)
    {
        emit networkError("A second Planner connection "
                          "was rejected");

        newClient->disconnectFromHost();
        newClient->deleteLater();

        return;
    }

    client_ = newClient;

    connect(client_, &QTcpSocket::readyRead,
            this, &SocketServer::onReadyRead);

    connect(client_, &QTcpSocket::disconnected,
            this, &SocketServer::onClientDisconnected);

    connect(client_, &QTcpSocket::disconnected,
            client_, &QObject::deleteLater);

    emit clientConnected();
}

void SocketServer::onReadyRead()
{
    if (!client_)
    {
        return;
    }

    /*
     * 每条JSON数据都以 '\n' 结束。
     *
     * 所以这里一行就是一条完整消息。
     */
    while (client_->canReadLine()) // 当前缓存里有没有完整的一行数据。
    {
        QByteArray line = client_->readLine().trimmed(); // trimmed()去掉开头和结尾的空白字符。

        if (!line.isEmpty())
        {
            emit dataReceived(line);
        }
    }
}

void SocketServer::onClientDisconnected()
{
    client_ = nullptr;

    emit clientDisconnected();
}