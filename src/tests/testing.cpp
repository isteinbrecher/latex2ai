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
 * \brief Main function for testing.
 */


#include "IllustratorSDK.h"

#include "testing.h"

#include "test_base64.h"
#include "test_file_system.h"
#include "test_framework.h"
#include "test_latex.h"
#include "test_parameter_list.h"
#include "test_string_functions.h"
#include "test_utlity.h"
#include "testing_utlity.h"


/**
 *
 */
void L2A::TEST::TestingMain(const bool print_status)
{
    // Create the testing object.
    L2A::TEST::UTIL::UnitTest ut;

    // Call the individual testing functions.
    L2A::TEST::TestParameterList(ut);
    L2A::TEST::TestStringFunctions(ut);
    L2A::TEST::TestFileSystem(ut);
    L2A::TEST::TestUtilityFunctions(ut);
    L2A::TEST::TestVersion(ut);
    L2A::TEST::TestBase64(ut);
    L2A::TEST::TestLatex(ut);

    // Print the testing summary. For now this is deactivated.
    ut.PrintTestSummary(print_status);
}

/**
 *
 */
void L2A::TEST::TestFramework(const bool print_status)
{
    // Create the testing object.
    L2A::TEST::UTIL::UnitTest ut;

    // Call the framework test.
    L2A::TEST::TestFramework(ut);

    // Print the testing summary. For now this is deactivated.
    ut.PrintTestSummary(print_status);
}
