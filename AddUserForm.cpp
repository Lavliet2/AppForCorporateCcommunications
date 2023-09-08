#include "AddUserForm.h"
#include "ui_AddUserForm.h"
#include <QDebug>
#include <QMessageBox>

#include "EmployeeModel.h"
#include <QProcess>

#include <QCoreApplication>
#include <ws2tcpip.h>
#include <winsock.h>


AddUserForm::AddUserForm(Employee &employee, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddUserForm)
    , employee(employee)
{
    ui->setupUi(this);
}

void AddUserForm::done(int result)
{
    if ( result != QDialog::Accepted ) {
        QDialog::done(result);
    }
    else {

        QStringList errorList;

        employee.name     = ui->editline_Name->text();
        employee.famili   = ui->editline_Famili->text();
        employee.otch     = ui->lineEdit_Otch->text();
        employee.post     = ui->lineEdit_Post->text();
        employee.birthday = ui->dateEdit->date().toString("dd.MM.yyyy");
        employee.email    = ui->lineEdit_Email->text();
        employee.phone    = ui->lineEdit_Phone->text();
        employee.gender   = ui->cb_Gender->currentText();

        employee.ip       = ui->lineEdit_IP->text();

        if ( employee.name.isEmpty()     )  errorList << "Поле «Имя» пустое";
        if ( employee.famili.isEmpty()   )  errorList << "Поле «Фамилия» пустое";
        if ( employee.otch.isEmpty()     )  errorList << "Поле «Отчество» пустое";
        if ( employee.post.isEmpty()     )  errorList << "Поле «Должность» пустое";
        if ( employee.birthday.isEmpty() )  errorList << "Поле «Дата рождения» пустое";
        if ( employee.email.isEmpty()    )  errorList << "Поле «Почта» пустое";
        if (!employee.email.contains("@") || !employee.email.contains(".")  )
            errorList    << "Поле «Почта» заполнено некорректно, отсутствуют специальные символы «@»«.»";

        if ( !errorList.isEmpty() ) {
            QMessageBox::warning(nullptr, "Предупреждение", errorList.join(";\n"));
            return;
        }

        QDialog::done(result);
    }
}


AddUserForm::~AddUserForm()
{
    delete ui;
}

QString AddUserForm::getIp()
{
    WSADATA wsaData;
    if ( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 ) {
        return "Error";
    }
    char hostName[256];
    if ( gethostname(hostName, sizeof(hostName)) != 0 ) {
        WSACleanup();
        return "Error";
    }

    struct addrinfo *result = nullptr;
    if ( getaddrinfo(hostName, nullptr, nullptr, &result) != 0 ) {
        WSACleanup();
        return "Error";
    }

    struct sockaddr_in *sockadd_ipv4 = reinterpret_cast<struct sockaddr_in *>(result->ai_addr);

    freeaddrinfo(result);
    WSACleanup();
    return inet_ntoa(sockadd_ipv4->sin_addr);
}

void AddUserForm::on_tb_getIp_clicked()
{
    ui->lineEdit_IP->setText(getIp());
}
