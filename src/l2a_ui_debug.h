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
 * \brief Form for debugging LaTeX errors
 */

#ifndef L2A_UI_DEBUG_H_
#define L2A_UI_DEBUG_H_

#include "l2a_latex.h"
#include "l2a_ui_base.h"

namespace L2A::UI
{
    /**
     * @brief Form for creating / editing an item
     */
    class Debug : public FormBase
    {
       public:
        // Define names for this form
        static const std::string FORM_NAME;
        static const std::string FORM_ID;
        static const std::string EVENT_TYPE_BASE;
        static const std::string EVENT_TYPE_READY;
        static const std::string EVENT_TYPE_OK;
        static const std::string EVENT_TYPE_CANCEL;
        static const std::string EVENT_TYPE_OPEN_LOG;
        static const std::string EVENT_TYPE_CREATE_DEBUG;
        static const std::string EVENT_TYPE_UPDATE;

        /**
         * \brief Flags for the type of current latex creation
         */
        enum class Action
        {
            none,
            create_item,
            edit_item,
            redo_items
        };

       public:
        /**
         * @brief Constructor
         */
        Debug();

        /**
         * @brief Reset internal data of the form that is not relevant after it is closed
         */
        void ResetFormData() override { action_ = Action::none; }

        /**
         * @brief Open the debug form
         * TODO: Check return type of this method and error handling
         */
        void OpenDebugForm(const Action action, L2A::LATEX::LatexCreationResult& latex_creation_result);

        /**
         * @brief This function is called once the ui is loaded. We send the data to the ui here
         */
        void CallbackFormReady(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback when the redo latex code button is pressed
         */
        void CallbackReeditLatexCode(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback when the form is canceled
         */
        void CallbackCancel(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback for opening the log file
         */
        void CallbackOpenLog(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback for creating the debug folder
         */
        void CallbackCreateDebugFolder(const csxs::event::Event* const eventParam);

        /**
         * \brief Send data to the form
         */
        ASErr SendData() override;

       private:
        //! Flag for current latex creationß
        Action action_;

        //! Data from the LaTeX creation process
        L2A::LATEX::LatexCreationResult latex_creation_result_;
    };
}  // namespace L2A::UI
#endif
