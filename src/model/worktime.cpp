#include "worktime.h"

#include <QDateTime>

WorkTime::WorkTime()
    : XmlData()
{
}

WorkTime::WorkTime(const QDomDocument& p_dataSource)
    : XmlData(p_dataSource)
{
    createNode(QDomNode(), QDateTime(), QDateTime());
}

WorkTime::WorkTime(const QDomDocument& p_dataSource, const QDomElement& p_node, 
                   const QDomNode& p_parent)
    : XmlData(p_dataSource, p_node, p_parent)
{
}

WorkTime::WorkTime(const QDomDocument& p_dataSource, const QDomNode& p_parent, 
                   const QDateTime& p_start, const QDateTime& p_stop)
    : XmlData(p_dataSource)
{
    createNode(p_parent, p_start, p_stop);
}

WorkTime::WorkTime(const WorkTime& p_other)
    : XmlData(p_other)
{
}

QDateTime 
WorkTime::start() const
{
    return attributeDateTime("start");
}

void 
WorkTime::setStart(const QDateTime& p_start)
{
    setAttribute("start", p_start);
}

QDateTime 
WorkTime::stop() const
{
    return attributeDateTime("stop");
}

void 
WorkTime::setStop(const QDateTime& p_stop)
{
    setAttribute("stop", p_stop);
}

qint64
WorkTime::timeInSeconds() const
{
    QDateTime startTime = start();
    QDateTime stopTime  = stop();
    
    return startTime.secsTo(stopTime);
}

void 
WorkTime::createNode(const QDomNode& p_parent, const QDateTime& p_start, 
                     const QDateTime& p_stop)
{
    if (m_dataSource.isNull()) {
        return;
    }
    
    m_parent = p_parent;
    m_node   = m_dataSource.createElement("time");
    
    setAttribute("start", p_start);
    setAttribute("stop",  p_stop);
    
    if (!m_parent.isNull()) {
        m_parent.appendChild(m_node);
    }
}
