#ifndef WORKDAYLIST_H
#define WORKDAYLIST_H

#include "workday.h"

#include <QDomElement>

class QDomDocument;

class WorkDayList
{
public:
    WorkDayList(QDomDocument* dataSource = nullptr);

    void setDataSource(QDomDocument* dataSource);

    bool addWorkDay(WorkDay wday);
    WorkDay findToday() /* const */;

private:
    QDomDocument* m_dataSource;
    QDomElement   m_days;
};

#endif // WORKDAYLIST_H
