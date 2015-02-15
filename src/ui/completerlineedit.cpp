/*
 * Copyright 2015 Robert Lohr
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