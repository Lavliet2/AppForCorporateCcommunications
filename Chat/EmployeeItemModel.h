#ifndef EMPLOYEEITEMMODEL_H
#define EMPLOYEEITEMMODEL_H

#include <EmployeeModel.h>
#include <QAbstractItemModel>


class EmployeeItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    EmployeeItemModel(EmployeeModel *sourceModel, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

private:
    EmployeeModel *sourceModel;
};

#endif // EMPLOYEEITEMMODEL_H
