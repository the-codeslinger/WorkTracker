#include "tasklistmodel.h"
#include "../task.h"

#include <QDomElement>
#include <QDomNamedNodeMap>
#include <QDate>
#include <QDebug>

TaskListModel::TaskListModel(QDomDocument* dataSource, QObject* parent)
    : QAbstractListModel(parent)
    , m_dataSource(dataSource)
{
}

int
TaskListModel::rowCount(const QModelIndex&) const
{
    // We don't have child-parent relationships, thus we can omit the parameter
    return Task::count(m_dataSource);
}

QVariant
TaskListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        Task item = Task::get(index.row(), m_dataSource);
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

void
TaskListModel::itemAppended()
{
    int position = rowCount();
    beginInsertRows(QModelIndex(), position, position + 1);
    endInsertRows();
}
