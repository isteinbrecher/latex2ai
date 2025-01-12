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
 * \brief Form for setting the LaTeX2AI options
 */

#ifndef L2A_UI_OPTIONS_H_
#define L2A_UI_OPTIONS_H_

#include "l2a_ui_base.h"

namespace L2A::UI
{
    /**
     * @brief Form for setting options
     */
    class Options : public FormBase
    {
       public:
        // Define names for this form
        static const std::string FORM_NAME;
        static const std::string FORM_ID;
        static const std::string EVENT_TYPE_BASE;
        static const std::string EVENT_TYPE_READY;
        static const std::string EVENT_TYPE_SAVE;
        static const std::string EVENT_TYPE_UPDATE;
        static const std::string EVENT_TYPE_GET_DEFAULT_VALUES;
        static const std::string EVENT_TYPE_SELECT_GHOST_SCRIPT;
        static const std::string EVENT_TYPE_SELECT_LATEX_BIN;
        static const std::string EVENT_TYPE_OPEN_HEADER;
        static const std::string EVENT_TYPE_CREATE_DEFAULT_HEADER;

       public:
        /**
         * @brief Constructor
         */
        Options();

        /**
         * @brief Reset internal data of the form that is not relevant after it is closed
         */
        void ResetFormData() override {};

        /**
         * @brief Open the form to edit all items in a document
         */
        void OpenOptionsForm();

        /**
         * \brief Send the complete data to the form
         */
        ASErr SendData(const std::shared_ptr<L2A::UTIL::ParameterList>& form_data);

        /**
         * @brief This function is called once the ui is loaded. We send the data to the ui here
         */
        void CallbackFormReady(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback when the form is not canceled
         */
        void CallbackSave(const csxs::event::Event* const eventParam);

        /**
         * @brief Send the default values to the form
         */
        void CallbackGetDefaultValues(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback to select the ghost script executable
         */
        void CallbackSelectGhostScript(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback to select the latex binary directory
         */
        void CallbackSelectLatex(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback to open the header
         */
        void CallbackOpenHeader(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback to create the default header
         */
        void CallbackCreateDefaultHeader(const csxs::event::Event* const eventParam);

       private:
        /**
         * @brief Convert callback data to parameter list
         */
        std::shared_ptr<L2A::UTIL::ParameterList> CallbackDataToParameterList(
            const csxs::event::Event* const eventParam);

        /**
         * @brief Set the header data in the parameter list that will be sent to the form
         */
        void SetHeaderData(const std::shared_ptr<L2A::UTIL::ParameterList> form_parameter_list);
    };
}  // namespace L2A::UI
#endif
