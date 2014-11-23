#ifndef TASKDELEGATE_H
#define TASKDELEGATE_H

#include <QStyledItemDelegate>
#include <QDomDocument>

class TaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TaskDelegate(const QDomDocument& p_dataSource, QObject* p_parent = nullptr);
    
    QWidget* createEditor(QWidget* p_parent, const QStyleOptionViewItem& p_option, 
                          const QModelIndex& p_index) const;
    
private:
    QDomDocument m_dataSource;
};

#endif // TASKDELEGATE_H
