#include "taskdelegate.h"
#include "../model/tasklistmodel.h"

#include <QCompleter>
#include <QLineEdit>

TaskDelegate::TaskDelegate(const QDomDocument& p_dataSource, QObject* p_parent)
    : QStyledItemDelegate(p_parent)
    , m_dataSource(p_dataSource)
{
}

QWidget* 
TaskDelegate::createEditor(QWidget* p_parent, const QStyleOptionViewItem& /* p_option */, 
                           const QModelIndex& p_index) const
{
    if (!p_index.isValid()) {
        return nullptr;
    }
    
    QLineEdit*  lineEdit  = new QLineEdit(p_parent);
    QCompleter* completer = new QCompleter(lineEdit);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setModel(new TaskListModel(m_dataSource, completer));
    lineEdit->setCompleter(completer);
    return lineEdit;
}
