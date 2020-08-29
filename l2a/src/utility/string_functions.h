// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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
 * \brief Utility functions for strings.
 */

#ifndef UTIL_STRING_H_
#define UTIL_STRING_H_


#include "IllustratorSDK.h"


namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Convert an integer to an ai::UnicodeString. Optional padding is possible.
         */
        ai::UnicodeString IntegerToString(const unsigned int& number, const unsigned int& pad_len = 0);

        /**
         * \brief Convert an ai::UnicodeString to an integer.
         */
        int StringToInteger(const ai::UnicodeString& string);

        /**
         * \brief Check if a string starts with another string.
         * @param caseless_compare If this is true, capitalization matters in the comparison.
         */
        bool StartsWith(const ai::UnicodeString& ref_string, const ai::UnicodeString& compare_string,
            bool caseless_compare = false);
    }  // namespace UTIL
}  // namespace L2A


namespace ai
{
    /**
     * \brief Overload the + operator for strings. Now existing strings can be simply added.
     */
    inline UnicodeString operator+(const UnicodeString& lhs, const UnicodeString& rhs)
    {
        ai::UnicodeString return_string(lhs);
        return_string += rhs;
        return return_string;
    }

    /**
     * \brief Overload the + operator for a string and a char*. Now existing strings can be simply added.
     */
    inline UnicodeString operator+(const UnicodeString& lhs, const char* rhs)
    {
        ai::UnicodeString return_string(lhs);
        return_string += ai::UnicodeString(rhs);
        return return_string;
    }

    /**
     * \brief Overload the + operator for a string and a char*.
     */
    inline UnicodeString operator+(const char* lhs, const UnicodeString& rhs)
    {
        ai::UnicodeString return_string(lhs);
        return_string += ai::UnicodeString(rhs);
        return return_string;
    }


    /**
     * \brief Overload the += operator for a string and a char*.
     */
    inline UnicodeString& operator+=(UnicodeString& lhs, char* rhs)
    {
        lhs += ai::UnicodeString(rhs);
        return lhs;
    }

    /**
     * \brief Overload the == operator for a string and a char*.
     */
    inline bool operator==(const UnicodeString& lhs, const char* rhs) { return lhs == ai::UnicodeString(rhs); }

    /**
     * \brief Overload the == operator for a char* and a string.
     */
    inline bool operator==(const char* lhs, const UnicodeString& rhs) { return rhs == ai::UnicodeString(lhs); }

}  // namespace ai


#endif
