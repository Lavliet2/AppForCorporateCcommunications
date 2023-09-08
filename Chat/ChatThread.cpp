#include "ChatThread.h"
#include <QTcpSocket>

ChatThread::ChatThread(qintptr socketDescriptor, QObject *parent)
    : QThread(parent)
    , socketDescriptor(socketDescriptor) {}

void ChatThread::run()
{
    QTcpSocket socket;
    if ( ! socket.setSocketDescriptor(socketDescriptor) ) {
        qDebug() << "Ошибка при установлении соксета:" << socket.errorString();
        return;
    }

    while ( socket.state() == QAbstractSocket::ConnectedState ) {
        QByteArray data = socket.readAll();
        if ( !data.isEmpty() ) {
            // Обработка принятых данных (сообщений) от клиента
            QString message = QString::fromUtf8(data);
            emit messageReceived(socket.peerAddress().toString(), message);
        }
    }

    // Закрытие соксета после завершения обмена
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}
