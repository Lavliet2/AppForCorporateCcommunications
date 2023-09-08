#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "EmployeeModel.h"

#include <QObject>
#include <QTcpServer>
//#include <QTcpSocket>
//#include <QList>


class ChatServer : public QObject
{
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> activeClients;
    EmployeeModel employeeModel;
};

#endif // CHATSERVER_H
