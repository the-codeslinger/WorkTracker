/*
 * Copyright 2023 Robert Lohr
 *
 * This file is part of WorkTracker.
 *
 * WorkTracker is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * WorkTracker is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with
 * WorkTracker. If not, see <https://www.gnu.org/licenses/>.
 */

#include "workdaydelegate.h"
#include "../../model/workday.h"

#include <QStyleOptionViewItem>
#include <QPainter>
#include <QModelIndex>
#include <QRect>
#include <QStyle>
#include <QApplication>
#include <QLocale>
#include <QDateTime>
#include <QMouseEvent>
#include <QMessageBox>
#include <QIcon>
#include <QLocale>

WorkDayDelegate::WorkDayDelegate(QObject* p_parent)
    : QStyledItemDelegate{p_parent}
{
}

void
WorkDayDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option,
                       const QModelIndex& p_index) const
{
    if (p_index.data().canConvert<WorkDay>()) {
        auto workday = qvariant_cast<WorkDay>(p_index.data());

        auto option = QStyleOptionViewItem{p_option};
        initStyleOption(&option, p_index);

        p_painter->save();

        auto* style = nullptr != p_option.widget ? p_option.widget->style()
                                                 : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &p_option, p_painter,
                           p_option.widget);

        auto cg = p_option.state & QStyle::State_Enabled
                                    ? QPalette::Normal
                                    : QPalette::Disabled;
        if (cg == QPalette::Normal && !(p_option.state & QStyle::State_Active)) {
            cg = QPalette::Inactive;
        }

        p_painter->setPen(p_option.palette.color(cg, QPalette::Text));

        auto base = p_option.rect;
        auto topRect = QRect{base.left() + 3,  base.top(),
                             base.width(),     base.height() / 3};
        auto staRect = QRect{base.left() + 10, base.top() + base.height() / 3,
                             base.width(),     base.height() / 3};
        auto stoRect = QRect{base.left() + 10, base.top() + (base.height() / 3) * 2,
                             base.width(),     base.height() / 3};

        auto localStart = workday.start().toLocalTime();
        auto localEnd   = workday.stop().toLocalTime();

        auto startDay     = localDateString(localStart.date());
        auto totalMinutes = static_cast<int>(workday.totalTime() / 60);

        auto hours   = static_cast<int>(totalMinutes / 60);
        auto minutes = static_cast<int>(totalMinutes % 60);

        // If the dates are different then the end string displays the date in addition
        // to the time
        auto endDateStr = localEnd.time().toString();
        if (localStart.date() != localEnd.date()) {
            endDateStr = QString{"%1 %2"}.arg(
                        localDateString(localEnd.date()),
                        localTimeString(localEnd.time()));
        }

        auto topString = tr("%1 (%2h %3m)").arg(startDay).arg(hours).arg(minutes);
        auto staString = tr("Start: %1").arg(localStart.time().toString());
        auto stoString = tr("End: %1").arg(endDateStr);

        p_painter->drawText(topRect, p_option.displayAlignment, topString);

        auto font = p_painter->font();
        font.setItalic(true);
        p_painter->setFont(font);
        p_painter->drawText(staRect, p_option.displayAlignment, staString);
        p_painter->drawText(stoRect, p_option.displayAlignment, stoString);
        

        // Draw a button on the right side that opens the summary again.
        QStyleOptionButton opt;
        opt.rect = base; // Used in calcButtonRect() to determine the proper top()
        opt.features = QStyleOptionButton::DefaultButton;
        opt.text = tr("Summary");
        opt.state = QStyle::State_Enabled;
        if (buttonMouseOver[option.rect.top()]) {
            opt.state |= QStyle::State_MouseOver;
        }
        if (buttonClicked[option.rect.top()]) {
            opt.state |= QStyle::State_Sunken;
        }
        else {
            opt.state |= QStyle::State_Raised;
        }
        opt.rect = calcButtonRect(option);
        // TODO Decide if icon makes it beautiful or more crowded.
        //opt.icon = 

        style->drawControl(QStyle::CE_PushButton, &opt, p_painter);

        p_painter->restore();
    }
    else {
        QStyledItemDelegate::paint(p_painter, p_option, p_index);
    }
}

QSize
WorkDayDelegate::sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index ) const
{
    auto result = QStyledItemDelegate::sizeHint(p_option, p_index);
    result.setHeight(result.height() * 3);
    return result;
}

bool
WorkDayDelegate::editorEvent(QEvent* event, QAbstractItemModel* model,
                             const QStyleOptionViewItem& option,
                             const QModelIndex& index)
{
    if (QEvent::MouseButtonPress == event->type()) {
        if (isButtonClick(event, option)) {
            buttonClicked[option.rect.top()] = true;
            return true;
        }
    }
    else if (QEvent::MouseButtonRelease == event->type()) {
        if (isButtonClick(event, option)) {
            buttonClicked[option.rect.top()] = false;

            auto data  = qvariant_cast<WorkDay>(index.data());
            auto day   = data.start().toLocalTime().date();
            auto title = tr("Summary for %1").arg(localDateString(day));

            QIcon i(":/icon/Summary.svg");
            QMessageBox b{ QMessageBox::NoIcon, title, data.generateSummary(), QMessageBox::Ok };
            b.setIconPixmap(i.pixmap(QSize(48, 48)));
            b.setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
            b.exec(); // Result is not interesting. There's only one button.

            return true;
        }
    }
    else if (QEvent::MouseMove == event->type()) {
        buttonMouseOver[option.rect.top()] = isMouseOverButton(event, option);
        return true;
    }

    // Everything else, like selecting a row, we delegate to the base class.
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect
WorkDayDelegate::calcButtonRect(const QStyleOptionViewItem &item) const
{
    // Get a rect for how big the rendered text would be. Based on that we can calculate the
    // offset from the top of the row so the button is placed in the center.
    auto textRect = item.fontMetrics.boundingRect(tr("Summary"));
    auto offsetTop = (item.rect.height() - textRect.height()) / 2;

    // Place the button anchored from the right. Add a little extra padding on all sides.
    return QRect{item.rect.right() - textRect.width() - 20, item.rect.top() + offsetTop - 5,
                 textRect.width() + 10,                     textRect.height() + 10};
}


bool
WorkDayDelegate::isMouseOverButton(QEvent* event, const QStyleOptionViewItem& option) const
{
    // By virtue of when this method is called this cast is safe.
    return calcButtonRect(option).contains(static_cast<QMouseEvent*>(event)->pos());
}

bool
WorkDayDelegate::isButtonClick(QEvent* event, const QStyleOptionViewItem& option) const
{
    // By virtue of when this method is called this cast is safe.
    return static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton 
           && isMouseOverButton(event, option);
}

QString
WorkDayDelegate::localDateString(const QDate& date) const
{
    return QLocale().toString(date, QLocale::LongFormat);
}

QString
WorkDayDelegate::localTimeString(const QTime& time) const
{
    return QLocale().toString(time, QLocale::LongFormat);
}
