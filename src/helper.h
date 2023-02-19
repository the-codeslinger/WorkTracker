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

#ifndef HELPER_H
#define HELPER_H

#include <functional>
#include <algorithm>
#include <type_traits>

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
template<typename Container>
inline typename Container::value_type
firstOrDefault(const Container& container,
               std::function<bool(typename Container::value_type)> func)
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
