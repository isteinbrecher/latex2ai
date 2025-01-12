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

#ifndef TESTING_UTILITY_H_
#define TESTING_UTILITY_H_


#include "IllustratorSDK.h"


namespace L2A
{
    namespace TEST
    {
        namespace UTIL
        {
            //! Strings that are used to compare during testing.
            static const char* test_string_1_ = "Umlaute äöÄÖÜß@!\nund neue Zeile";
            static const char* test_string_2_ =
                "Hier ist ein langer Text mit Umlauten äöÄÖÜß@!\nund neue Zeilen\n\n\nthe end<>/''\"!\n";
            static const char* test_string_3_ = "    \n    \n    \n    \nMainOptionText\n    \n    \n";
            static const char* test_string_4_ =
                "Hier ist ein anderer langer Text mit Umlauten äöÄÖÜß@!\nund neue Zeilen\n\n\nthe end<>/''\"!\n";
            static const char* test_string_5_ = "abcdefg123567890";

            //! Unicode string
            const char16_t test_string_unicode_characters[] = {
                0x041F, 0x0440, 0x0438, 0x0432, 0x0435, 0x0442, 0x0020, 0x043C, 0x0438, 0x0440, 0x0021, 0x0020, 0x3053,
                0x3093, 0x306B, 0x3061, 0x306F, 0x4E16, 0x754C, 0xFF01,
                0x0000  // Null terminator
            };
            ai::UnicodeString test_string_unicode();
            ai::UnicodeString test_string_unicode_multiline();

            //! Container to store the data for the test string comparisons
            struct TestStringData
            {
                ai::UnicodeString string_;
                std::string string_hash_;
                std::string encoded_file_hash_win_;
                std::string encoded_file_hash_mac_;
            };

            //! Vector of the strings including their hashes. Sine the line endings are different, the strings with a
            //! new line have different hashes for Windows and MacOS
            std::vector<TestStringData> test_strings();

            /**
             * \brief A class that handles testing.
             */
            class UnitTest
            {
               public:
                /**
                 * \brief Default constructor.
                 */
                UnitTest() : test_name_(ai::UnicodeString("default")), test_count_(0), test_count_passed_(0) {};

                /**
                 * \brief Set the name of the current test.
                 */
                void SetTestName(const ai::UnicodeString& test_name);

                /**
                 * \brief Compare two integers.
                 */
                void CompareInt(const int& val1, const int& val2);

                /**
                 * \brief Compare two floats with a tollerance.
                 */
                void CompareFloat(const AIReal& val1, const AIReal& val2, const AIReal& eps);

                /**
                 * \brief Compare two strings.
                 */
                void CompareStr(const ai::UnicodeString& val1, const ai::UnicodeString& val2);

                /**
                 * \brief Compare two rectangles.
                 */
                void CompareRect(const AIRealRect& val1, const AIRealRect& val2);

                /**
                 * \brief Compare two string vectors.
                 */
                void CompareStringVector(
                    const std::vector<ai::UnicodeString>& val1, const std::vector<ai::UnicodeString>& val2);

                /**
                 * \brief Print summary of the tests.
                 */
                void PrintTestSummary(const bool print_status);

               private:
                //! Name of the current test.
                ai::UnicodeString test_name_;

                //! Number of tests.
                unsigned int test_count_;

                //! Number of passed tests.
                unsigned int test_count_passed_;
            };
        }  // namespace UTIL
    }  // namespace TEST
}  // namespace L2A

#endif
