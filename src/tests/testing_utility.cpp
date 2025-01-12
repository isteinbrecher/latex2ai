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
 * \brief Utility functions for testing.
 */


#include "IllustratorSDK.h"

#include "testing_utlity.h"

#include "l2a_constants.h"
#include "l2a_string_functions.h"


/**
 *
 */
ai::UnicodeString L2A::TEST::UTIL::test_string_unicode()
{
    return ai::UnicodeString(
        reinterpret_cast<const ai::UnicodeString::UTF16Char*>(L2A::TEST::UTIL::test_string_unicode_characters));
}

/**
 *
 */
ai::UnicodeString L2A::TEST::UTIL::test_string_unicode_multiline()
{
    return test_string_unicode() + "\n" + test_string_unicode() + "\n" + L2A::UTIL::StringStdToAi(test_string_4_) +
           test_string_unicode() + "\n";
}

/**
 *
 */
std::vector<L2A::TEST::UTIL::TestStringData> L2A::TEST::UTIL::test_strings()
{
    return {{L2A::UTIL::StringStdToAi(test_string_1_), "168fcba7bbad5ac1", "e55339768e9b9d93", "aa1275166c0e08a6"},
        {L2A::UTIL::StringStdToAi(test_string_2_), "f51a4ec38c535aec", "99e5bbb4caec0750", "56133a1f4e042a14"},
        {L2A::UTIL::StringStdToAi(test_string_3_), "a6c5f57342b2591", "e24088e7de7e0764", "5f906d24d8b354b2"},
        {L2A::UTIL::StringStdToAi(test_string_4_), "62c1cd8642ed5be8", "6ff8d5730c8b63fa", "87ec20640dce3c6d"},
        {L2A::UTIL::StringStdToAi(test_string_5_), "6b80f03d7ef29102", "53df0414bbc17175", "53df0414bbc17175"},
        {test_string_unicode(), "c78005aaef48587d", "6ecbced3e51f5f33", "6ecbced3e51f5f33"},
        {test_string_unicode_multiline(), "f3764ff58ed3b329", "c9d48d8e239a32d9", "eeb9b4603c04ab81"}};
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::SetTestName(const ai::UnicodeString& test_name) { test_name_ = test_name; }

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::CompareInt(const int& val1, const int& val2)
{
    test_count_++;
    if (val1 == val2)
        test_count_passed_++;
    else
    {
        ai::UnicodeString error_string = "Integer compare test for: " + test_name_ + " failed!\nExpected \"" +
                                         L2A::UTIL::IntegerToString(val1) + "\" got \"" +
                                         L2A::UTIL::IntegerToString(val2) + "\"";
        sAIUser->MessageAlert(error_string);
    }
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::CompareFloat(const AIReal& val1, const AIReal& val2, const AIReal& eps)
{
    test_count_++;
    if (abs(val1 - val2) < eps)
        test_count_passed_++;
    else
        sAIUser->MessageAlert(ai::UnicodeString("Float compare test failed!"));
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::CompareStr(const ai::UnicodeString& val1, const ai::UnicodeString& val2)
{
    test_count_++;
    if (val1 == val2)
        test_count_passed_++;
    else
    {
        ai::UnicodeString error_string("");
        error_string += "String compare test for: ";
        error_string += test_name_;
        error_string += " failed!\nExpected \"";
        error_string += val1;
        error_string += "\" got \"";
        error_string += val2 + "\"";
        sAIUser->MessageAlert(error_string);
    }
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::CompareRect(const AIRealRect& val1, const AIRealRect& val2)
{
    test_count_++;
    if (abs(val1.bottom - val2.bottom) < L2A::CONSTANTS::eps_pos_ &&
        abs(val1.left - val2.left) < L2A::CONSTANTS::eps_pos_ &&
        abs(val1.right - val2.right) < L2A::CONSTANTS::eps_pos_ && abs(val1.top - val2.top) < L2A::CONSTANTS::eps_pos_)
    {
        test_count_passed_++;
    }
    else
    {
        ai::UnicodeString error_string("");
        error_string += "Rectangle compair failed!\n";
        error_string += "\nval1.bottom = ";
        error_string += ai::UnicodeString(std::to_string(val1.bottom));
        error_string += "\nval2.bottom = ";
        error_string += ai::UnicodeString(std::to_string(val2.bottom));
        error_string += "\n\nval1.left = ";
        error_string += ai::UnicodeString(std::to_string(val1.left));
        error_string += "\nval2.left = ";
        error_string += ai::UnicodeString(std::to_string(val2.left));
        error_string += "\n\nval1.right = ";
        error_string += ai::UnicodeString(std::to_string(val1.right));
        error_string += "\nval2.right = ";
        error_string += ai::UnicodeString(std::to_string(val2.right));
        error_string += "\n\nval1.top = ";
        error_string += ai::UnicodeString(std::to_string(val1.top));
        error_string += "\nval2.top = ";
        error_string += ai::UnicodeString(std::to_string(val2.top));
        sAIUser->MessageAlert(error_string);
    }
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::CompareStringVector(
    const std::vector<ai::UnicodeString>& val1, const std::vector<ai::UnicodeString>& val2)
{
    CompareInt((int)val1.size(), (int)val2.size());
    for (size_t i = 0; i < val1.size(); i++) CompareStr(val1[i], val2[i]);
}

/**
 *
 */
void L2A::TEST::UTIL::UnitTest::PrintTestSummary(const bool print_status)
{
    if (print_status)
    {
        ai::UnicodeString summary_string("");
        summary_string += "Performed ";
        summary_string += L2A::UTIL::IntegerToString(test_count_);
        summary_string += " tests\n";
        summary_string += L2A::UTIL::IntegerToString(test_count_passed_);
        summary_string += " passed\n";
        summary_string += L2A::UTIL::IntegerToString(test_count_ - test_count_passed_);
        summary_string += " failed";
        sAIUser->MessageAlert(summary_string);
    }
}
