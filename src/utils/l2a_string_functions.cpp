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

#include "IllustratorSDK.h"
#include "l2a_string_functions.h"

#include <iomanip>

#include "l2a_suites.h"
#include "l2a_error.h"


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
        if (pad_len < number_string.length()) l2a_error("Paddig length is shorter than integer length!");

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
template <typename int_type>
std::string L2A::UTIL::IntegerToHexString(int_type value)
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(int_type) * 2) << std::hex << value;
    return stream.str();
}

//! Explicit template instantiation.
template std::string L2A::UTIL::IntegerToHexString<int>(int);

/**
 *
 */
int L2A::UTIL::StringToInteger(const ai::UnicodeString& string)
{
    int return_value;
    if (!ai::NumberFormat().parseString(string, return_value)) l2a_error("Integer could not be converted to string!");
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

/**
 *
 */
void L2A::UTIL::StringReplaceAll(
    ai::UnicodeString& string, const ai::UnicodeString& search_string, const ai::UnicodeString& replace_string)
{
    // https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
    if (search_string.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = string.find(search_string, start_pos)) != std::string::npos)
    {
        string = string.replace(start_pos, search_string.length(), replace_string);
        start_pos += replace_string.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

/**
 *
 */
std::vector<ai::UnicodeString> L2A::UTIL::SplitString(
    const ai::UnicodeString& string, const ai::UnicodeString& split_string)
{
    if (split_string.empty()) l2a_error("Split string can not be empty");

    std::vector<ai::UnicodeString> split_vector;
    size_t split_pos = 0;
    size_t start_pos = 0;
    while ((split_pos = string.find(split_string, start_pos)) < string.size())
    {
        split_vector.push_back(string.substr(start_pos, split_pos - start_pos));
        start_pos = split_pos + split_string.length();
    }
    split_vector.push_back(string.substr(start_pos, string.size() - start_pos));
    return split_vector;
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::StringHash(const ai::UnicodeString& string)
{
    std::size_t hash_int = std::hash<std::string>{}(string.as_UTF8());
    std::stringstream stream;
    stream << std::hex << hash_int;
    std::string hash(stream.str());
    return ai::UnicodeString(hash);
}
