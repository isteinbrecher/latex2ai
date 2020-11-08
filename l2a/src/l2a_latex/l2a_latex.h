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


#ifndef L2A_LATEX_H_
#define L2A_LATEX_H_


#include "IllustratorSDK.h"

#include "l2a_names.h"
#include "l2a_error/l2a_error.h"


namespace L2A
{
    namespace LATEX
    {
        /**
         * \brief Enum for the result of an Latex creation.
         */
        enum class LatexCreationResult
        {
            //! Everyting worked fine.
            item_created,
            //! The creation failed and the user wants to edit the item.
            fail_redo,
            //! The creation failed and the user wants to quit.
            fail_quit
        };

        /**
         * \brief Get the full LaTeX text for a given latex code.
         */
        ai::UnicodeString GetLatexString(const ai::UnicodeString& latex_code);

        /**
         * \brief Get string that will be put in the bat file to create the latex item.
         */
        ai::UnicodeString GetBatchString(const ai::FilePath& batch_directory, const ai::UnicodeString& latex_compiler,
            const ai::UnicodeString& latex_options,
            const ai::UnicodeString& tex_name = ai::UnicodeString(L2A::NAMES::create_pdf_tex_name_));

        /**
         * \brief Split up a pdf document in a single pdf file for each page.
         */
        std::vector<ai::FilePath> SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages);

        /**
         * \brief Create a latex document for a latex code string.
         * @param (in) Latex_code String with the full latex code to be compiled.
         * @param (out) Path of the created pdf file.
         * @return True if creation was successfull.
         */
        bool CreateLatex(const ai::UnicodeString& latex_code, ai::FilePath& pdf_file);

        /**
         * \brief Create a latex document for a latex code string, if the creation failes, a form with debug options
         * will be opened.
         * @param (in) Latex_code String with the full latex code to be compiled.
         * @param (out) Path of the created pdf file.
         * @param (in) Creation type for the debug form.
         * @return Result of the Latex creation.
         */
        L2A::LATEX::LatexCreationResult CreateLatexWithDebug(
            const ai::UnicodeString& latex_code, ai::FilePath& pdf_file, const ai::UnicodeString& creation_type);

        /**
         * \brief Create all the files that are needed to create a latex document.
         * @return Path to the batch file.
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
        ai::FilePath GetHeaderPath();

        /**
         * \brief Get the license information for tex files.
         */
        ai::UnicodeString GetTexLicense();

        /**
         * \brief Get the header as a string, wehre all inputs are resoved.
         */
        ai::UnicodeString GetHeaderWithIncludedInputs(const ai::FilePath& header_path);
    }  // namespace LATEX
}  // namespace L2A

#endif
