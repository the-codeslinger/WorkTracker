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

WorkDay
WorkDayList::findToday() /* const */
{
    QDomNodeList children = m_days.childNodes();

    int count = children.count();
    for (int c = 0; c < count; c++) {
        QDomNode day = children.item(c);

        QDomNamedNodeMap attributes = day.attributes();
        QDomAttr dateAttr = attributes.namedItem("date").toAttr();
        if (!dateAttr.isNull()) {
            QDate date  = QDate::fromString(dateAttr.value(), Qt::ISODate);
            QDate today = QDate::currentDate();

            if (date == today) {
                m_days.removeChild(day); // Hack. Future versions should be able to merge
                                         // existing days with new in addWorkDay(). This
                                         // shall prevent duplicate days when closing the
                                         // application.
                return WorkDay::fromDomNode(&day, m_dataSource);
            }
        }
    }

    return WorkDay();
}
