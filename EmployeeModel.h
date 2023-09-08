#ifndef EMPLOYEEMODEL_H
#define EMPLOYEEMODEL_H

#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QComboBox>

///######################################################################
/// @class GenderDelegate для QComboBox для колонки "Gender"
///######################################################################
class GenderDelegate : public QStyledItemDelegate
{
public:
    GenderDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(option); Q_UNUSED(index);
        QComboBox *editor = new QComboBox(parent);
        editor->addItem("М");
        editor->addItem("Ж");
        return editor;
    }
};


struct Employee
{
    QString famili;
    QString name;
    QString otch;
    QString post;
    QString birthday;
    QString email;
    QString phone;
    QString gender;
    QString ip;
    bool ping = false;
    bool isConnected = false;
};

///######################################################################
/// @class EmployeeModel модель сотрудников отдела
///######################################################################
class EmployeeModel : public QAbstractTableModel
{

public:
    enum EColumn
    {
        Famili,
        Name,
        Otch,
        Post,
        Birthday,
        Gender,
        Phone,
        Email,
        IP
    };

    EmployeeModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role) override;
    void     addData(Employee *employee);

    int  rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int  columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void sort(int column, Qt::SortOrder order, bool sortBy = false);//override;

    void startProcessing();
    QList<Employee *> getEmployeeList() {return employeeList;};


signals:
    void processResultReady(int index, QString result);

private slots:
    void handeProcessResult(int index, QString result);

private:
    void fillDataList();
    void sortData(bool sortBy = false);

    int calculateAge(const QDate &birthDate) const;
    static int daysUntilBirthday(const QDate &date);

public:
    Qt::SortOrder sortOrder = Qt::AscendingOrder;        

private:
    QString filePath;
    QStringList dataList;

    QList<Employee *> employeeList;
    QStringList headers; // Заголовки столбцов
    QThread *thread;

    int sortColumn = -1;
    QTimer *timer;
};

#endif // EMPLOYEEMODEL_H
