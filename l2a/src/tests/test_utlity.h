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
 * \brief Utility functions for testing.
 */

#ifndef TEST_UTILITY_H_
#define TEST_UTILITY_H_


#include "IllustratorSDK.h"


namespace L2A
{
    namespace TEST
    {
        namespace UTIL
        {
            /**
             * \brief A class that handles testing.
             */
            class UnitTest
            {
               public:
                /**
                 * \brief Default constructor.
                 */
                UnitTest() : test_name_(ai::UnicodeString("default")), test_count_(0), test_count_passed_(0){};

                /**
                 * \brief Set the name of the current test.
                 */
                void SetTestName(const ai::UnicodeString& test_name);

                /**
                 * \brief Compare two integers.
                 */
                void CompareInt(const int& val1, const int& val2);

                /**
                 * \brief Compare two strings.
                 */
                void CompareStr(const ai::UnicodeString& val1, const ai::UnicodeString& val2);

                /**
                 * \brief Compare two rectangles.
                 */
                void CompareRect(const AIRealRect& val1, const AIRealRect& val2);

                /**
                 * \brief Print summary of the tests.
                 */
                void PrintTestSummary();

               private:
                //! Name of the current test.
                ai::UnicodeString test_name_;

                //! Number of tests.
                unsigned int test_count_;

                //! Number of passed tests.
                unsigned int test_count_passed_;
            };
        }  // namespace UTIL
    }      // namespace TEST
}  // namespace L2A

#endif
