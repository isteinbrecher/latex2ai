// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
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

#include "IllustratorSDK.h"
#include "string_functions.h"

#include "../l2a_suites.h"
#include "../l2a_error/l2a_error.h"


/**
 *
 */
ai::UnicodeString L2A::UTIL::IntegerToString(const unsigned int& number, const unsigned int& pad_len)
{
    ai::NumberFormat format;
    ai::UnicodeString number_string;

    ASErr error = sAIStringFormatUtils->IntegerToString(format, number, number_string);
    ai::check_ai_error(error);

    // If string should be padded.
    if (pad_len != 0)
    {
        // Check that the string is not longer than the padding length.
        if (pad_len < number_string.length())
            throw L2A::ERR::Exception(
                ai::UnicodeString("UTIL::IntegerToString\nPaddig length is shorter than integer length!"));

        ai::UnicodeString padding("");
        for (unsigned int i = 0; i < pad_len - number_string.length(); i++) padding.append(ai::UnicodeString("0"));

        padding += number_string;
        return padding;
    }
    else
    {
        return number_string;
    }
}

/**
 *
 */
int L2A::UTIL::StringToInteger(const ai::UnicodeString& string)
{
    int return_value;
    if (!ai::NumberFormat().parseString(string, return_value))
        throw L2A::ERR::Exception(
            ai::UnicodeString("UTIL::StringToInteger\nInteger could not be converted to string!"));
    return return_value;
}

/**
 *
 */
bool L2A::UTIL::StartsWith(
    const ai::UnicodeString& ref_string, const ai::UnicodeString& compare_string, bool caseless_compare)
{
    int compare_value;
    if (caseless_compare)
        compare_value = ref_string.substr(0, compare_string.size()).caseCompare(compare_string);
    else
        compare_value = ref_string.substr(0, compare_string.size()).compare(compare_string);

    if (compare_value == 0)
        return true;
    else
        return false;
}
