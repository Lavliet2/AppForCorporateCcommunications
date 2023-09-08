#include "ChatServer.h"
#include <QDebug>
#include <qlistwidget.h>

#include <QTcpSocket>

ChatServer::ChatServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    if ( !server->listen(QHostAddress::Any, 8088) ) qDebug() << "Ошибка запуска сервера";
    else                                            qDebug() << "Сервер запущен на порту" << 8088;
    connect(server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}

void ChatServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();
    activeClients.append(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);
}

void ChatServer::onReadyRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if ( clientSocket ) {
        QByteArray data = clientSocket->readAll();
        // Отправить полученное сообщение всем подключенным клиентам
        for ( QTcpSocket *client : activeClients )
            if ( client != clientSocket )
                 client->write(data);

    }
}

void ChatServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if ( clientSocket ) {
        activeClients.removeOne(clientSocket);
        clientSocket->deleteLater();
    }
}
