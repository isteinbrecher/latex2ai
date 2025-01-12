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
 * \brief Define constant names and function related to names for LaTeX2AI.
 */


#ifndef L2A_NAMES_H_
#define L2A_NAMES_H_


#include "l2a_string_functions.h"

namespace L2A
{
    namespace NAMES
    {
        //! Name of the directory where the L2A header and pdfs will be stored.
        static const char* pdf_file_directory_ = "links";

        //! Name of header in the L2A directory.
        static const char* tex_header_name_ = "LaTeX2AI_header.tex";

        //! Postfix to the document name for the pdf items.
        static const char* pdf_item_post_fix_ = "_LaTeX2AI_";

        //! Name for the item in ai.
        static const char* ai_item_name_ = "LaTeX2AI";

        //! Name for the tex file when creating a latex item.
        static const char* create_pdf_tex_name_base_ = "LaTeX2AI_item";
        static const char* create_pdf_tex_name_ =
            "LaTeX2AI_item"
            ".tex";

        /**
         * \brief Get the name of a pdf for an item of the current document.
         */
        inline ai::UnicodeString GetPdfItemName(const ai::UnicodeString& document_name, const unsigned int i)
        {
            return document_name + L2A::NAMES::pdf_item_post_fix_ + L2A::UTIL::IntegerToString(i, 3) + ".pdf";
        }
    }  // namespace NAMES
}  // namespace L2A


#endif  // L2A_NAMES_H_
