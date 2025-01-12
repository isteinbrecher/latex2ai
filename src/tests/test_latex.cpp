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

#include "test_latex.h"

#include "testing_utlity.h"

#include "l2a_file_system.h"
#include "l2a_latex.h"


/**
 *
 */
void TestLatexBase(L2A::TEST::UTIL::UnitTest& ut, const ai::FilePath& temp_directory)
{
    ai::FilePath test_directory = temp_directory;
    test_directory.AddComponent(ai::UnicodeString("latex_test_") + L2A::TEST::UTIL::test_string_unicode());
    L2A::UTIL::RemoveDirectoryAI(test_directory, false);
    L2A::UTIL::CreateDirectoryL2A(test_directory);

    ai::FilePath tex_file = test_directory;
    tex_file.AddComponent(ai::UnicodeString(L2A::TEST::UTIL::test_string_unicode() + "test.tex"));

    L2A::UTIL::WriteFileUTF8(
        tex_file, ai::UnicodeString(
                      "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\begin{document}Test\\end{document}"));

    // Compile the latex file
    ai::FilePath pdf_file;
    const auto compile_ok = L2A::LATEX::CompileLatexDocument(tex_file, pdf_file);
    ut.CompareInt(compile_ok, 1);
    ut.CompareInt(L2A::UTIL::IsFile(pdf_file), 1);
}

/**
 *
 */
void L2A::TEST::TestLatex(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("TestLatex"));

    // Store the working, so we can reset it at the end
    const auto old_cwd = std::filesystem::current_path();

    // Get the name of the temp directory and clear it.
    const auto temp_directory = L2A::UTIL::ClearTemporaryDirectory();

    // Test that we can create a Latex document with a unicode path
    TestLatexBase(ut, temp_directory);

    L2A::UTIL::SetWorkingDirectory(L2A::UTIL::FilePathStdToAi(old_cwd));
}
