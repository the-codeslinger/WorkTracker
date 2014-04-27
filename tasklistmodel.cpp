#include "tasklistmodel.h"
#include "tasklist.h"

#include <QDomElement>
#include <QDomNamedNodeMap>
#include <QDate>
#include <QDebug>

TaskListModel::TaskListModel(TaskList* tasks, QObject* parent)
    : QAbstractListModel(parent)
    , m_tasks(tasks)
{
}

int
TaskListModel::rowCount(const QModelIndex&) const
{
    // We don't have child-parent relationships, thus we can omit the parameter
    return m_tasks->count();
}

QVariant
TaskListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        Task item = m_tasks->itemById(index.row());
        if (item.isNull()) {
            return QVariant();
        }

        return QVariant(item.name());
    }

    return QVariant();
}


Qt::ItemFlags
TaskListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant
TaskListModel::headerData(int , Qt::Orientation, int) const
{
    return QVariant("Tasks");
}

bool
TaskListModel::appendItem(const QString& value)
{
    int position = rowCount();
    beginInsertRows(QModelIndex(), position, position + 1);

    // Find the item before inserting
    Task item = m_tasks->itemByName(value);
    if (!item.isNull()) {
        // If we already know the value we can quit this
        endInsertRows();
        return false;
    }

    m_tasks->addItem(Task(value, QDate::currentDate()));

    endInsertRows();
    return true;
}
