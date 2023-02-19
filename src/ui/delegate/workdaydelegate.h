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

#ifndef WORKDAYDELEGATE_H
#define WORKDAYDELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>

/*!
 * Custom renderer for a list of `WorkDay` items. Paints a three-line layout with the
 * day's date in the first line and individual lines for start and stop times.
 */
class WorkDayDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    // Use the top position of a row as identifier for clicked-states. If en extra enum were
    // introduced one list would be enough.
    QMap<int, bool> buttonMouseOver;
    QMap<int, bool> buttonClicked;
public:
    /*!
     * Create a new instance.
     *
     * \param p_parent
     * The parent of the delegate.
     */
    explicit WorkDayDelegate(QObject* p_parent = nullptr);

    /*!
     * Paints the custom item.
     */
    void paint(QPainter* p_painter, const QStyleOptionViewItem& p_option,
               const QModelIndex& p_index) const;

    /*!
     * \return
     * Returns the height for three lines.
     */
    QSize sizeHint(const QStyleOptionViewItem& p_option,
                   const QModelIndex& p_index) const;

    /*!
     * \return
     * `true` if the event was handled or `false` if not.
     */
    bool editorEvent(QEvent* event, QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index);

private:
    // Cannot be static because access to the `tr()` function is required.
    QRect calcButtonRect(const QStyleOptionViewItem &options) const;
    bool isMouseOverButton(QEvent* event, const QStyleOptionViewItem& option) const;
    bool isButtonClick(QEvent* event, const QStyleOptionViewItem& option) const;
    QString localDateString(const QDate& date) const;
    QString localTimeString(const QTime& time) const;
};

#endif // WORKDAYDELEGATE_H
