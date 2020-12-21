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
