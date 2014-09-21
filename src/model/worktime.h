#ifndef WORKTIME_H
#define WORKTIME_H

#include "xmldata.h"

/*!
 * Implements the DOM node of the work-task that contains the start and stop time.
 */
class WorkTime : public XmlData
{
public:
    /*!
     * Creates an empty instance. Calling `WorkTime::isNull` returns `true`.
     */
    WorkTime();
    
    /*!
     * Creates a new instance. The XML node will not be attached to the DOM until the
     * parent is set.
     */
    WorkTime(const QDomDocument& p_dataSource);
    
    /*!
     * Creates a new instance from the DOM node and the parent od that node.
     */
    WorkTime(const QDomDocument& p_dataSource, const QDomElement& p_node, 
             const QDomNode& p_parent);
    
    /*!
     * Creates a new instance with the start and stop time that is immediately attached to
     * the DOM tree through the parent.
     */
    WorkTime(const QDomDocument& p_dataSource, const QDomNode& p_parent, 
             const QDateTime& p_start, const QDateTime& p_stop);
    
    /*!
     * Copy constructor.
     */
    WorkTime(const WorkTime& p_other);
    
    /*!
     * \return 
     * Returns the start time if it is set. Otherwise the date-time returned is null.
     */
    QDateTime start() const;
    
    /*!
     * Set the start date-time.
     * 
     * \param p_start
     * The new start value. If is is null then the DOM node is cleared.
     */
    void setStart(const QDateTime& p_start);
    
    /*!
     * \return 
     * Returns the stop time if it is set. Otherwise the date-time returned is null.
     */
    QDateTime stop() const;
    
    /*!
     * Set the stop date-time.
     * 
     * \param p_stop
     * The new stop value. If is is null then the DOM node is cleared.
     */
    void setStop(const QDateTime& p_stop);
    
    /*!
     * \return 
     * Returns the total amount of seconds between start and stop.
     */
    qint64 timeInSeconds() const;
    
protected:
    /*!
     * Creates a new DOM node with the given start and stop times. Depending on whether
     * the data-source and parent are set the respective DOM nodes are created.
     * 
     * \param p_parent
     * The node's parent node.
     * 
     * \param p_start
     * The new start value. If is is null then the DOM node is empty.
     * 
     * \param p_stop
     * The new stop value. If is is null then the DOM node is empty.
     */
    void createNode(const QDomNode& p_parent, const QDateTime& p_start, 
                    const QDateTime& p_stop);
};

#endif // WORKTIME_H
