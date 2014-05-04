#ifndef TASK_H
#define TASK_H

#include "xmldata.h"

#include <QString>
#include <QDate>
#include <QDomNode>
#include <QList>
#include <QVariant>

#include <functional>

class QDomDocument;
class QDomAttr;

class Task : public XmlData
{
public:
    static int invalidId;

    Task();
    Task(QDomDocument* dataSource);
    Task(QDomDocument* dataSource, QDomElement node);
    Task(QDomDocument* dataSource, QString name, QDate lastUsed);
    Task(QDomDocument* dataSource, int id, QString name, QDate lastUsed);
    Task(const Task& other);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QDate lastUsed() const;
    void setLastUsed(const QDate& lastUsed);

    // Used to fetch from the data source
    static Task fromDomNode(QDomElement node, QDomDocument* dataSource);
    static Task get(int id, QDomDocument* dataSource);
    static Task findByName(QString name, QDomDocument* dataSource);
    static QList<Task> list(QDomDocument* dataSource);
    static int count(QDomDocument* dataSource);

private:

    void createNode(int id, QString name, QDate lastUsed);
    QVariant attributeValue(QString name) const;

    static Task findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate);

    static QDomNodeList getTaskNodes(QDomDocument* dataSource);
};

#endif // TASK_H
