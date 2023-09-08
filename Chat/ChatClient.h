#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include "EmployeeModel.h"

#include <QObject>
#include <QTcpSocket>
#include <QListWidget>

class ChatClient : public QObject
{
    Q_OBJECT

public:
    ChatClient(QListWidget *employeeListWidget, QObject *parent = nullptr);


public slots:
    void connectToServer(QString host, int port);
    void sendMessage(QString message);

signals:
    void messageReceived(QString message);

private slots:
    void onReadyRead();

private:
    QTcpSocket       *socket;
    QListWidget      *employeeListWidget; // Ссылка на виджет списка сотрудников
    EmployeeModel     employeeModel;
};
#endif // CHATCLIENT_H
