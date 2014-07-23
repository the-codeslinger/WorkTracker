#ifndef WORKDAYDELEGATE_H
#define WORKDAYDELEGATE_H

#include <QStyledItemDelegate>

class WorkDayDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit WorkDayDelegate(QObject* p_parent = nullptr);

    void paint(QPainter* p_painter, const QStyleOptionViewItem& p_option,
               const QModelIndex& p_index) const;

    QSize sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const;

signals:

public slots:

};

#endif // WORKDAYDELEGATE_H
