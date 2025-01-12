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
 * \brief Test utility functions.
 */


#include "IllustratorSDK.h"

#include "test_utlity.h"
#include "testing_utlity.h"

#include "l2a_ai_functions.h"
#include "l2a_constants.h"
#include "l2a_error.h"
#include "l2a_version.h"


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
    ut.SetTestName(ai::UnicodeString("TestUtilityFunctions"));

    // Call the individual tests
    TestErrorCodeConversion(ut);
}

/**
 *
 */
void L2A::TEST::TestVersion(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("TestVersion"));

    try
    {
        const auto main_version = L2A::UTIL::ParseVersion(L2A_VERSION_STRING_);
        ut.CompareStr(ai::UnicodeString(main_version.str()), ai::UnicodeString(L2A_VERSION_STRING_));

        const auto version_from_github = L2A::UTIL::ParseVersion("v0.0.5");
        ut.CompareStr(ai::UnicodeString(version_from_github.str()), ai::UnicodeString("0.0.5"));
        ut.CompareInt(0, version_from_github.is_prerelease());

        const auto version_pre_release_4 = L2A::UTIL::ParseVersion("v1.3.5-rc.4");
        ut.CompareStr(ai::UnicodeString(version_pre_release_4.str()), ai::UnicodeString("1.3.5-rc.4"));
        ut.CompareInt(1, version_pre_release_4.is_prerelease());

        const auto version_pre_release_5 = L2A::UTIL::ParseVersion("v1.3.5-rc.5");
        ut.CompareStr(ai::UnicodeString(version_pre_release_5.str()), ai::UnicodeString("1.3.5-rc.5"));
        ut.CompareInt(1, version_pre_release_5.is_prerelease());

        const auto version_pre_release_9 = L2A::UTIL::ParseVersion("v1.3.5-rc.9");

        ut.CompareInt(1, version_pre_release_4 < version_pre_release_5);
        ut.CompareInt(0, version_pre_release_4 > version_pre_release_5);
        ut.CompareInt(0, version_pre_release_4 == version_pre_release_5);

        ut.CompareInt(1, version_pre_release_4 < version_pre_release_9);
        ut.CompareInt(0, version_pre_release_4 > version_pre_release_9);
        ut.CompareInt(0, version_pre_release_4 == version_pre_release_9);

        ut.CompareInt(1, version_pre_release_5 < version_pre_release_9);
        ut.CompareInt(0, version_pre_release_5 > version_pre_release_9);
        ut.CompareInt(0, version_pre_release_5 == version_pre_release_9);

        const auto version_alpha = L2A::UTIL::ParseVersion("v1.3.5-alpha");
        ut.CompareStr(ai::UnicodeString(version_alpha.str()), ai::UnicodeString("1.3.5-alpha"));
        ut.CompareInt(1, version_alpha.is_prerelease());

        const auto version_a = L2A::UTIL::ParseVersion("0.1.20");
        const auto version_b = L2A::UTIL::ParseVersion("1.0.2");
        const auto version_c = L2A::UTIL::ParseVersion("0.10.2");
        const auto version_d = L2A::UTIL::ParseVersion("0.1.20");
        ut.CompareInt(1, version_a < version_b);
        ut.CompareInt(1, version_a == version_d);
        ut.CompareInt(1, version_a <= version_d);
        ut.CompareInt(1, version_b > version_c);
    }
    catch (semver::semver_exception& exception)
    {
        L2A::AI::MessageAlert(ai::UnicodeString("Parsing error in version testing"));
    }
}
