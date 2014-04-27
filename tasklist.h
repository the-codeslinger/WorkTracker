#ifndef TASKLIST_H
#define TASKLIST_H

#include "task.h"

#include <QString>
#include <QDomElement>
#include <functional>

class QDomDocument;

class TaskList
{
public:
    TaskList(QDomDocument* dataSource = nullptr);

    void setDataSource(QDomDocument* dataSource);

    int count() const;

    Task itemById(int id) const;
    Task itemByName(QString name) const;

    bool addItem(Task item);

private:
    QDomDocument* m_dataSource;
    QDomElement   m_tasks;

    Task findTask(std::function<bool(Task)> predicate) const;
};

#endif // TASKLIST_H
