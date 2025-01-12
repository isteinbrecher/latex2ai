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


#ifndef L2A_LATEX_H_
#define L2A_LATEX_H_


#include "IllustratorSDK.h"

#include "l2a_error.h"
#include "l2a_names.h"


namespace L2A
{
    // Forward declarations
    class Property;

    namespace LATEX
    {
        /**
         * \brief Container for the result of a Latex creation operation
         */
        struct LatexCreationResult
        {
            /**
             * \brief Enum for the result
             */
            enum class Result
            {
                //! Everything worked fine
                ok,
                //! The latex command succeeded, but no file was created
                error_tex_code,
                //! The creation of the latex document failed
                error_tex,
                //! The split with ghostscript failed
                error_gs,
                //! Other error
                error_other
            };

            //! Result flag
            Result result_;

            //! Path to the log file
            ai::FilePath log_file_;

            //! Path to the tex file
            ai::FilePath tex_file_;

            //! Path to the tex header
            ai::FilePath tex_header_file_;
        };

        /**
         * \brief Get the full LaTeX text for a given latex code.
         */
        ai::UnicodeString GetLatexString(const ai::UnicodeString& latex_code);

        /**
         * \brief Get command to compile the tex document.
         */
        ai::UnicodeString GetLatexCompileCommand(const ai::FilePath& tex_file);

        /**
         * \brief Split up a pdf document in a single pdf file for each page.
         *
         * Optionally the path to the ghost script command can be given. Per default the one from the global object is
         * taken.
         */
        std::vector<ai::FilePath> SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages);
        std::vector<ai::FilePath> SplitPdfPages(
            const ai::FilePath& pdf_file, const unsigned int& n_pages, const ai::UnicodeString& gs_command);

        /**
         * \brief Create a latex document for a latex code string
         * @param (in/out) property Property containing the item property that should be converted. If everything is
         * successful the pdf contents are stored in the property.
         * @return Result of the latex creation function
         */
        std::pair<LatexCreationResult, ai::FilePath> CreateLatexItem(const L2A::Property& property);

        /**
         * \brief Create a latex document for a latex code string
         * @param (in/out) properties Vector containing all item properties that should be converted. If everything
         * is successful the pdf contents are stored in the properties.
         * @return Result of the latex creation function
         */
        std::pair<LatexCreationResult, std::vector<ai::FilePath>> CreateLatexItems(
            const std::vector<L2A::Property>& properties);

        /**
         * \brief Create a latex document for a latex code string.
         * @param (in) Latex_code String with the full latex code to be compiled.
         * @param (out) Path of the created pdf file.
         * @return True if creation was successful.
         */
        bool CreateLatexDocument(const ai::UnicodeString& latex_code, ai::FilePath& pdf_file);

        /**
         * \brief Actually compile the latex document.
         * @param (in) tex_file Path to the tex file.
         * @param (out) Path of the created pdf file.
         * @return True if creation was successful.
         */
        bool CompileLatexDocument(const ai::FilePath& tex_file, ai::FilePath& pdf_file);

        /**
         * \brief Create all the files that are needed to create a latex document.
         * @return Path to the main latex document.
         */
        ai::FilePath WriteLatexFiles(const ai::UnicodeString& latex_code, const ai::FilePath& tex_folder);

        /**
         * \brief Get the default header string.
         */
        ai::UnicodeString GetDefaultHeader();

        /**
         * \brief Get the path to the header for the current document. If no header exists, a default one will be
         * created.
         */
        ai::FilePath GetHeaderPath(const bool create_default_if_not_exist = true);

        /**
         * \brief Get the header as a string, where all inputs are resolved.
         */
        std::string GetHeaderWithIncludedInputs(const ai::FilePath& header_path);

        /**
         * \brief Search the path to ghostscript on the system.
         */
        ai::UnicodeString SearchDefaultGhostScriptCommand();

        /**
         * \brief Get a path to ghostscript on the system. It is checked if this path works, the first item in the
         * return pair is the flag if the command is fine.
         */
        std::pair<bool, ai::UnicodeString> GetDefaultGhostScriptCommand();

        /**
         * \brief Search the path to the latex binaries on the system.
         */
        ai::FilePath SearchDefaultLatexPath();

        /**
         * \brief Search the path to the latex binaries on the system. It is checked if this path works, the first item
         * in the return pair is the flag if the command is fine.
         */
        std::pair<bool, ai::FilePath> GetDefaultLatexPath();

        /**
         * \brief Check if the ghostscript command is valid.
         */
        bool CheckGhostscriptCommand(const ai::UnicodeString& gs_command);

        /**
         * \brief Check that the stored LaTeX command is correct.
         */
        bool CheckLatexCommand(const ai::FilePath& path_latex);
    }  // namespace LATEX
}  // namespace L2A

#endif
