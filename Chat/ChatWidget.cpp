#include "ChatWidget.h"
#include "ui_ChatWidget.h"

//#include <QObject>

#include <ws2tcpip.h>
#include <winsock.h>

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
{
    ui->setupUi(this);

    client = new ChatClient(ui->employeeListWidget, this);

    // Установка соединения с сервером
    client->connectToServer("192.168.0.11", 8080);

    // Связь кнопки с отправкой сообщения
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWidget::sendMessage);

    // Слушатель для получения входящих сообщений
    connect(client, &ChatClient::messageReceived, this, &ChatWidget::onMessageReceived);

    employeeItemModel = new EmployeeItemModel(new EmployeeModel(this));
    ui->listView->setModel(employeeItemModel);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

QSplitter *ChatWidget::getSplitter()
{
    return ui->splitter;
}

void ChatWidget::setSplitter(QByteArray splitterState)
{
    ui->splitter->restoreState(splitterState);
}

void ChatWidget::sendMessage()
{
    QString message = ui->messageLineEdit->text();
    if (!message.isEmpty()) {
        // Отправка сообщения на сервер
        client->sendMessage(message);
        // Очистка поля ввода
        ui->messageLineEdit->clear();

        // Добавление отправленного сообщения в chatDisplayTextEdit
        QString currentText = ui->chatDisplayTextEdit->toPlainText();
        currentText += "Вы: " + message + "\n";
        ui->chatDisplayTextEdit->setPlainText(currentText);

        // Прокрутка вниз, чтобы видеть новые сообщения
        QTextCursor cursor = ui->chatDisplayTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->chatDisplayTextEdit->setTextCursor(cursor);
    }
}

void ChatWidget::onMessageReceived(QString message)
{
    qDebug() << Q_FUNC_INFO << message;
    // Вывод полученного сообщения в консоль (в реальном приложении это должно выводиться в интерфейс)
    qDebug() << "Получено сообщение от сервера:" << message;

    // Получение текущего текста из QTextEdit
    QString currentText = ui->chatDisplayTextEdit->toPlainText();

    // Добавление нового сообщения к текущему тексту с переносом строки
    currentText += message + "\n";

    // Установка обновленного текста в QTextEdit
    ui->chatDisplayTextEdit->setPlainText(currentText);

    // Прокрутка вниз, чтобы видеть новые сообщения
    QTextCursor cursor = ui->chatDisplayTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->chatDisplayTextEdit->setTextCursor(cursor);
}
