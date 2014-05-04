#include "xmldata.h"

#include <QDebug>
#include <QVariant>
#include <QDomDocument>

XmlData::XmlData(QDomDocument* dataSource)
    : m_dataSource(dataSource)
{
}

XmlData::XmlData(QDomDocument* dataSource, QDomElement node)
    : m_dataSource(dataSource)
    , m_node(node)
{
}

XmlData::XmlData(const XmlData& other)
    : m_dataSource(other.m_dataSource)
    , m_node(other.m_node)
{
}

XmlData::XmlData(XmlData&& temp)
    : m_dataSource(temp.m_dataSource)
    , m_node(temp.m_node)
{
    temp.m_dataSource = nullptr;
}

void
XmlData::setDataSource(QDomDocument* dataSource)
{
    m_dataSource = dataSource;
}

QDomNode
XmlData::node() const
{
    return m_node;
}

bool
XmlData::isNull() const
{
    return m_node.isNull();
}

void
XmlData::clear()
{
    m_node.clear();
}

void
XmlData::addAttribute(QString name, QString value)
{
    if (m_node.isNull()) {
        qDebug() << "node is null";
    }

    if (!value.isEmpty()) {
        QDomAttr attr = findAttribute(name).toAttr();
        if (attr.isNull()) {
            attr = m_dataSource->createAttribute(name);
            m_node.setAttributeNode(attr);
        }
        attr.setValue(value);
    }
}

QDomNode
XmlData::findAttribute(QString name) const
{
    QDomNamedNodeMap attributes = m_node.attributes();
    return attributes.namedItem(name);
}

QVariant
XmlData::attributeValue(QString name) const
{
    QDomNode attrNode = findAttribute(name);
    if (attrNode.isNull() || !attrNode.isAttr()) {
        return QVariant();
    }

    QDomAttr attr = attrNode.toAttr();
    return QVariant(attr.value());
}
