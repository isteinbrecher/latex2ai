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
 * \brief Functions related to latex.
 */


#include "IllustratorSDK.h"
#include "l2a_latex.h"

#include "utility/string_functions.h"
#include "utility/file_system.h"
#include "utility/parameter_list.h"
#include "l2a_forms/l2a_forms.h"
#include "l2a_global/l2a_global.h"
#include "l2a_names.h"


/**
 *
 */
ai::UnicodeString L2A::LATEX::GetLatexString(const ai::UnicodeString& latex_code)
{
    // Create the latex code.
    ai::UnicodeString temp = GetTexLicense();
    temp += "\n% include the packages defined by the user\n";
    temp += "\\input{";
    temp += ai::UnicodeString(L2A::NAMES::tex_header_name_);
    temp += "}\n";
    temp += "\n";
    temp += "% Encoding options\n";
    temp += "\\usepackage[utf8]{inputenc}\n";
    temp += "\\usepackage[T1]{fontenc}\n";
    temp += "\n";
    temp += "% to create the line for baseline placement so Illustrator does not crop the pdf\n";
    temp += "\\usepackage{tikz}\n";
    temp += "\n";
    temp += "% dimensions for tikzpicture\n";
    temp += "\\newlength\\maxheight\n";
    temp += "\\newlength\\diffheight\n";
    temp += "\n";
    temp += "% configuration for standalone package\n";
    temp += "\\standaloneconfig{border=1pt, multi}\n";
    temp += "\n";
    temp += "% standalone environment\n";
    temp += "\\newenvironment{lta}{\\ignorespaces}{\\ignorespacesafterend}\n";
    temp += "\\standaloneenv{lta}\n";
    temp += "\n";
    temp += "% environment for standard placement\n";
    temp += "\\newcommand{\\LaTeXtoAI}[1]{%\n";
    temp += "    \\begin{lta}%\n";
    temp += "        \\scalebox{\\itemscalefactor}{#1}%\n";
    temp += "    \\end{lta}%\n";
    temp += "}\n";
    temp += "\n";
    temp += "% environment for baseline placement\n";
    temp += "\\newbox\\ltabox\n";
    temp += "\\newcommand{\\LaTeXtoAIbase}[1]{\n";
    temp += "	\\begin{lta}\n";
    temp += "	    \\scalebox{\\itemscalefactor}{%\n";
    temp += "		\\setbox\\ltabox\\hbox{%\n";
    temp += "            #1%\n";
    temp += "        }%\n";
    temp += "        \\begin{tikzpicture}[baseline={(current bounding box.center)}]%\n";
    temp += "            \\pgfmathsetlength{\\maxheight}{max(\\ht\\ltabox,\\dp\\ltabox)+1pt};\n";
    temp += "            \\pgfmathsetlength{\\diffheight}{0.01pt};\n";
    temp +=
        "            \\draw [line width=0.5\\diffheight, opacity=0, draw=white](0,\\maxheight-\\diffheight) "
        "--(0,\\maxheight);%\n";
    temp +=
        "            \\draw [line width=0.5\\diffheight, opacity=0, draw=white](0,-\\maxheight+\\diffheight) "
        "--(0,-\\maxheight);%\n";
    temp += "        \\end{tikzpicture}%\n";
    temp += "        \\unhbox\\ltabox}%\n";
    temp += "	\\end{lta}%\n";
    temp += "}\n";
    temp += "\n";
    temp += "\\begin{document}\n";

    // Add the 'real' code.
    temp += latex_code;

    // Finish latex document.
    temp += "\n\\end{document}";

    return temp;
}

/**
 *
 */
ai::UnicodeString L2A::LATEX::GetBatchString(const ai::FilePath& batch_directory,
    const ai::UnicodeString& latex_compiler, const ai::UnicodeString& latex_options, const ai::UnicodeString& tex_name)
{
    ai::UnicodeString path_unicode = batch_directory.GetFullPath();
    ai::UnicodeString temp(path_unicode.substr(0, 1));
    temp += ":\n";
    temp += "cd ";
    temp += path_unicode;
    temp += "\n\"";
    temp += latex_compiler;
    temp += "\" ";
    temp += latex_options;
    temp += " \"";
    temp += tex_name;
    temp += "\"\n\n";

    return temp;
}

/**
 *
 */
std::vector<ai::FilePath> L2A::LATEX::SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages)
{
    // Check if file exists.
    if (!L2A::UTIL::IsFile(pdf_file))
        l2a_error("The file to split up '" + pdf_file.GetFullPath() + "' does not exits!");

    // Ghostscript path.
    ai::UnicodeString gs_path("\"");
    gs_path += L2A::Global().command_gs_;
    gs_path += ai::UnicodeString("\"");

    // Get name and folder of pdf file.
    ai::UnicodeString split_pdf_name = pdf_file.GetFileNameNoExt();
    ai::FilePath pdf_folder = pdf_file.GetParent();
    ai::FilePath batch_file = pdf_folder;
    batch_file.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_split_batch_name_));
    ai::FilePath pdf_pages = pdf_folder;
    pdf_pages.AddComponent(split_pdf_name);

    // Delete existing files.
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::UnicodeString file_name = pdf_pages.GetFullPath();
        file_name += ai::UnicodeString("_");
        file_name += L2A::UTIL::IntegerToString(i, 3);
        file_name += ai::UnicodeString(".pdf");
        L2A::UTIL::RemoveFile(ai::FilePath(file_name), false);
    }

    // Create string for system to execute.
    ai::UnicodeString command("");
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        command += gs_path;
        command += ai::UnicodeString(" -dBATCH -dSAFER -dNOPAUSE -dFirstPage=");
        command += L2A::UTIL::IntegerToString(i);
        command += ai::UnicodeString(" -dLastPage=");
        command += L2A::UTIL::IntegerToString(i);
        command += ai::UnicodeString(" -sDEVICE=pdfwrite -o \"");
        command += pdf_pages.GetFullPath();
        command += ai::UnicodeString("_");
        command += L2A::UTIL::IntegerToString(i, 3);
        command += ai::UnicodeString(".pdf\" \"");
        command += pdf_file.GetFullPath();
        command += ai::UnicodeString("\"\n");
    }

    // Write command to batch file.
    L2A::UTIL::WriteFileUTF8(batch_file, command, true);

    // Execute batch file.
    L2A::UTIL::ExecuteFile(batch_file);

    // Get vector of pdf files for the single items.
    std::vector<ai::FilePath> pdf_files;
    for (unsigned int i = 1; i <= n_pages; i++)
    {
        ai::UnicodeString file_name = pdf_pages.GetFullPath();
        file_name += ai::UnicodeString("_");
        file_name += L2A::UTIL::IntegerToString(i, 3);
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
    // Remove a mabe existing file.
    ai::FilePath batch_file;
    pdf_file = batch_file = L2A::Global().path_temp_;
    pdf_file.AddComponent(ai::UnicodeString(L2A::NAMES::create_pdf_tex_name_base_) + ".pdf");
    L2A::UTIL::RemoveFile(pdf_file, false);

    // Create the latex and batch files.
    batch_file = WriteLatexFiles(latex_code, Global().path_temp_);

    // Execute the batch file.
    L2A::UTIL::ExecuteFile(batch_file);

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
            if (L2A::Form(ai::UnicodeString("l2a_debug"), debug_parameter_list, new_parameter_list))
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
                        form_result = sAIUser->OKCancelAlert(form_string, true, NULL);

                        if (!form_result)
                            // Do not create the folder and open the main debug form again.
                            continue;
                    }

                    // Clear the contents in the debug folder.
                    L2A::UTIL::RemoveDirectoryAI(debug_directory, false);
                    L2A::UTIL::CreateDirectoryL2A(debug_directory);

                    // Create all relevant latex files in the debug folder.
                    L2A::LATEX::WriteLatexFiles(latex_code, debug_directory);
                    sAIUser->MessageAlert("The debug files were created in \"" + debug_directory.GetFullPath() + "\"");

                    // Open explorer in the debug folder.
                    ShellExecute(
                        NULL, "open", debug_directory.GetFullPath().as_Platform().c_str(), NULL, NULL, SW_SHOWDEFAULT);
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

    // Copy the header into the temp directory.
    L2A::UTIL::CopyFileL2A(GetHeaderPath(), tex_header_file);

    // Creates the LaTeX file.
    L2A::UTIL::WriteFileUTF8(tex_file, GetLatexString(latex_code), true);

    // Create the batch file.
    L2A::UTIL::WriteFileUTF8(batch_file,
        GetBatchString(tex_folder, L2A::Global().GetLatexCommand(), L2A::Global().command_latex_options_), true);

    // Return the batch file so it can be executed.
    return batch_file;
}

/**
 *
 */
ai::UnicodeString L2A::LATEX::GetDefaultHeader()
{
    ai::UnicodeString header = GetTexLicense();
    header += "\n% DO NOT change the document class\n";
    header += "\\documentclass[class=scrartcl]{standalone}\n";
    header += "\\KOMAoptions{fontsize=11pt}\n";
    header += "\\usepackage{amsmath}\n\n";
    header += "% All items can be scaled using this variable. This variable HAS to be defined (default 1).\n";
    header += "\\newcommand{\\itemscalefactor}{1}\n\n";
    header += "% Add needed packages and macros here:\n";
    return header;
}

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
ai::UnicodeString L2A::LATEX::GetTexLicense() { return ai::UnicodeString(L2A::LICENSE::tex_license_); }
