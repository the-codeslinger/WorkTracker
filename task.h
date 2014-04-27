#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QDomElement>

class QDomNode;
class QDomDocument;

class Task
{
public:
    Task();
    Task(QString name, QDate lastUsed);

    QString name() const;
    void setName(const QString& name);

    QDate lastUsed() const;
    void setLastUsed(const QDate& lastUsed);

    bool isNull() const;

    static int invalidId;

    int id() const;
    void setId(int id);

private:
    int     m_id;
    QString m_name;
    QDate   m_lastUsed;

    friend class TaskList;

    Task(int id, QString name, QDate lastUsed);

    QDomElement createElement(int id, QDomDocument* dataSource);
    static Task fromDomNode(QDomNode* node);

    static int idFromAttr(QDomNode* attr);
    static QString nameFromAttr(QDomNode* attr);
    static QDate lastUsedFromAttr(QDomNode* attr);
};

#endif // TASK_H
