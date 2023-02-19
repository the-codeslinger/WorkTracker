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

#include "completerlineedit.h"

#include <QKeyEvent>
#include <QCompleter>

CompleterLineEdit::CompleterLineEdit(QWidget* parent)
    : QLineEdit{parent}
{ }

CompleterLineEdit::~CompleterLineEdit()
{ }

void 
CompleterLineEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);

    // Always open completer if there's no text in the line edit.
    showCompleterIfNoText();

    // Open completer if there's text in the line edit and the user presses down.
    auto comp = completer();
    if (!text().isEmpty() && Qt::Key_Down == event->key() && nullptr != comp) {
        comp->complete();
    }
}

void 
CompleterLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    QLineEdit::mouseReleaseEvent(event);

    if (Qt::LeftButton == event->button()) {
        showCompleterIfNoText();
    }
}

void 
CompleterLineEdit::showCompleterIfNoText()
{
    auto comp = completer();
    if (text().isEmpty() && nullptr != comp) {
        comp->setCompletionPrefix("");
        comp->complete();
    }
}
