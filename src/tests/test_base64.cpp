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
 * \brief File system functions testing.
 */


#include "IllustratorSDK.h"

#include "test_base64.h"

#include "base64.h"
#include "testing_utlity.h"

#include "l2a_file_system.h"
#include "l2a_string_functions.h"


/**
 *
 */
void TestBase64Unit(L2A::TEST::UTIL::UnitTest& ut)
{
    // Some examples taken from https://en.wikipedia.org/wiki/Base64

    std::vector<char> char_vec = {107, -17, -15, 3, -1, -105, -5};
    std::string char_str(char_vec.data(), char_vec.size());

    std::vector<std::string> text = {"Man", "Ma", "M", "light work.", "light work", "light wor", "light wo", "light w",
        L2A::TEST::UTIL::test_string_4_, char_str};
    std::vector<std::string> result = {"TWFu", "TWE", "TQ", "bGlnaHQgd29yay4", "bGlnaHQgd29yaw", "bGlnaHQgd29y",
        "bGlnaHQgd28", "bGlnaHQgdw",
        "SGllciBpc3QgZWluIGFuZGVyZXIgbGFuZ2VyIFRleHQgbWl0IFVtbGF1dGVuIMOkw7bDhMOWw5zDn0AhCnVuZCBuZXVlIFplaWxlbgoKCnRoZS"
        "BlbmQ8Pi8nJyIhCg",
        "a+/xA/+X+w"};

    for (unsigned int i_value = 0; i_value < text.size(); i_value++)
    {
        std::string encoded = base64::encode(text[i_value].c_str(), text[i_value].length());
        ut.CompareStr(ai::UnicodeString(encoded), ai::UnicodeString(result[i_value]));
        auto decoded_char = base64::decode(encoded);
        std::string decoded(decoded_char.data(), decoded_char.size());
        ut.CompareStr(ai::UnicodeString(decoded), ai::UnicodeString(text[i_value]));
    }
}

/**
 *
 */
void TestBase64EnAndDecoding(L2A::TEST::UTIL::UnitTest& ut)
{
    // Get the name of the temp directory and clear it.
    const auto temp_directory = L2A::UTIL::ClearTemporaryDirectory();

    // Set name for the temp file to create.
    ai::FilePath temp_file = temp_directory;
    temp_file.AddComponent(ai::UnicodeString("l2a_test_base64.txt"));
    ai::FilePath temp_file_out = temp_directory;
    temp_file_out.AddComponent(ai::UnicodeString("l2a_test_base64_out.txt"));

    // If the file exists, delete it.
    L2A::UTIL::RemoveFile(temp_file, false);

    // Create the file with a text.
    const ai::UnicodeString test_text(L2A::TEST::UTIL::test_string_4_);
    L2A::UTIL::WriteFileUTF8(temp_file, test_text);

    // Load the file decoded in base64.
    std::string encoded_file = L2A::UTIL::encode_file_base64(temp_file);

    // Save the encoded string to file.
    L2A::UTIL::decode_file_base64(temp_file_out, L2A::UTIL::StringStdToAi(encoded_file));

    // Load the created file.
    ai::UnicodeString text_from_file = L2A::UTIL::ReadFileUTF8(temp_file_out);

    // Compare values.
    ut.CompareStr(text_from_file, ai::UnicodeString(L2A::TEST::UTIL::test_string_4_));
}

/**
 *
 */
void L2A::TEST::TestBase64(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("TestBase64"));

    TestBase64Unit(ut);
    TestBase64EnAndDecoding(ut);
}
