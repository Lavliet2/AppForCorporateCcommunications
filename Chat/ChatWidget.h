#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include "ChatClient.h"
#include "EmployeeModel.h"
#include "EmployeeItemModel.h"

#include <QWidget>
#include <QSplitter>
#include <QSortFilterProxyModel>


namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    QSplitter *getSplitter();
    void setSplitter(QByteArray splitterState);

private slots:
    void sendMessage();
    void onMessageReceived(QString message);

private:
    Ui::ChatWidget *ui;
    ChatClient *client;
    EmployeeModel employeeModel;
    EmployeeItemModel *employeeItemModel;
};

#endif // CHATWIDGET_H
