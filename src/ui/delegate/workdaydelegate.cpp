/*
 * Copyright 2014 Robert Lohr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "workdaydelegate.h"
#include "../../model/workday.h"

#include <QStyleOptionViewItemV4>
#include <QPainter>
#include <QModelIndex>
#include <QRect>
#include <QStyle>
#include <QApplication>
#include <QLocale>
#include <QDateTime>

WorkDayDelegate::WorkDayDelegate(QObject* p_parent)
    : QStyledItemDelegate(p_parent)
{
}

void
WorkDayDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option,
                       const QModelIndex& p_index) const
{
    if (p_index.data().canConvert<WorkDay>()) {
        WorkDay workday = qvariant_cast<WorkDay>(p_index.data());

        QStyleOptionViewItemV4 option = p_option;
        initStyleOption(&option, p_index);

        p_painter->save();

        QStyle* style = nullptr != p_option.widget ? p_option.widget->style()
                                                   : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &p_option, p_painter,
                           p_option.widget);

        QPalette::ColorGroup cg = p_option.state & QStyle::State_Enabled
                                    ? QPalette::Normal
                                    : QPalette::Disabled;
        if (cg == QPalette::Normal && !(p_option.state & QStyle::State_Active)) {
            cg = QPalette::Inactive;
        }

        p_painter->setPen(p_option.palette.color(cg, QPalette::Text));

        QRect base = p_option.rect;
        QRect topRect(base.left() + 3,  base.top(),
                      base.width(),     base.height() / 3);
        QRect staRect(base.left() + 10, base.top() + base.height() / 3,
                      base.width(),     base.height() / 3);
        QRect stoRect(base.left() + 10, base.top() + (base.height() / 3) * 2,
                      base.width(),     base.height() / 3);

        QDateTime localStart = workday.start().toLocalTime();
        QDateTime localEnd   = workday.stop().toLocalTime();

        QString startDay = localStart.date().toString(Qt::DefaultLocaleLongDate);
        int totalMinutes = static_cast<int>(workday.totalTime() / 60);

        int hours   = static_cast<int>(totalMinutes / 60);
        int minutes = static_cast<int>(totalMinutes % 60);

        // If the dates are different then the end string displays the date in addition
        // to the time
        QString endDateStr = localEnd.time().toString();
        if (localStart.date() != localEnd.date()) {
            endDateStr = QString("%1 %2")
                            .arg(localEnd.date().toString(Qt::DefaultLocaleLongDate))
                            .arg(localEnd.time().toString(Qt::DefaultLocaleShortDate));
        }

        QString topString = tr("%1 (%2h %3m)").arg(startDay).arg(hours).arg(minutes);
        QString staString = tr("Start: %1").arg(localStart.time().toString());
        QString stoString = tr("End: %1").arg(endDateStr);

        p_painter->drawText(topRect, p_option.displayAlignment, topString);

        QFont font = p_painter->font();
        font.setItalic(true);
        p_painter->setFont(font);
        p_painter->drawText(staRect, p_option.displayAlignment, staString);
        p_painter->drawText(stoRect, p_option.displayAlignment, stoString);

        p_painter->restore();
    }
    else {
        QStyledItemDelegate::paint(p_painter, p_option, p_index);
    }
}

QSize
WorkDayDelegate::sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index ) const
{
    QSize result = QStyledItemDelegate::sizeHint(p_option, p_index);
    result.setHeight(result.height() * 3);
    return result;
}
