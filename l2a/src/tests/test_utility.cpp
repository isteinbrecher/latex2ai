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
 * \brief Utility functions for testing.
 */


#include "IllustratorSDK.h"
#include "test_utlity.h"

#include "utility/string_functions.h"
#include "l2a_constants.h"


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
            L2A::UTIL::IntegerToString(val1) + "\" got \"" + L2A::UTIL::IntegerToString(val2) + "\"";
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
