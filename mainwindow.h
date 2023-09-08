#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "EmployeeModel.h"
#include "VacationModel.h"
#include "Chat/ChatServer.h"
#include "Chat/ChatClient.h"


#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool confirmDeletion();

    void dateOgonki();
    void dateLight(const QDate &eventDate, const QDate &currentDate, const QString &toolText, QColor color = Qt::green, bool isAppend = false);
    void removeRowFromModel(int row);
    void removeRowFromFile (int row);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void showEvent (QShowEvent  *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void init();
    void readSettings();
    void vacation(int month);

    void onConnectedToServer();

private slots:
    void on_pb_Add_clicked();
    void on_pb_Del_clicked();
    void on_pb_Save_clicked();
    void on_pb_Sort_clicked();
    void on_pb_Send_clicked();

    void removeLineFromFile(const QString& filename, int lineIndex);
    void handleHeaderClicked(int logicalIndex);

    void on_tb_Style_clicked(bool checked);
    void on_tb_Vacation_clicked(bool checked);
    void on_tb_Chat_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    EmployeeModel  *eModel     = nullptr;
    VacationModel  *vModel     = nullptr;

    ChatClient *client;
    ChatServer server;


    QString filePath;
    int sortClickCount = 0;

    bool isStyle    = false;
    bool isVacation = false;
    bool isChat     = false;
};
#endif // MAINWINDOW_H
