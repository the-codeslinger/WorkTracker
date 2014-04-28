#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QDomElement>
#include <QList>

#include <functional>

class QDomNode;
class QDomDocument;

class Task
{
public:
    static int invalidId;

    Task();
    Task(QString name, QDate lastUsed);
    Task(int id, QString name, QDate lastUsed);

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString& name);

    QDate lastUsed() const;
    void setLastUsed(const QDate& lastUsed);

    bool isNull() const;

    QDomElement createElement(int id, QDomDocument* dataSource) const;

    // Used to fetch from the data source
    static Task fromDomNode(QDomNode* node);
    static Task get(int id, QDomDocument* dataSource);
    static Task findByName(QString name, QDomDocument* dataSource);
    static QList<Task> list(QDomDocument* dataSource);
    static int count(QDomDocument* dataSource);

private:
    int     m_id;
    QString m_name;
    QDate   m_lastUsed;

    static int idFromAttr(QDomNode* attr);
    static QString nameFromAttr(QDomNode* attr);
    static QDate lastUsedFromAttr(QDomNode* attr);

    static Task findTask(QDomDocument* dataSource, std::function<bool(Task)> predicate);

    static QDomNodeList getTaskNodes(QDomDocument* dataSource);
};

#endif // TASK_H
