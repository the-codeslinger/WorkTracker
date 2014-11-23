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