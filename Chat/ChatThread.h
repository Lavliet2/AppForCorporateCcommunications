#ifndef CHATTHREAD_H
#define CHATTHREAD_H

#include <QThread>

class QTcpSocket;

class ChatThread : public QThread
{
    Q_OBJECT

public:
    ChatThread(qintptr socketDescriptor, QObject *parent = nullptr);

signals:
    void messageReceived(QString sender, QString message);

protected:
    void run() override;

private:
    qintptr socketDescriptor;
};

#endif // CHATTHREAD_H
