#include "VacationModel.h"

#include <QDebug>
#include <QFileInfo>
#include <QTextStream>
#include <QDate>

VacationModel::VacationModel(EmployeeModel *eModel, QComboBox *cb, QObject *parent)
    : QAbstractTableModel(parent)
    , eModel(eModel)
    , cb(cb) {}

QVariant VacationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal && role == Qt::BackgroundRole ) {
        QDate date = QDate(QDate::currentDate().year(), cb->currentIndex() + 1, section + 1);
        if ( date.dayOfWeek() == 6 || date.dayOfWeek() == 7)
            return QVariant(QColor(165, 38, 10));
    }

    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal && section >= 0 /*&& section < headers.size()*/ ) {
        return section+1;
    }

    if ( orientation == Qt::Vertical && section >= 0 ) {
        QStringList fioIni = employeeVacationList[section]->fio.split(" ");
            QString fio = fioIni[0] + " " + fioIni[1][0] + ". " + fioIni[2][0] + ".";
            return fio;
    }
    return QVariant();
}

QVariant VacationModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) return QVariant();

    if ( role == Qt::DecorationRole ) {
        if ( employeeVacationList[index.row()]->dayList[index.column()] == true )
            return employeeVacationList[index.row()]->color;
    }
    return QVariant();
}

int VacationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return employeeVacationList.count();
}

int VacationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return employeeVacationList[0]->dayList.count();
}

void VacationModel::addItem(EmployeeVacation *ev)
{
    beginInsertRows(QModelIndex(), employeeVacationList.size(), employeeVacationList.size());
    employeeVacationList.append(ev);
    endInsertRows();
}

void VacationModel::clear()
{
    employeeVacationList.clear();
}
