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
#include "l2a_string_functions.h"
#include "l2a_file_system.h"
#include "l2a_parameter_list.h"
#include "l2a_forms.h"
#include "l2a_global.h"
#include "l2a_names.h"
#include "l2a_execute.h"

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
    // Check if file exists.
    if (!L2A::UTIL::IsFile(pdf_file))
        l2a_error("The file to split up '" + pdf_file.GetFullPath() + "' does not exits!");

    // Get name and folder of the pdf file.
    ai::UnicodeString split_pdf_name = pdf_file.GetFileName();
    ai::UnicodeString split_pdf_name_no_ext = pdf_file.GetFileNameNoExt();
    ai::FilePath pdf_folder = pdf_file.GetParent();
    ai::FilePath pdf_pages = pdf_folder;
    pdf_pages.AddComponent(split_pdf_name_no_ext);

    // Delete existing files for the individual pages.
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::UnicodeString file_name = pdf_pages.GetFullPath();
        file_name += ai::UnicodeString("_");
        file_name += L2A::UTIL::IntegerToString(i);
        file_name += ai::UnicodeString(".pdf");
        L2A::UTIL::RemoveFile(ai::FilePath(file_name), false);
    }

    // Get the ghostscript command to split the pdf.
    ai::UnicodeString gs_command("\"");
    gs_command += L2A::Global().command_gs_;
    gs_command += "\" -sDEVICE=pdfwrite -o ";
    gs_command += split_pdf_name_no_ext;
    gs_command += "_%d.pdf ";
    gs_command += split_pdf_name;

    // Call the command to split up the pdf file.
    L2A::UTIL::SetWorkingDirectory(pdf_folder);
    auto command_result = L2A::UTIL::ExecuteCommandLine(gs_command, true);
    if (command_result.exit_status_ != 0) l2a_error("Error in the ghostscript call >>" + gs_command + "<<");

    // Get vector of pdf files for the single items.
    std::vector<ai::FilePath> pdf_files;
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::UnicodeString file_name = pdf_pages.GetFullPath();
        file_name += ai::UnicodeString("_");
        file_name += L2A::UTIL::IntegerToString(i);
        file_name += ai::UnicodeString(".pdf");
        pdf_files.push_back(ai::FilePath(file_name));

        // Check if the file was created.
        if (!L2A::UTIL::IsFile(ai::FilePath(file_name)))
            l2a_error("The split file '" + file_name + "' was not created!");
    }

    return pdf_files;
}

/**
 *
 */
bool L2A::LATEX::CreateLatex(const ai::UnicodeString& latex_code, ai::FilePath& pdf_file)
{
    // Create the latex and batch files.
    const ai::FilePath tex_file = WriteLatexFiles(latex_code, Global().path_temp_);

    // Remove a mabe existing pdf file.
    pdf_file = tex_file.GetParent();
    pdf_file.AddComponent(tex_file.GetFileNameNoExt() + ".pdf");
    L2A::UTIL::RemoveFile(pdf_file, false);

    // Compile the latex file
    L2A::UTIL::SetWorkingDirectory(tex_file.GetParent());
    const ai::UnicodeString latex_command = GetLatexCompileCommand(tex_file);
    L2A::UTIL::ExecuteCommandLine(latex_command, false);

    // Check if the pdf file was created.
    if (L2A::UTIL::IsFile(pdf_file))
        return true;
    else
        return false;
}

/**
 *
 */
L2A::LATEX::LatexCreationResult L2A::LATEX::CreateLatexWithDebug(
    const ai::UnicodeString& latex_code, ai::FilePath& pdf_file, const ai::UnicodeString& creation_type)
{
    // Try to create the Latex document.
    if (L2A::LATEX::CreateLatex(latex_code, pdf_file))
    {
        // Creation was successfull.
        return L2A::LATEX::LatexCreationResult::item_created;
    }
    else
    {
        // Pdf could not be be created, this is most likely an latex error -> Open the debug form.

        // Get the parameters for the debug form.
        L2A::UTIL::ParameterList debug_parameter_list;
        debug_parameter_list.SetOption(ai::UnicodeString("creation_type"), creation_type);
        ai::FilePath log_file = pdf_file.GetParent();
        log_file.AddComponent(pdf_file.GetFileNameNoExt() + ".log");
        debug_parameter_list.SetOption(ai::UnicodeString("log_file"), log_file.GetFullPath());

        // Open the debug form. This is done in a while loop, as the user can choose options that require the form to be
        // opened again.
        std::shared_ptr<L2A::UTIL::ParameterList> new_parameter_list;
        while (true)
        {
            if (!L2A::Form(ai::UnicodeString("l2a_debug"), debug_parameter_list, new_parameter_list).canceled)
            {
                if (new_parameter_list->GetIntOption(ai::UnicodeString("create_debug_folder")) == 1)
                {
                    // The user wants to create the debug folder in the header directory.
                    ai::FilePath debug_directory = L2A::UTIL::GetPdfFileDirectory();
                    debug_directory.AddComponent(ai::UnicodeString("debug"));
                    if (L2A::UTIL::IsDirectory(debug_directory))
                    {
                        // In this case the user has to select the path manually.
                        AIBoolean form_result;
                        ai::UnicodeString form_string("");
                        form_string += "The debug folder \"" + debug_directory.GetFullPath() +
                            "\" and its contents will be delete in this process. Do you want to continue?";
                        form_result = sAIUser->OKCancelAlert(form_string, true, nullptr);

                        if (!form_result)
                            // Do not create the folder and open the main debug form again.
                            continue;
                    }

                    // Clear the contents in the debug folder.
                    L2A::UTIL::RemoveDirectoryAI(debug_directory, false);
                    L2A::UTIL::CreateDirectoryL2A(debug_directory);

                    // Create all relevant latex files in the debug folder.
                    L2A::LATEX::WriteLatexFiles(latex_code, debug_directory);
                    // TODO: Add command call here as well
                    sAIUser->MessageAlert("The debug files were created in \"" + debug_directory.GetFullPath() + "\"");

#ifdef WIN_ENV
                    // Open explorer in the debug folder.
                    ShellExecute(nullptr, "open", debug_directory.GetFullPath().as_Platform().c_str(), nullptr, nullptr,
                        SW_SHOWDEFAULT);
#endif
                }
                else
                    // The user wants to redo the item.
                    return L2A::LATEX::LatexCreationResult::fail_redo;
            }
            else
                // The form was canceled, stop the creation process.
                return L2A::LATEX::LatexCreationResult::fail_quit;
        }
    }
}

/**
 *
 */
ai::FilePath L2A::LATEX::WriteLatexFiles(const ai::UnicodeString& latex_code, const ai::FilePath& tex_folder)
{
    // Make sure the directory exists.
    L2A::UTIL::CreateDirectoryL2A(tex_folder);

    // Define the path varaibles for this function.
    ai::FilePath tex_header_file, tex_file, batch_file;
    tex_header_file = tex_file = batch_file = tex_folder;
    tex_header_file.AddComponent(ai::UnicodeString(L2A::NAMES::tex_header_name_));
    tex_file.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_tex_name_));
    batch_file.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_batch_name_));

    // Create the header in the temp directory.
    ai::UnicodeString header_string = L2A::UTIL::StringStdToAi(L2A::LATEX::GetHeaderWithIncludedInputs(GetHeaderPath()));
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
        L2A::MessageAlert(ai::UnicodeString("Default header file was created, with 11pt font size."));
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
