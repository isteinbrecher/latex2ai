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
 * \brief Test the string functions.
 */


#include "IllustratorSDK.h"

#include "test_string_functions.h"

#include "testing_utlity.h"

#include "l2a_string_functions.h"

/**
 *
 */
void TestReferenceStringsAndStringConversion(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test that the hashes of the test strings are as expected
    for (const auto& test_string_data : L2A::TEST::UTIL::test_strings())
    {
        ut.CompareStr(
            L2A::UTIL::StringHash(L2A::UTIL::StringStdToAi(L2A::UTIL::StringAiToStd(test_string_data.string_))),
            ai::UnicodeString(test_string_data.string_hash_));
    }
}

/**
 *
 */
void TestConvertIntegerToString(L2A::TEST::UTIL::UnitTest& ut)
{
    // Convert integer to string
    ai::UnicodeString int_to_string = L2A::UTIL::IntegerToString(1234567890);
    ut.CompareStr(int_to_string, ai::UnicodeString("1234567890"));

    // Convert integer to string padded
    int_to_string = L2A::UTIL::IntegerToString(1234567890, 15);
    ut.CompareStr(int_to_string, ai::UnicodeString("000001234567890"));

    // Convert string to integer
    int string_to_int = L2A::UTIL::StringToInteger(ai::UnicodeString("1234567890"));
    ut.CompareInt(string_to_int, 1234567890);
    string_to_int = L2A::UTIL::StringToInteger(ai::UnicodeString("000001234567890"));
    ut.CompareInt(string_to_int, 1234567890);
}


/**
 *
 */
void TestOperatorOverloads(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test the overloaded operators
    ai::UnicodeString old_operator("value1");
    old_operator += ai::UnicodeString("value2");
    old_operator += ai::UnicodeString("value3");
    old_operator += ai::UnicodeString("value4");
    old_operator += "value5";
    old_operator += ai::UnicodeString("value6");

    ai::UnicodeString new_operator = ai::UnicodeString("value2") + ai::UnicodeString("value3");
    new_operator = "value1" + new_operator;
    new_operator = new_operator + "value4";
    new_operator = new_operator + "value5" + ai::UnicodeString("value6");

    ut.CompareStr(old_operator, new_operator);
}

/**
 *
 */
void TestStartsWith(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test the starts with functions
    ai::UnicodeString long_name("starTName");
    ai::UnicodeString start_name("starT");
    ai::UnicodeString start_name_case("start");
    ai::UnicodeString start_name_wrong("stat");

    ut.CompareInt(1, L2A::UTIL::StartsWith(long_name, start_name));
    ut.CompareInt(0, L2A::UTIL::StartsWith(long_name, start_name_case));
    ut.CompareInt(1, L2A::UTIL::StartsWith(long_name, start_name_case, true));
    ut.CompareInt(0, L2A::UTIL::StartsWith(long_name, start_name_wrong));
}

/**
 *
 */
void TestReplace(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test the replace functions
    ai::UnicodeString full_string("hello $name with $other $other $name and line breaks \n\n\r\n\r\n\n just like that");
    L2A::UTIL::StringReplaceAll(full_string, ai::UnicodeString("$name"), ai::UnicodeString("Full Name"));
    L2A::UTIL::StringReplaceAll(full_string, ai::UnicodeString("$other"), ai::UnicodeString("s$other and more"));
    L2A::UTIL::StringReplaceAll(full_string, ai::UnicodeString("\r\n"), ai::UnicodeString("\n"));
    ut.CompareStr(full_string,
        ai::UnicodeString("hello Full Name with s$other and more s$other and more Full Name and line breaks \n\n\n\n\n "
                          "just like that"));
}

/**
 *
 */
void TestSplit(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test the split function
    ai::UnicodeString split_string("text1%text22%text333");
    std::vector<ai::UnicodeString> split_ref = {
        ai::UnicodeString("text1"), ai::UnicodeString("text22"), ai::UnicodeString("text333")};
    auto split = L2A::UTIL::SplitString(split_string, ai::UnicodeString("%"));
    ut.CompareStringVector(split, split_ref);

    split_string = ai::UnicodeString("text1%%text22%%text333");
    split = L2A::UTIL::SplitString(split_string, ai::UnicodeString("%%"));
    ut.CompareStringVector(split, split_ref);

    split_string = ai::UnicodeString("text1%%text22%%text333%");
    split_ref = {ai::UnicodeString("text1"), ai::UnicodeString(""), ai::UnicodeString("text22"), ai::UnicodeString(""),
        ai::UnicodeString("text333"), ai::UnicodeString("")};
    split = L2A::UTIL::SplitString(split_string, ai::UnicodeString("%"));
    ut.CompareStringVector(split, split_ref);
}

/**
 *
 */
void L2A::TEST::TestStringFunctions(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name
    ut.SetTestName(ai::UnicodeString("StringFunctions"));

    TestReferenceStringsAndStringConversion(ut);
    TestConvertIntegerToString(ut);
    TestOperatorOverloads(ut);
    TestStartsWith(ut);
    TestReplace(ut);
    TestSplit(ut);
}
