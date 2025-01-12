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
         * \brief Convert a std string to an AI string
         */
        ai::UnicodeString StringStdToAi(const std::string& string_std);

        /**
         * \brief Convert an AI string to a std string
         */
        std::string StringAiToStd(const ai::UnicodeString& string_ai);

        /**
         * \brief Convert an AI string to a std wstring (needed for Windows)
         */
#ifdef WIN_ENV
        std::wstring StringAiToStdW(const ai::UnicodeString& string_ai);
#endif

        /**
         * \brief Convert an integer to an ai::UnicodeString.
         * @param number Number to be converted to a string.
         * @param pad_len Padded length of the string. If this is 0, then the string will not be padded.
         */
        ai::UnicodeString IntegerToString(const unsigned int& number, const unsigned int& pad_len = 0);

        /**
         * \brief Convert an integer to an std::string representing its hex value.
         * @value The integer value to convert.
         * @return The integer in hex formating.
         */
        template <typename int_type>
        std::string IntegerToHexString(int_type value);

        /**
         * \brief Convert an ai::UnicodeString to an integer.
         */
        int StringToInteger(const ai::UnicodeString& string);

        /**
         * \brief Check if a string starts with another string.
         * @param ref_string The string that should be checked.
         * @param compare_string The string that should be at the beginning.
         * @param caseless_compare If this is true, capitalization matters in the comparison.
         */
        bool StartsWith(const ai::UnicodeString& ref_string, const ai::UnicodeString& compare_string,
            bool caseless_compare = false);

        /**
         * \brief Replace parts of a string.
         * @param string String where parts will be replaced.
         * @param search_string Substring that will be replaced.
         * @param replace_string Substring that will be inserted into the replaced parts.
         */
        void StringReplaceAll(
            ai::UnicodeString& string, const ai::UnicodeString& search_string, const ai::UnicodeString& replace_string);

        /**
         * \brief Split string at occurences of split_string.
         */
        std::vector<ai::UnicodeString> SplitString(
            const ai::UnicodeString& string, const ai::UnicodeString& split_string);

        /**
         * \brief Calculate a hash from a string.
         */
        ai::UnicodeString StringHash(const ai::UnicodeString& string);
    }  // namespace UTIL
}  // namespace L2A


namespace ai
{
#if kPluginInterfaceVersion >= 0x22000001
    //! This function is not required in newer SDK versions.
#else
    /**
     * \brief Overload the + operator for strings. Now existing strings can be simply added.
     */
    inline UnicodeString operator+(const UnicodeString& lhs, const UnicodeString& rhs)
    {
        ai::UnicodeString return_string(lhs);
        return_string += rhs;
        return return_string;
    }
#endif

    /**
     * \brief Overload the + operator for a string and a char*. Now existing strings can be simply added.
     */
    inline UnicodeString operator+(const UnicodeString& lhs, const std::string& rhs)
    {
        ai::UnicodeString return_string(lhs);
        return_string += L2A::UTIL::StringStdToAi(rhs);
        return return_string;
    }

    /**
     * \brief Overload the + operator for a string and a char*.
     */
    inline UnicodeString operator+(const std::string& lhs, const UnicodeString& rhs)
    {
        ai::UnicodeString return_string = L2A::UTIL::StringStdToAi(lhs);
        return_string += ai::UnicodeString(rhs);
        return return_string;
    }


    /**
     * \brief Overload the += operator for a string and a char*.
     */
    inline UnicodeString& operator+=(UnicodeString& lhs, const std::string& rhs)
    {
        lhs += L2A::UTIL::StringStdToAi(rhs);
        return lhs;
    }

    /**
     * \brief Overload the == operator for a string and a char*.
     */
    inline bool operator==(const UnicodeString& lhs, const std::string& rhs)
    {
        return lhs == L2A::UTIL::StringStdToAi(rhs);
    }

    /**
     * \brief Overload the == operator for a char* and a string.
     */
    inline bool operator==(const std::string& lhs, const UnicodeString& rhs)
    {
        return rhs == L2A::UTIL::StringStdToAi(lhs);
    }

}  // namespace ai


#endif
