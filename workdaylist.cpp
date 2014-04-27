#include "workdaylist.h"

WorkDayList::WorkDayList(QDomDocument* dataSource)
{
    setDataSource(dataSource);
}

void
WorkDayList::setDataSource(QDomDocument* dataSource)
{
    if (nullptr != dataSource) {
        m_dataSource = dataSource;
        QDomElement root = m_dataSource->documentElement();
        m_days = root.firstChildElement("workdays");
    }
}

bool
WorkDayList::addWorkDay(WorkDay wday)
{
    QDomElement elem = wday.createElement(m_dataSource);
    if (!elem.isNull()) {
        QDomNode result = m_days.appendChild(elem);
        return !result.isNull();
    }

    return false;
}
