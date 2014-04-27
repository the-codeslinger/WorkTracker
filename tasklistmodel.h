#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include "tasklist.h"

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>

class QDomDocument;

class TaskListModel : public QAbstractListModel
{
public:
    TaskListModel(TaskList* tasks, QObject* parent = nullptr);

    bool appendItem(const QString& value);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

private:
    TaskList* m_tasks;

};

#endif // TASKLISTMODEL_H
