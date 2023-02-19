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

#include "helper.h"

#include <QDomNodeList>
#include <QDomElement>

float
roundTwoDecimals(float number)
{
    return static_cast<float>(static_cast<int>(number * 100 + 0.5) / 100.0);
}

void 
forEachNode(const QDomNodeList& nodes, std::function<void(QDomElement)> func)
{
    for (int c = 0; c < nodes.length(); c++) {
        auto node = nodes.item(c);
        if (node.isElement()) {
            func(node.toElement());
        }
    }
}

QDomElement 
findFirstNode(const QDomNodeList& nodes, std::function<bool(QDomElement)> func)
{
    for (int c = 0; c < nodes.length(); c++) {
        auto node = nodes.item(c);
        if (node.isElement()) {
            auto element = node.toElement();
            if (func(element)) {
                return element;
            }
        }
    }
    return QDomElement();
}
