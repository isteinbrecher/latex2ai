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
 * \brief Functions related to latex.
 */


#include "IllustratorSDK.h"

#include "l2a_latex.h"

#include "auto_generated/tex.h"

#include "l2a_ai_functions.h"
#include "l2a_execute.h"
#include "l2a_file_system.h"
#include "l2a_forms.h"
#include "l2a_global.h"
#include "l2a_names.h"
#include "l2a_parameter_list.h"
#include "l2a_property.h"
#include "l2a_string_functions.h"

#include <regex>


/**
 *
 */
ai::UnicodeString L2A::LATEX::GetLatexString(const ai::UnicodeString& latex_code)
{
    // Get the latex code.
    ai::UnicodeString code(L2A_LATEX_ITEM_);

    // Replace placeholders.
    L2A::UTIL::StringReplaceAll(
        code, ai::UnicodeString("{tex_header_name}"), ai::UnicodeString(L2A::NAMES::tex_header_name_));
    L2A::UTIL::StringReplaceAll(code, ai::UnicodeString("{latex_code}"), ai::UnicodeString(latex_code));

    return code;
}

/**
 *
 */
ai::UnicodeString L2A::LATEX::GetLatexCompileCommand(const ai::FilePath& tex_file)
{
    ai::UnicodeString command;
    command += L2A::Global().GetLatexCommand();
    command += " ";
    command += L2A::Global().command_latex_options_;
    command += " \"";
    command += tex_file.GetFullPath();
    command += "\"";

    return command;
}

/**
 *
 */
std::vector<ai::FilePath> L2A::LATEX::SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages)
{
    // Check if file exists
    if (!L2A::UTIL::IsFile(pdf_file))
        l2a_error("The file to split up '" + pdf_file.GetFullPath() + "' does not exits!");

    // Get name and folder of the pdf file
    const ai::UnicodeString pdf_name = pdf_file.GetFileName();
    const ai::UnicodeString pdf_name_no_ext = pdf_file.GetFileNameNoExt();
    ai::FilePath pdf_folder = pdf_file.GetParent();

    // Delete possibly existing files for the individual pages
    ai::UnicodeString split_files_regex;
    split_files_regex += pdf_name_no_ext;
    split_files_regex += "_[0-9]+\\.pdf$";
    const auto old_pdf_pages = L2A::UTIL::FindFilesInFolder(pdf_folder, split_files_regex);
    for (const auto old_split_page : old_pdf_pages)
    {
        L2A::UTIL::RemoveFile(old_split_page, false);
    }

    // Get the ghostscript command to split the pdf
    ai::UnicodeString gs_command("\"");
    gs_command += L2A::Global().command_gs_;
    gs_command += "\" -sDEVICE=pdfwrite -o ";
    gs_command += pdf_name_no_ext;
    gs_command += "_%d.pdf ";
    gs_command += pdf_name;

    // Call the command to split up the pdf file
    L2A::UTIL::SetWorkingDirectory(pdf_folder);
    auto command_result = L2A::UTIL::ExecuteCommandLine(gs_command, true);
    if (command_result.exit_status_ != 0) l2a_error("Error in the ghostscript call >>" + gs_command + "<<");

    // Get vector of pdf files for the created split items
    std::vector<ai::FilePath> pdf_files;
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::FilePath split_file = pdf_folder;
        split_file.AddComponent(pdf_name_no_ext + "_" + L2A::UTIL::IntegerToString(i) + ".pdf");
        pdf_files.push_back(split_file);

        // Check if the file was created
        if (!L2A::UTIL::IsFile(split_file))
            l2a_error("The split file '" + split_file.GetFullPath() + "' was not created!");
    }

    return pdf_files;
}

/**
 *
 */
std::pair<L2A::LATEX::LatexCreationResult, ai::FilePath> L2A::LATEX::CreateLatexItem(const L2A::Property& property)
{
    std::vector<L2A::Property> properties = {property};
    auto [latex_result, file_paths] = L2A::LATEX::CreateLatexItems(properties);
    if (latex_result.result_ == LatexCreationResult::Result::ok)
        return {latex_result, file_paths[0]};
    else
        return {latex_result, ai::FilePath(ai::UnicodeString(""))};
}

/**
 *
 */
std::pair<L2A::LATEX::LatexCreationResult, std::vector<ai::FilePath>> L2A::LATEX::CreateLatexItems(
    const std::vector<L2A::Property>& properties)
{
    std::vector<ai::FilePath> pdf_files;

    try
    {
        // Loop over all properties and get the combined the latex code as string
        ai::UnicodeString combined_latex_code("\n\n");
        for (const auto& property : properties)
        {
            combined_latex_code += property.GetLaTeXCode();
            combined_latex_code += "\n\n";
        }

        // Create the latex document
        ai::FilePath pdf_file;
        try
        {
            if (not CreateLatexDocument(combined_latex_code, pdf_file))
            {
                auto file_name = pdf_file.GetFileNameNoExt();
                auto log_file = pdf_file.GetParent();
                log_file.AddComponent(file_name + ".log");
                auto tex_file = pdf_file.GetParent();
                tex_file.AddComponent(file_name + ".tex");
                auto tex_header_file = pdf_file.GetParent();
                tex_header_file.AddComponent(ai::UnicodeString(L2A::NAMES::tex_header_name_));

                return {{LatexCreationResult::Result::error_tex_code, log_file, tex_file, tex_header_file}, {}};
            }
        }
        catch (L2A::ERR::Exception& ex)
        {
            return {{LatexCreationResult::Result::error_tex}, {}};
        }

        // Split up the created pdf file into the items, i.e., each page represents a single item. We split the document
        // into the individual pages with ghost script.
        try
        {
            pdf_files = L2A::LATEX::SplitPdfPages(pdf_file, (unsigned int)properties.size());
        }
        catch (L2A::ERR::Exception& ex)
        {
            return {{LatexCreationResult::Result::error_gs}, {}};
        }
    }
    catch (...)
    {
        return {{LatexCreationResult::Result::error_other}, {}};
    }

    // Everything worked fine
    return {{LatexCreationResult::Result::ok}, pdf_files};
}

/**
 *
 */
bool L2A::LATEX::CreateLatexDocument(const ai::UnicodeString& latex_code, ai::FilePath& pdf_file)
{
    // Create the latex files
    const ai::FilePath tex_file = WriteLatexFiles(latex_code, L2A::UTIL::GetTemporaryDirectory());

    // Remove a mabe existing pdf file.
    pdf_file = tex_file.GetParent();
    pdf_file.AddComponent(tex_file.GetFileNameNoExt() + ".pdf");
    L2A::UTIL::RemoveFile(pdf_file, false);

    // Compile the latex file
    L2A::UTIL::SetWorkingDirectory(tex_file.GetParent());
    const ai::UnicodeString latex_command = GetLatexCompileCommand(tex_file);
    const auto command_result = L2A::UTIL::ExecuteCommandLine(latex_command, false);

    // Sometimes we get 0 exit status but still no pdf file. TODO: Find the reason for that. Intermediate fix: loop as
    // long as this condition is not fulfilled any more
    if (command_result.exit_status_ == 0 and !L2A::UTIL::IsFile(pdf_file))
        l2a_error("Got 0 exit status, but no pdf file was created");

    // Check if the pdf file was created.
    if (L2A::UTIL::IsFile(pdf_file))
        return true;
    else
        return false;
}

/**
 *
 */
ai::FilePath L2A::LATEX::WriteLatexFiles(const ai::UnicodeString& latex_code, const ai::FilePath& tex_folder)
{
    // Make sure the directory exists.
    L2A::UTIL::CreateDirectoryL2A(tex_folder);

    // Define the path varaibles for this function.
    ai::FilePath tex_header_file, tex_file;
    tex_header_file = tex_file = tex_folder;
    tex_header_file.AddComponent(ai::UnicodeString(L2A::NAMES::tex_header_name_));
    tex_file.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_tex_name_));

    // Create the header in the temp directory.
    ai::UnicodeString header_string =
        L2A::UTIL::StringStdToAi(L2A::LATEX::GetHeaderWithIncludedInputs(GetHeaderPath()));
    L2A::UTIL::WriteFileUTF8(tex_header_file, header_string, true);

    // Creates the LaTeX file.
    L2A::UTIL::WriteFileUTF8(tex_file, GetLatexString(latex_code), true);

    // Return the path to the tex file.
    return tex_file;
}

/**
 *
 */
ai::UnicodeString L2A::LATEX::GetDefaultHeader() { return ai::UnicodeString(L2A_LATEX_HEADER_); }

/**
 *
 */
ai::FilePath L2A::LATEX::GetHeaderPath(const bool create_default_if_not_exist)
{
    // Header directory.
    ai::FilePath path = L2A::UTIL::GetDocumentPath().GetParent();

    // Check if the header file exists.
    path.AddComponent(ai::UnicodeString(L2A::NAMES::tex_header_name_));
    if (!L2A::UTIL::IsFile(path) && create_default_if_not_exist)
    {
        // Write the default header to file and notify the user that header was created.
        L2A::UTIL::WriteFileUTF8(path, GetDefaultHeader());
        L2A::AI::MessageAlert(ai::UnicodeString("Default header file was created, with 11pt font size."));
    }

    return path;
}

/**
 *
 */
std::string L2A::LATEX::GetHeaderWithIncludedInputs(const ai::FilePath& header_path)
{
    // Get the full path here, so relative directories will be resolved.
    auto header_path_full = L2A::UTIL::GetFullFilePath(header_path);
    auto header_dir = header_path_full.GetParent();
    auto header_text = L2A::UTIL::ReadFileUTF8(header_path_full);
    std::string header_string = L2A::UTIL::StringAiToStd(header_text);

    // Regex string to find inputs in the header.
    std::regex re_input("\\\\(input) *\\{.*\\}");

    // Loop over inputs.
    auto input_begin = std::sregex_iterator(header_string.begin(), header_string.end(), re_input);
    auto input_end = std::sregex_iterator();
    std::string return_header("");
    long long int last_pos = 0;
    for (std::sregex_iterator i = input_begin; i != input_end; ++i)
    {
        std::smatch match = *i;
        return_header += header_string.substr(last_pos, match.position() - last_pos);

        // Get the path of the input_file.
        std::smatch brackets_match;
        std::regex re_brackets("\\{.*\\}");
        auto input_string = header_string.substr(match.position(), match.length());
        std::regex_search(input_string, brackets_match, re_brackets);
        std::string input_path_string = brackets_match.str(0);
        input_path_string = input_path_string.substr(1, input_path_string.length() - 2);
        auto input_header_path = header_dir;
        input_header_path.AddComponent(ai::UnicodeString(input_path_string));
        if (L2A::UTIL::IsFile(input_header_path))
            return_header += GetHeaderWithIncludedInputs(input_header_path);
        else
            return_header += input_path_string;

        last_pos = match.position() + match.length();
    }
    return_header += header_string.substr(last_pos, header_string.length());
    return return_header;
}
