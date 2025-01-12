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

#include "test_file_system.h"

#include "testing_utlity.h"

#include "l2a_execute.h"
#include "l2a_file_system.h"
#include "l2a_string_functions.h"


/**
 *
 */
void TestFileWriteRead(
    L2A::TEST::UTIL::UnitTest& ut, const ai::FilePath& temp_directory, const ai::UnicodeString& file_name)
{
    // Set name for the temp file to create
    ai::FilePath temp_file = temp_directory;
    temp_file.AddComponent(file_name);

    // If the file exists, delete it
    L2A::UTIL::RemoveFile(temp_file, false);

    // Add a file with a temp string to check that the file will be overwritten
    L2A::UTIL::WriteFileUTF8(temp_file, ai::UnicodeString("wrong text"));

    for (const auto& test_string_data : L2A::TEST::UTIL::test_strings())
    {
        // Loop over the test strings, write them to file and read them to check if everything went ok

        // Create the file with a text
        const ai::UnicodeString test_text = test_string_data.string_;
        L2A::UTIL::WriteFileUTF8(temp_file, test_text, true);

        // Load the file and compare the results
        const ai::UnicodeString read_text = L2A::UTIL::ReadFileUTF8(temp_file);
        ut.CompareStr(read_text, test_text);

        // Compare the encoded file hash
        ai::UnicodeString encoded_file_hash =
            L2A::UTIL::StringHash(L2A::UTIL::StringStdToAi(L2A::UTIL::encode_file_base64(temp_file)));
#ifdef WIN_ENV
        ut.CompareStr(encoded_file_hash, L2A::UTIL::StringStdToAi(test_string_data.encoded_file_hash_win_));
#else
        ut.CompareStr(encoded_file_hash, L2A::UTIL::StringStdToAi(test_string_data.encoded_file_hash_mac_));
#endif
    }

    // Delete the temp file
    L2A::UTIL::RemoveFile(temp_file);
}

/**
 *
 */
void TestCreateDirectory(L2A::TEST::UTIL::UnitTest& ut, const ai::FilePath& temp_directory,
    const ai::UnicodeString& base_name_directory, const ai::UnicodeString& base_name_file)
{
    ai::FilePath test_directory = temp_directory;
    test_directory.AddComponent(base_name_directory);

    L2A::UTIL::RemoveDirectoryAI(test_directory, false);
    ut.CompareInt(false, L2A::UTIL::IsDirectory(test_directory));

    L2A::UTIL::CreateDirectoryL2A(test_directory);
    ut.CompareInt(true, L2A::UTIL::IsDirectory(test_directory));

    // Create a file in the folder
    ai::FilePath file_path = test_directory;
    file_path.AddComponent(ai::UnicodeString(base_name_file + ".tex"));
    L2A::UTIL::WriteFileUTF8(file_path, ai::UnicodeString("Test content"));
    ut.CompareInt(true, L2A::UTIL::IsFile(file_path));

    // Create multiple files and check if the find files in folder function works
    std::vector<ai::FilePath> files_to_create(4, test_directory);
    files_to_create[0].AddComponent(ai::UnicodeString(base_name_file + "_01.tex"));
    files_to_create[1].AddComponent(ai::UnicodeString(base_name_file + "_02.tex"));
    files_to_create[3].AddComponent(ai::UnicodeString(base_name_file + "_04.texx"));
    // Also create one in a sub directory -> that one should not be found
    ai::FilePath sub_dir = test_directory;
    sub_dir.AddComponent(ai::UnicodeString("tmp"));
    L2A::UTIL::CreateDirectoryL2A(sub_dir);
    files_to_create[2] = sub_dir;
    files_to_create[2].AddComponent(ai::UnicodeString(base_name_file + "_03.tex"));
    for (const auto& file : files_to_create)
    {
        L2A::UTIL::WriteFileUTF8(file, ai::UnicodeString("Test content"));
    }

    // Copy one -> that one should be found
    auto file_to_copy = test_directory;
    file_to_copy.AddComponent(ai::UnicodeString(base_name_file + "_05.tex"));
    L2A::UTIL::CopyFileL2A(files_to_create[0], file_to_copy);

    // Search for a regex match
    const std::vector<ai::FilePath> ref_files = {files_to_create[0], files_to_create[1], file_to_copy};
    const auto files_in_folder =
        L2A::UTIL::FindFilesInFolder(test_directory, ai::UnicodeString(".*_\\d{2}\\.[tT][eE][xX]$"));
    ut.CompareInt(3, static_cast<int>(files_in_folder.size()));
    for (unsigned int i = 0; i < ref_files.size(); i++)
        ut.CompareStr(ref_files[i].GetFullPath(), files_in_folder[i].GetFullPath());

    // Test that we can set this directory as working directory
    const auto current_cwd = std::filesystem::current_path();
    L2A::UTIL::SetWorkingDirectory(test_directory);
#ifndef WIN_ENV
    const auto cli_cwd = L2A::UTIL::ExecuteCommandLine(ai::UnicodeString("pwd"));
    ut.CompareStr(cli_cwd.output_, test_directory.GetFullPath() + "\n");
#endif
    L2A::UTIL::SetWorkingDirectory(L2A::UTIL::FilePathStdToAi(current_cwd));

    // Delete directory
    L2A::UTIL::RemoveDirectoryAI(test_directory);
    ut.CompareInt(false, L2A::UTIL::IsDirectory(test_directory));
    ut.CompareInt(false, L2A::UTIL::IsFile(file_path));
}

/**
 *
 */
void TestExecute(L2A::TEST::UTIL::UnitTest& ut)
{
    // Test that we can call a command with unicode characters and get the unicode result string.
    // Currently this does not work with Windoww as `echo` can't be found when calling the command.
#ifndef WIN_ENV
    const ai::UnicodeString command(ai::UnicodeString("echo ") + L2A::TEST::UTIL::test_string_unicode());
    const auto result = L2A::UTIL::ExecuteCommandLine(command);
    ut.CompareStr(result.output_, L2A::TEST::UTIL::test_string_unicode() + "\n");
#endif
}

/**
 *
 */
void L2A::TEST::TestFileSystem(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("TestFileSystem"));

    // Get the name of the temp directory and clear it.
    const auto temp_directory = L2A::UTIL::ClearTemporaryDirectory();

    // Perform the tests
    TestFileWriteRead(ut, temp_directory, ai::UnicodeString("l2a_test_file_system.txt"));
    TestFileWriteRead(ut, temp_directory, ai::UnicodeString(L2A::TEST::UTIL::test_string_unicode() + ".txt"));

    // Get the application data directory to check if this function can be executed without an error
    L2A::UTIL::GetApplicationDataDirectory();

    // Create a directory in the temp directory.
    TestCreateDirectory(ut, temp_directory, ai::UnicodeString("test1"), ai::UnicodeString("test_file"));
    TestCreateDirectory(
        ut, temp_directory, L2A::TEST::UTIL::test_string_unicode(), L2A::TEST::UTIL::test_string_unicode());

    // Test the execute function with unicode strings
    TestExecute(ut);
}
