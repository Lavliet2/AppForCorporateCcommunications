#include "EmployeeItemModel.h"


EmployeeItemModel::EmployeeItemModel(EmployeeModel *sourceModel, QObject *parent)
    : QAbstractItemModel(parent)
    , sourceModel(sourceModel) {}

int EmployeeItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return sourceModel->rowCount();
}

int EmployeeItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return 1; // Одна колонка для фамилии
}

QVariant EmployeeItemModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) return QVariant();

    if ( role == Qt::DisplayRole ) {
        int row = index.row();
        if ( row >= 0 && row < sourceModel->rowCount() ) {
            Employee *employee = sourceModel->getEmployeeList().at(row);
            return QString("%1 - %2").arg(employee->famili).arg(employee->isConnected);
        }
    }

    return QVariant();
}

QModelIndex EmployeeItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) ) return QModelIndex();
    return createIndex(row, column);
}

QModelIndex EmployeeItemModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

