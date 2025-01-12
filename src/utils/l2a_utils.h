// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020-2025 Ivo Steinbrecher
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -----------------------------------------------------------------------------

/**
 * \brief Utility functions.
 */

#ifndef UTILS_H_
#define UTILS_H_


#include "l2a_error.h"


namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Convert a key that is in one array to a corresponding key in a second array.
         * @param key_array Array containing the keys.
         * @param value_array Array containing the values.
         * @param key Key that should be converted to a value.
         *
         * @tparam T Type of key.
         * @tparam V Type of value.
         * @tparam n Number of elements.
         */
        template <typename T, typename V, size_t n>
        V KeyToValue(const std::array<T, n>& key_array, const std::array<V, n>& value_array, const T& key)
        {
            for (unsigned int i = 0; i < n; i++)
                if (key_array[i] == key) return value_array[i];
            l2a_error("Key not found in key_array!");
        }
    }  // namespace UTIL
}  // namespace L2A

#endif
