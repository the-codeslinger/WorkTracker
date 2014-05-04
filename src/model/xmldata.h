#ifndef XMLDATA_H
#define XMLDATA_H

#include <QString>
#include <QDomElement>

class QVariant;
class QDomNode;
class QDomDocument;

class XmlData
{
public:
    void setDataSource(QDomDocument* dataSource);
    QDomNode node() const;

    bool isNull() const;
    void clear();

protected:
    XmlData(QDomDocument* dataSource);
    XmlData(QDomDocument* dataSource, QDomElement node);
    XmlData(const XmlData& other);
    XmlData(XmlData&& temp);

    void addAttribute(QString name, QString value);
    QDomNode findAttribute(QString name) const;
    QVariant attributeValue(QString name) const;

    QDomDocument* m_dataSource;
    QDomElement   m_node;
};

#endif // XMLDATA_H
