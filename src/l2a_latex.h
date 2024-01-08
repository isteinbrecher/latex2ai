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
         * \brief Enum for the result of an Latex creation.
         */
        enum class LatexCreationResult
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

        /**
         * \brief Enum for the result of an Latex creation.
         */
        enum class LatexCreationDebugResult
        {
            //! Everything worked fine.
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
         * \brief Get command to compile the tex document.
         */
        ai::UnicodeString GetLatexCompileCommand(const ai::FilePath& tex_file);

        /**
         * \brief Split up a pdf document in a single pdf file for each page.
         */
        std::vector<ai::FilePath> SplitPdfPages(const ai::FilePath& pdf_file, const unsigned int& n_pages);

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
         * \brief Create a latex document for a latex code string, if the creation fails, a form with debug options
         * will be opened.
         * @param (in) Latex_code String with the full latex code to be compiled.
         * @param (out) Path of the created pdf file.
         * @param (in) Creation type for the debug form.
         * @return Result of the Latex creation.
         */
        LatexCreationDebugResult CreateLatexWithDebug(
            const ai::UnicodeString& latex_code, ai::FilePath& pdf_file, const ai::UnicodeString& creation_type);

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
    }  // namespace LATEX
}  // namespace L2A

#endif
