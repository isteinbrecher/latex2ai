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
 * \brief Functions related to latex.
 */


#include "IllustratorSDK.h"

#include "l2a_latex.h"

#include "auto_generated/tex.h"

#include "l2a_ai_functions.h"
#include "l2a_execute.h"
#include "l2a_file_system.h"
#include "l2a_global.h"
#include "l2a_names.h"
#include "l2a_parameter_list.h"
#include "l2a_property.h"
#include "l2a_string_functions.h"

#include <regex>

#ifdef WIN_ENV
#include <Shlobj.h>
#endif


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
    // This string will contain the actual command send to the commandline
    ai::UnicodeString full_latex_command;

    const auto& latex_bin_path = L2A::Global().latex_bin_path_;
    const auto& latex_engine = L2A::Global().latex_engine_;
    if (latex_bin_path.GetFullPath() == "")
    {
        // In the case there is an empty bin directory, so we simply run the latex engine command name
        full_latex_command += latex_engine;
    }
    else
    {
        ai::FilePath exe_path = latex_bin_path;
#ifdef WIN_ENV
        exe_path.AddComponent(latex_engine + ".exe");
#else
        exe_path.AddComponent(latex_engine);
#endif
        full_latex_command += "\"" + exe_path.GetFullPath() + "\"";
    }

    // Add the options and the name of the tex file
    full_latex_command += " ";
    full_latex_command += L2A::Global().latex_command_options_;
    full_latex_command += " \"";
    full_latex_command += tex_file.GetFullPath();
    full_latex_command += "\"";
    return full_latex_command;
}

/**
 *
 */
std::vector<ai::FilePath> L2A::LATEX::SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages)
{
    return SplitPdfPages(pdf_file, n_pages, L2A::Global().gs_command_);
}

/**
 *
 */
std::vector<ai::FilePath> L2A::LATEX::SplitPdfPages(
    const ai::FilePath& pdf_file, const unsigned int& n_pages, const ai::UnicodeString& gs_command)
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
    ai::UnicodeString full_gs_command;
    full_gs_command += "\"";
    full_gs_command += gs_command;
    full_gs_command += "\" -sDEVICE=pdfwrite -o ";
    full_gs_command += pdf_name_no_ext;
    full_gs_command += "_%d.pdf ";
    full_gs_command += pdf_name;

    // Call the command to split up the pdf file
    L2A::UTIL::SetWorkingDirectory(pdf_folder);
    auto command_result = L2A::UTIL::ExecuteCommandLine(full_gs_command);
    if (command_result.exit_status_ == 127)
    {
        // This exit code means that the command was not found.
        l2a_warning("Got wrong Ghostscript path: \"" + gs_command +
                    "\". Please set the correct path to your Ghostscript executable in the LaTeX2AI options.");
    }
    else if (command_result.exit_status_ != 0)
    {
        l2a_error("Error in the ghostscript call >>" + full_gs_command +
                  "<<. Exit code: " + L2A::UTIL::IntegerToString(command_result.exit_status_));
    }

#ifdef _DEBUG
    // Check that the correct number of files was created
    const auto new_pdf_pages = L2A::UTIL::FindFilesInFolder(pdf_folder, split_files_regex);
    if (n_pages != new_pdf_pages.size())
        l2a_error("The given number of pdf pages " + L2A::UTIL::IntegerToString(n_pages) +
                  " does not match with the number of created split files " +
                  L2A::UTIL::IntegerToString(static_cast<unsigned int>(new_pdf_pages.size())));
#endif

    // Get vector of pdf files for the created split items
    std::vector<ai::FilePath> pdf_files;
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::FilePath split_file = pdf_folder;
        split_file.AddComponent(pdf_name_no_ext + "_" + L2A::UTIL::IntegerToString(i) + ".pdf");

        // Check if the file was created
        if (!L2A::UTIL::IsFile(split_file))
            l2a_error("The split file '" + split_file.GetFullPath() + "' was not created!");

        pdf_files.push_back(split_file);
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
            if (property.IsBaseline())
                combined_latex_code += ai::UnicodeString("\\LaTeXtoAIbase{");
            else
                combined_latex_code += ai::UnicodeString("\\LaTeXtoAI{");
            combined_latex_code += property.GetLaTeXCode();
            combined_latex_code += ai::UnicodeString("}\n\n");
        }

        // Create the latex document
        ai::FilePath pdf_file;
        try
        {
            if (!CreateLatexDocument(combined_latex_code, pdf_file))
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
    // Get the directory where the items shall be created
    ai::FilePath tex_directory = L2A::UTIL::GetTemporaryDirectory();
    tex_directory.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_tex_name_base_));

    // Make sure the directory exists and is empty
    L2A::UTIL::ClearDirectory(tex_directory, false);

    // Create the latex files
    const ai::FilePath tex_file = WriteLatexFiles(latex_code, tex_directory);

    // Compile the latex file
    return CompileLatexDocument(tex_file, pdf_file);
}

/**
 *
 */
bool L2A::LATEX::CompileLatexDocument(const ai::FilePath& tex_file, ai::FilePath& pdf_file)
{
    // Get the pdf file name
    pdf_file = tex_file.GetParent();
    pdf_file.AddComponent(tex_file.GetFileNameNoExt() + ".pdf");

    // Compile the latex file
    L2A::UTIL::SetWorkingDirectory(tex_file.GetParent());
    const ai::UnicodeString latex_command = GetLatexCompileCommand(tex_file);
    const auto command_result = L2A::UTIL::ExecuteCommandLine(latex_command);

    // Sometimes we get 0 exit status but still no pdf file. TODO: Find the reason for that. Intermediate fix: loop as
    // long as this condition is not fulfilled any more -> Use this fix and print the warning in debug mode
    if (command_result.exit_status_ == 0 && !L2A::UTIL::IsFile(pdf_file))
    {
        l2a_error("Got 0 exit status, but no pdf file was created");
    }
    else if (command_result.exit_status_ == 127)
    {
        l2a_warning("Got wrong LaTeX binaries path: \"" + L2A::Global().latex_bin_path_.GetFullPath() +
                    "\". Please set the correct path to your LaTeX installation in the LaTeX2AI options.");
    }

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

/**
 *
 */
ai::UnicodeString L2A::LATEX::SearchDefaultGhostScriptCommand()
{
#ifdef WIN_ENV
    // Get the path to the programs folder.
    std::array<std::tuple<REFKNOWNFOLDERID, int>, 2> programm_shortcuts = {
        std::make_tuple(FOLDERID_ProgramFilesX86, 32), std::make_tuple(FOLDERID_ProgramFiles, 64)};
    TCHAR pathBuffer[MAX_PATH];
    ITEMIDLIST* pIDList;
    for (unsigned int i = 0; i < 2; i++)
    {
        if (S_OK == SHGetKnownFolderIDList(std::get<0>(programm_shortcuts[i]), 0, nullptr, &pIDList))
        {
            if (SHGetPathFromIDList(pIDList, pathBuffer))
            {
                ai::FilePath program_folder = ai::FilePath(ai::UnicodeString(pathBuffer));
                program_folder.AddComponent(ai::UnicodeString("gs"));

                // Check if the ghostscript folder exists.
                if (L2A::UTIL::IsDirectory(program_folder))
                {
                    ai::FilePath gs_folder;
                    for (auto& p : std::filesystem::directory_iterator(L2A::UTIL::FilePathAiToStd(program_folder)))
                    {
                        // Check if the directory starts with "gs".
                        gs_folder = ai::FilePath(ai::UnicodeString(p.path().string()));
                        if (L2A::UTIL::StartsWith(gs_folder.GetFileName(), ai::UnicodeString("gs"), true))
                        {
                            // We do not care about the version -> use the first "gs*" folder that we find.

                            // Check if an execuable can be found.
                            gs_folder.AddComponent(ai::UnicodeString("bin"));
                            gs_folder.AddComponent(ai::UnicodeString("gswin") +
                                                   L2A::UTIL::IntegerToString(std::get<1>(programm_shortcuts[i])) +
                                                   "c.exe");
                            if (L2A::UTIL::IsFile(gs_folder))
                                return gs_folder.GetFullPath();
                            else
                                break;
                        }
                    }
                }
            }
        }
    }
    return ai::UnicodeString("");
#else
    // Check possible locations for ghost script
    std::vector<ai::FilePath> gs_bin_locations = {             //
        ai::FilePath(ai::UnicodeString("/opt/homebrew/bin")),  //
        ai::FilePath(ai::UnicodeString("/usr/local/bin")),     //
        ai::FilePath(ai::UnicodeString("/opt/local/bin")),     //
        ai::FilePath(ai::UnicodeString("/usr/bin"))};

    const auto& [is_gs_path, gs_path] = L2A::UTIL::FindExecutable(gs_bin_locations, ai::UnicodeString("gs"));
    if (is_gs_path)
    {
        return gs_path.GetFullPath();
    }
    else
    {
        return ai::UnicodeString("");
    }
#endif
}

/**
 *
 */
std::pair<bool, ai::UnicodeString> L2A::LATEX::GetDefaultGhostScriptCommand()
{
    // Check if the automatically found command works
    const auto default_gs_command = SearchDefaultGhostScriptCommand();
    if (L2A::LATEX::CheckGhostscriptCommand(default_gs_command))
    {
        return {true, default_gs_command};
    }

    // We did not find a command that works, raise a warning and return an empty command here
    L2A::AI::WarningAlert(ai::UnicodeString(
        "Could not determine the Ghostscript command. Please set the command yourself in the LaTeX2AI options."));
    return {false, ai::UnicodeString("")};
}

/**
 *
 */
ai::FilePath L2A::LATEX::SearchDefaultLatexPath()
{
#ifdef WIN_ENV
    return ai::FilePath(ai::UnicodeString(""));
#else
    // Check possible locations for latex
    const std::vector<ai::FilePath> latex_bin_locations = {
        ai::FilePath(
            ai::UnicodeString("/Library/TeX/texbin")),      // MacTeX (Intel and Apple Silicon, via universal binaries)
        ai::FilePath(ai::UnicodeString("/usr/local/bin")),  // Common for Homebrew (Intel)
        ai::FilePath(ai::UnicodeString("/opt/homebrew/bin")),  // Homebrew on Apple Silicon
        ai::FilePath(ai::UnicodeString("/opt/local/bin")),     // MacPorts (Intel and Apple Silicon, if supported)
        // TeX Live versions 2017 to 2024 for Intel and Apple Silicon architectures, including universal
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2024/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2024/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2024/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2023/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2023/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2023/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2022/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2022/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2022/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2021/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2021/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2021/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2020/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2020/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2020/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2019/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2019/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2019/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2018/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2018/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2018/bin/universal-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2017/bin/x86_64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2017/bin/arm64-darwin")),
        ai::FilePath(ai::UnicodeString("/usr/local/texlive/2017/bin/universal-darwin"))};

    const auto& [is_latex_path, latex_path] =
        L2A::UTIL::FindExecutable(latex_bin_locations, ai::UnicodeString("pdflatex"));
    if (is_latex_path)
    {
        return latex_path.GetParent();
    }
    else
    {
        return ai::FilePath(ai::UnicodeString(""));
    }
#endif
}

/**
 *
 */
std::pair<bool, ai::FilePath> L2A::LATEX::GetDefaultLatexPath()
{
    // First try if the default (empty) value works
    const ai::FilePath empty_latex_bin_path(ai::UnicodeString(""));
    if (L2A::LATEX::CheckLatexCommand(empty_latex_bin_path))
    {
        return {true, empty_latex_bin_path};
    }

    // Next, try to automatically find the path
    const auto default_latex_bin_path = SearchDefaultLatexPath();
    if (L2A::LATEX::CheckLatexCommand(default_latex_bin_path))
    {
        return {true, default_latex_bin_path};
    }

    // Nothing worked, raise a warning and return an empty path
    L2A::AI::WarningAlert(ai::UnicodeString(
        "Could not determine the LaTeX binaries path. Please set the path yourself in the LaTeX2AI options."));
    return {false, ai::FilePath(ai::UnicodeString(""))};
}

/**
 *
 */
bool L2A::LATEX::CheckGhostscriptCommand(const ai::UnicodeString& gs_command)
{
    if (gs_command == ai::UnicodeString(""))
    {
        return false;
    }

    ai::UnicodeString full_gs_command = "\"" + gs_command + "\" -v";
    ai::UnicodeString command_output;

    try
    {
        auto command_result = L2A::UTIL::ExecuteCommandLine(full_gs_command);
        if (command_result.output_.find(ai::UnicodeString(" Ghostscript ")) != std::string::npos)
            return true;
        else
            return false;
    }
    catch (...)
    {
        return false;
    }
}

/**
 *
 */
bool L2A::LATEX::CheckLatexCommand(const ai::FilePath& path_latex)
{
    // We check the path to the binaries with pdflatex, independently of what engine the user selects

    ai::UnicodeString command_latex;
    if (L2A::UTIL::IsDirectory(path_latex))
    {
        ai::FilePath exe_path = path_latex;
#ifdef WIN_ENV
        exe_path.AddComponent(ai::UnicodeString("pdflatex.exe"));
#else
        exe_path.AddComponent(ai::UnicodeString("pdflatex"));
#endif
        command_latex = "\"" + exe_path.GetFullPath() + "\"";
    }
    else if (path_latex.IsEmpty())
        command_latex = ai::UnicodeString("pdflatex");
    else
        // The directory does not exist and is not empty -> this will not work.
        return false;

    command_latex += " -version";
    ai::UnicodeString command_output;
    try
    {
        auto command_result = L2A::UTIL::ExecuteCommandLine(command_latex);
        if (command_result.output_.find(ai::UnicodeString("pdfTeX")) != std::string::npos)
            return true;
        else
            return false;
    }
    catch (...)
    {
        return false;
    }
}
