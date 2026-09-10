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

bool SocketServer::startTcpServer(quint16 port)
{
    // isListening() 是 QTcpServer 自带函数 判断服务器是否处于监听状态
    if (server_->isListening())
    {
        return true;
    }

    return server_->listen(QHostAddress::LocalHost, port); // 在本机指定的 port 端口等待客户端连接。
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
void SocketServer::sendToClient(const QByteArray &data)
{
    if (!client_)
    {
        return;
    }

    if (client_->state() != QTcpSocket::ConnectedState)
    {
        return;
    }

    client_->write(data); // 真正发送
    client_->flush();     // 作用是把当前待发送的数据尽量写出去。
}

void SocketServer::onNewConnection()
{
    if (!server_->hasPendingConnections()) // 有没有一个已经连过来、等待我们接收的客户端。
    {
        return;
    }

    // V0.10只支持一个Python Planner
    client_ = server_->nextPendingConnection(); // 把这个刚连接进来的客户端取出来，得到一个 QTcpSocket*。

    connect(client_, &QTcpSocket::readyRead,
            this, &SocketServer::onReadyRead);

    connect(client_, &QTcpSocket::disconnected,
            this, &SocketServer::onClientDisconnected);

    // 二者均是自带的 意思就是客户端断开以后，让 Qt 后续安全地删除这个 client_ 对象。
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