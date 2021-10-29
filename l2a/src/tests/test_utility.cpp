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
 * \brief Test utility functions.
 */


#include "IllustratorSDK.h"
#include "test_utlity.h"

#include "testing_utlity.h"
#include "l2a_error/l2a_error.h"

/**
 *
 */
void TestErrorCodeConversion(L2A::TEST::UTIL::UnitTest& ut)
{
    int code = 'SHRT';
    ut.CompareStr(L2A::ERR::AIErrorCodeToString(code), ai::UnicodeString("SHRT"));
    code = 'ORD?';
    ut.CompareStr(L2A::ERR::AIErrorCodeToString(code), ai::UnicodeString("ORD?"));
    code = '~VAT';
    ut.CompareStr(L2A::ERR::AIErrorCodeToString(code), ai::UnicodeString("~VAT"));
}

/**
 *
 */
void L2A::TEST::TestUtilityFunctions(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("StringFunctions"));

    // Call the individual tests
    TestErrorCodeConversion(ut);
}
