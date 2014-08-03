#ifndef WORKTASKMODEL_H
#define WORKTASKMODEL_H

#include "../worktask.h"

#include <QAbstractItemModel>

/*!
 * Table model to edit the start and stop times of `WorkTask` items.
 */
class WorkTaskModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    /*!
     * Create a new instance of the model.
     *
     * \param p_parent
     * The parent of the model.
     */
    explicit WorkTaskModel(QObject* p_parent = nullptr);

    /*!
     * Set the model's new work tasks. This replaces the previous work tasks.
     */
    void setWorkTasks(const QList<WorkTask>& p_worktasks);

    /*!
     * \return
     * Returns the number of available work task times.
     */
    int rowCount(const QModelIndex& p_parent) const;

    /*!
     * \return
     * Always returns the number 2. There are always two columns in the model, the start
     * and the stop time.
     */
    int columnCount(const QModelIndex& p_parent) const;

    /*!
     * \return
     * Returns "Start" for the first column and "Stop" for the second column in horizontal
     * orientation. No valid values are returned for vertical orientation. The role is
     * ignored, header are always the same for every role.
     */
    QVariant headerData(int p_section, Qt::Orientation p_orientation,
                        int p_role = Qt::DisplayRole) const;

    /*!
     * \return
     * Returns the start or stop time for the work task at the given position.
     */
    QVariant data(const QModelIndex& p_index, int p_role = Qt::DisplayRole) const;

    /*!
     * Set a new `QDateTime` for a specific work task.
     */
    bool setData(const QModelIndex& p_index, const QVariant& p_value,
                 int p_role = Qt::EditRole);

    /*!
     * \return
     * Returns `QAbstractItemModel::flags() | Qt::ItemIsEditable`.
     */
    Qt::ItemFlags flags(const QModelIndex& p_index) const;

    /*!
     * \see QAbstractItemModel::index(int, int, const QModelIndex&) const
     */
    QModelIndex index(int p_row, int p_column,
                      const QModelIndex& p_parent = QModelIndex()) const;

    /*!
     * \return
     * Always returns an invalid index. None of the items have a parent.
     */
    QModelIndex parent(const QModelIndex& p_child) const;

private:
    QList<WorkTask> m_worktasks;

};

#endif // WORKTASKMODEL_H
