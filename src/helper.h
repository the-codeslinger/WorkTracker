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

#ifndef HELPER_H
#define HELPER_H

#include <functional>
#include <algorithm>

/*! \file
 * Contains useful helper methods that are not necessarily bound to a single class.
 */

/*!
 * Rounds `number` to two decimals in order to show it to the user.
 */
float roundTwoDecimals(float number);

/*!
 * Wrapper for `std::for_each` that iterates over the whole container.
 */
template<typename Container, typename Func>
inline Func forEach(const Container& container, Func func)
{
    return std::for_each(std::begin(container), std::end(container), func);
}

/*!
 * Wrapper for `std::find_if` that iterates over the whole container.
 */
template<typename Container, typename Func>
inline typename Container::const_iterator findIf(const Container& container, Func func)
{
    return std::find_if(std::begin(container), std::end(container), func);
}

/*!
 * Iterates the container until `func` returns `true`. Returns the found element or a 
 * default constructed one.
 */
template<typename Container, typename Func>
inline typename Container::value_type 
firstOrDefault(Container&& container, Func&& func)
{
    for (const auto& item : container) {
        if (func(item)) {
            return item;
        }
    }
    return typename Container::value_type();
}

class QDomNodeList;
class QDomElement;

/*!
 * Iterates a `QDomNodeList` and performs `func` on every item.
 */
void forEachNode(const QDomNodeList& nodes, std::function<void(QDomElement)> func);

/*!
 * Iterates a `QDomNodeList` until `func` returns `true`. The found element is returned 
 * or a null-element.
 */
QDomElement findFirstNode(const QDomNodeList& nodes, 
                          std::function<bool(QDomElement)> func);

#endif // HELPER_H
