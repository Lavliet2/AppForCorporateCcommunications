#include "ChatClient.h"

#include <algorithm>
#include <QDataStream>


ChatClient::ChatClient(QListWidget *employeeListWidget, QObject *parent)
    : QObject(parent)
    , socket(new QTcpSocket(this))
    , employeeListWidget(employeeListWidget)
{
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
}

void ChatClient::connectToServer(QString host, int port)
{
    socket->connectToHost(host, port);
}

void ChatClient::sendMessage(QString message)
{
    socket->write(message.toUtf8());
}

void ChatClient::onReadyRead()
{
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    emit messageReceived(message);
}
