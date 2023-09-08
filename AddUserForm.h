#ifndef ADDUSERFORM_H
#define ADDUSERFORM_H

#include <QDialog>
#include "QDate"

struct Employee;

namespace Ui {
class AddUserForm;
}

class AddUserForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserForm(Employee &employee, QWidget *parent = nullptr);
    virtual void done(int result)override;
    ~AddUserForm();

    QString getIp();

private slots:
    void on_tb_getIp_clicked();

private:
    Ui::AddUserForm *ui;
    Employee &employee;

};

#endif // ADDUSERFORM_H
