#include "worktaskmodel.h"

WorkTaskModel::WorkTaskModel(QObject* p_parent)
    : QAbstractItemModel(p_parent)
{
}

void
WorkTaskModel::setWorkTasks(const QList<WorkTask>& p_worktasks)
{
    beginResetModel();
    m_worktasks = p_worktasks;
    endResetModel();
}

int
WorkTaskModel::rowCount(const QModelIndex& /* ignored */) const
{
    return m_worktasks.size();
}

int
WorkTaskModel::columnCount(const QModelIndex& /* ignored */) const
{
    return 2;
}

QVariant
WorkTaskModel::headerData(int p_section, Qt::Orientation p_orientation,
                          int p_role) const
{
    // Vertical orientation or an index out of bounds for an "array of size 2" return
    // nothing (only two columns supported)
    if (Qt::DisplayRole != p_role || Qt::Vertical == p_orientation || 2 <= p_section) {
        return QVariant();
    }

    if (0 == p_section) {
        return tr("Start");
    }

    if (1 == p_section) {
        return tr("Stop");
    }

    // Practically impossible to get here
    return QVariant();
}

QVariant
WorkTaskModel::data(const QModelIndex& p_index, int p_role) const
{
    if (!p_index.isValid() || p_index.row() >= m_worktasks.count()
            || p_index.column() >= 2) {
        return QVariant();
    }

    if (Qt::DisplayRole == p_role || Qt::EditRole == p_role) {
        WorkTask wt = m_worktasks.at(p_index.row());
        if (0 == p_index.column()) {
            return wt.start().toLocalTime();
        }
        else {
            return wt.stop().toLocalTime();
        }
    }

    return QVariant();
}

bool
WorkTaskModel::setData(const QModelIndex& p_index, const QVariant& p_value, int p_role)
{
    if (!p_index.isValid() || p_index.row() >= m_worktasks.count()
            || p_index.column() >= 2) {
        return false;
    }

    if (Qt::EditRole == p_role && p_value.canConvert<QDateTime>()) {
        WorkTask  wt = m_worktasks.at(p_index.row());
        QDateTime dt = qvariant_cast<QDateTime>(p_value);

        if (0 == p_index.column()) {
            wt.setStart(dt.toUTC());
        }
        else {
            wt.setStop(dt.toUTC());
        }

        return true;
        emit dataChanged(p_index, p_index, { Qt::EditRole });
    }

    return false;
}

Qt::ItemFlags
WorkTaskModel::flags(const QModelIndex& p_index) const
{
    return QAbstractItemModel::flags(p_index) | Qt::ItemIsEditable;
}

QModelIndex
WorkTaskModel::index(int p_row, int p_column, const QModelIndex& /* ignored */) const
{
    if (p_row >= m_worktasks.count() || p_column >= 2) {
        return QModelIndex();
    }

    return this->createIndex(p_row, p_column);
}

QModelIndex
WorkTaskModel::parent(const QModelIndex& /* ignored */) const
{
    return QModelIndex();
}
