#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>

class QDomDocument;

class TaskListModel : public QAbstractListModel
{
public:
    TaskListModel(QDomDocument* dataSource, QObject* parent = nullptr);

    void itemAppended();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

private:
    QDomDocument* m_dataSource;

};

#endif // TASKLISTMODEL_H
