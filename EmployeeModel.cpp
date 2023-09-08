#include "EmployeeModel.h"
#include "ProcessHandler.hpp"

#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QTextStream>
#include <QDate>
#include <QTimer>
#include <QThread>

//class ProcessHandler;
///######################################################################
/// @class EmployeeModel модель сотрудников отдела
///######################################################################
EmployeeModel::EmployeeModel(QObject *parent)
    : QAbstractTableModel(parent)
    , filePath("C:/temp/database.txt")
//    , filePath("O:/06. Данные отделов/НИО ММНМ/Рабочие данные/Собственные программы/Murzin_AV/Base/birthdays.txt")

{
    // Инициализация заголовков столбцов
    headers << "Фамилия" << "Имя" << "Отчество" << " Должность " << " Дата рождения " << "Пол" << "Телефон" << "Почта" << "IP" ;
    fillDataList();

    startProcessing();
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &EmployeeModel::startProcessing);
    timer->start(600000);// таймер обновления IP
}


int EmployeeModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() ) return 0;
    return employeeList.count();
}


int EmployeeModel::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() ) return 0;
    return headers.size();
}


QVariant EmployeeModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() ) return QVariant();

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {

        switch (index.column())
        {
        case EColumn::Famili:
            return employeeList[index.row()]->famili;
        case EColumn::Name:
            return employeeList[index.row()]->name;
        case EColumn::Otch:
            return employeeList[index.row()]->otch;
        case EColumn::Post:
            return employeeList[index.row()]->post;
        case EColumn::Birthday:
        {
            // Преобразование строки в QDate
            QDate birthDate = QDate::fromString(employeeList[index.row()]->birthday, "dd.MM.yyyy");
            if ( birthDate.isValid() ) return birthDate.toString("dd.MM.yyyy"); // Форматирование QDate обратно в строку для отображения
            else                       return QVariant();                       // Некорректная дата
        }

        case EColumn::Email:
            return employeeList[index.row()]->email;
        case EColumn::Phone:
            return employeeList[index.row()]->phone;
        case EColumn::Gender:
            return employeeList[index.row()]->gender;
        case EColumn::IP:
            return employeeList[index.row()]->ip;

        }
    }
    if ( role == Qt::ToolTipRole && index.column() == Birthday ) {
        QDate birthDate = QDate::fromString(employeeList[index.row()]->birthday, "dd.MM.yyyy");
        if ( birthDate.isValid() ) {
            int age = calculateAge(birthDate);
            return "Возраст: " + QString::number(age);
        }
        else return "Invalid birth date";
    }
    return QVariant();
}

QVariant EmployeeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Vertical && role == Qt::BackgroundRole ) {
        if ( employeeList[section]->ping ) return QVariant(QColor(0, 255, 0));
        else                               return QVariant(QColor(255, 0, 0));
    }

    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal && section >= 0 && section < headers.size() ) {
        return headers[section];
    }
    return QVariant();
}


bool EmployeeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() && role == Qt::EditRole ) {
        switch (index.column())
        {
        case EColumn::Famili:
            employeeList[index.row()]->famili  = value.toString().trimmed();
            break;
        case EColumn::Name:
            employeeList[index.row()]->name = value.toString().trimmed();
            break;
        case EColumn::Otch:
            employeeList[index.row()]->otch  = value.toString().trimmed();
            break;
        case EColumn::Post:
            employeeList[index.row()]->post  = value.toString().trimmed();
            break;
        case EColumn::Birthday:
        {
            QString birthday = value.toString();

            QDate date = QDate::fromString(birthday, "dd.MM.yyyy");
            if (date.isValid()) {
                employeeList[index.row()]->birthday = birthday;
            } else {
                QMessageBox::warning(nullptr, "Предупреждение", "Поле «ДЕНЬ РОЖДЕНИЯ» заполнено некорректно");
                return false;
            }
        }
            break;
        case EColumn::Email:
        {
            QString email = value.toString();
            if ( email.contains("@") && email.contains(".") ) {
                employeeList[index.row()]->email = email;
            } else {
                QMessageBox::warning(nullptr, "Предупреждение", "Поле «Почта» заполнено некорректно, отсутсвуют специальные символы «@»«.»");
                return false;
            }
        }
            break;
        case EColumn::Phone:
            employeeList[index.row()]->phone = value.toString().trimmed();
            break;
        case EColumn::Gender:
            employeeList[index.row()]->gender = value.toString().trimmed();
            break;
        case EColumn::IP:
            employeeList[index.row()]->ip = value.toString().trimmed();
            break;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}


Qt::ItemFlags EmployeeModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() ) return Qt::NoItemFlags;
    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


bool EmployeeModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    employeeList.removeAt(row);

    return true;
}

void EmployeeModel::addData(Employee *employee)
{
    beginInsertRows(QModelIndex(), employeeList.size(), employeeList.size());
    employeeList.append(employee);
    endInsertRows();
}

void EmployeeModel::sort(int column, Qt::SortOrder order, bool sortBy)
{
    sortOrder = order;
    sortColumn = column;
    sortData(sortBy);
}


void EmployeeModel::startProcessing()
{
    qDebug() << "startProcessing;";
    for ( int i = 0; i< employeeList.count(); i++ ) {
        Employee *employee = employeeList.at(i);
        ProcessHandler *handler = new ProcessHandler(employee);
        handler->index = i;

        thread = new QThread;
        handler->moveToThread(thread);

        QObject::connect(thread,  &QThread::started, handler, &ProcessHandler::startProcess);
        QObject::connect(handler, &ProcessHandler::processFinished, this, &EmployeeModel::handeProcessResult);
        QObject::connect(thread,  &QThread::finished, thread, &QThread::deleteLater);

        thread->start();
    }

}

void EmployeeModel::processResultReady(int index, QString result){Q_UNUSED(index); Q_UNUSED(result)}
void EmployeeModel::handeProcessResult(int index, QString result)
{
    emit processResultReady(index, result);
    layoutChanged();
}


void EmployeeModel::fillDataList()
{
    // Открываем файл
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл";
        return;
    }
    // Читаем информацию из файла
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QStringList splitLine = in.readLine().split('\t');

        Employee *empl = new Employee;

        empl->famili   = splitLine[Famili];
        empl->name     = splitLine[Name];
        empl->otch     = splitLine[Otch];
        empl->post     = splitLine[Post];
        empl->birthday = splitLine[Birthday];
        empl->email    = splitLine[Email];
        empl->phone    = splitLine[Phone];
        empl->gender   = splitLine[Gender];
        empl->ip       = splitLine[IP];


        employeeList.append(empl);
    }
    // Закрываем файл
    file.close();
}


int EmployeeModel::daysUntilBirthday(const QDate &date)
{
    QDate currentDate = QDate::currentDate();
    QDate nextBirthday(currentDate.year(), date.month(), date.day());

    if (nextBirthday < currentDate)
        nextBirthday = nextBirthday.addYears(1);

    return currentDate.daysTo(nextBirthday);
}

void EmployeeModel::sortData(bool sortBy)
{
    if ( sortBy ) {
        std::sort(employeeList.begin(), employeeList.end(), [this](const Employee *a, const Employee *b) {
            QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
            QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

            if (!dateA.isValid() || !dateB.isValid())
                return false;

            int daysUntilBirthdayA = daysUntilBirthday(dateA);
            int daysUntilBirthdayB = daysUntilBirthday(dateB);

            if (sortOrder == Qt::AscendingOrder)
                return daysUntilBirthdayA < daysUntilBirthdayB;
            else
                return daysUntilBirthdayA > daysUntilBirthdayB;
        });
    }
    else {
        switch ( sortColumn )
        {
        case Famili: case Name: case Otch: case Post: case Email: case Phone: case Gender: case IP:
            std::sort(employeeList.begin(), employeeList.end(), [this](const Employee *a, const Employee *b) {
                //            return compareNames(a, b);
                if (sortOrder == Qt::AscendingOrder) {
                    if      ( sortColumn == Famili)
                        return a->famili < b->famili;
                    else if ( sortColumn == Name)
                        return a->name < b->name;
                    else if ( sortColumn == Otch)
                        return a->otch < b->otch;
                    else if ( sortColumn == Post)
                        return a->post < b->post;
                    else if ( sortColumn == Email)
                        return a->email < b->email;

                    else if ( sortColumn == Phone)
                        return a->phone < b->phone;
                    else if ( sortColumn == Gender)
                        return a->gender < b->gender;
                    else if ( sortColumn == IP)                        
                        return a->ping < b->ping;

                }
                else
                    if      ( sortColumn == Famili)
                        return a->famili > b->famili;
                    else if ( sortColumn == Name)
                        return a->name > b->name;
                    else if ( sortColumn == Otch)
                        return a->otch > b->otch;
                    else if ( sortColumn == Post)
                        return a->post > b->post;
                    else if ( sortColumn == Email)
                        return a->email > b->email;

                    else if ( sortColumn == Phone)
                        return a->phone > b->phone;
                    else if ( sortColumn == Gender)
                        return a->gender > b->gender;
                    else if ( sortColumn == IP)
                        return a->ping > b->ping;

                return  a->famili > b->famili; //хз
            });

            break;
        case Birthday:
        {
            std::sort(employeeList.begin(), employeeList.end(), [this](const Employee *a, const Employee *b) {
                QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
                QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

                if (!dateA.isValid() || !dateB.isValid())
                    return false;

                if (sortOrder == Qt::AscendingOrder)
                    return dateA < dateB;
                else
                    return dateA > dateB;
            });

        }
        default:
            break;
        }
    }

    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

int EmployeeModel::calculateAge(const QDate &birthDate) const
{
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - birthDate.year();

    if (birthDate.month() > currentDate.month() ||
            (birthDate.month() == currentDate.month() && birthDate.day() > currentDate.day())) {
        age--;
    }

    return age;
}

