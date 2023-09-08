#ifndef VACATIONMODEL_H
#define VACATIONMODEL_H

#include "EmployeeModel.h"

#include <QAbstractTableModel>
#include <QDate>
#include <QStyledItemDelegate>
#include <QPainter>

struct EmployeeVacation
{
    QString fio;
    QString type;
    QList<bool> dayList;
    QColor color = Qt::yellow;
};

class VacationModel : public QAbstractTableModel
{
public:
    VacationModel(EmployeeModel *eModel, QComboBox *cb, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int  rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int  columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void addItem(EmployeeVacation * ev);
    void clear();


//    void fiilData();
    QList<EmployeeVacation *> getEmployeeVacationList() {return employeeVacationList;};

private:
    EmployeeModel *eModel;
    QList<EmployeeVacation *> employeeVacationList;
    QComboBox *cb;

    QStringList headers;
};



class VacationDelegate : public QStyledItemDelegate
{
public:
    VacationDelegate(QList<EmployeeVacation *> employeeVacationList, QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
        , employeeVacationList(employeeVacationList){}
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
//        QVariant data = index.data();

        if ( employeeVacationList[index.row()]->dayList[index.column()] == true )
            painter->fillRect(option.rect, employeeVacationList[index.row()]->color);
        else return;

    };
private:
    QList<EmployeeVacation *> employeeVacationList;
};

#endif // VACATIONMODEL_H
