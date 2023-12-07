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
 * \brief File system functions testing.
 */


#include "IllustratorSDK.h"
#include "test_file_system.h"

#include "testing_utlity.h"
#include "l2a_file_system.h"

/**
 *
 */
void L2A::TEST::TestFileSystem(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("TestFileSystem"));

    // Get the name of the temp directory.
    const ai::FilePath temp_directory = L2A::UTIL::GetTemporaryDirectory();

    // Set name for the temp file to create.
    ai::FilePath temp_file = temp_directory;
    temp_file.AddComponent(ai::UnicodeString("l2a_test_file_system.txt"));

    // If the file exists, delete it.
    L2A::UTIL::RemoveFile(temp_file, false);

    // Create the file with a text.
    const ai::UnicodeString test_text(L2A::TEST::UTIL::test_string_1_);
    L2A::UTIL::WriteFileUTF8(temp_file, ai::UnicodeString("wrong text"));
    L2A::UTIL::WriteFileUTF8(temp_file, test_text, true);

    // Load the file and compare the results.
    const ai::UnicodeString read_text = L2A::UTIL::ReadFileUTF8(temp_file);
    ut.CompareStr(read_text, test_text);

    // Delete the temp file.
    L2A::UTIL::RemoveFile(temp_file);

    // Get the application data directory.
    L2A::UTIL::GetApplicationDataDirectory();

    // Create a directory in the temp directory.
    {
        ai::FilePath test_directory = temp_directory;
        test_directory.AddComponent(ai::UnicodeString("test1"));
        ai::FilePath test_directory_first = test_directory;
        ai::FilePath file_path = test_directory;
        file_path.AddComponent(ai::UnicodeString("test_file.tex"));
        L2A::UTIL::RemoveDirectoryAI(test_directory, false);
        test_directory.AddComponent(ai::UnicodeString("test2"));
        test_directory.AddComponent(ai::UnicodeString("test3"));
        L2A::UTIL::CreateDirectoryL2A(test_directory);
        ut.CompareInt(true, L2A::UTIL::IsDirectory(test_directory));

        // Create a file in the second folder.
        L2A::UTIL::WriteFileUTF8(file_path, ai::UnicodeString("Test content"));

        // Delete the last directory.
        L2A::UTIL::RemoveDirectoryAI(test_directory);
        ut.CompareInt(false, L2A::UTIL::IsDirectory(test_directory));

        // Delete the first directory.
        L2A::UTIL::RemoveDirectoryAI(test_directory_first);
        ut.CompareInt(false, L2A::UTIL::IsDirectory(test_directory_first));
        ut.CompareInt(false, L2A::UTIL::IsFile(file_path));
    }
}
