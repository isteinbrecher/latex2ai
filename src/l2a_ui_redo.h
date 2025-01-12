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
 * \brief Form for redoing all items
 */

#ifndef L2A_UI_REDO_H_
#define L2A_UI_REDO_H_

#include "l2a_item.h"
#include "l2a_property.h"
#include "l2a_ui_base.h"

namespace L2A::UI
{
    /**
     * @brief Form for redoing all items
     */
    class Redo : public FormBase
    {
       public:
        // Define names for this form
        static const std::string FORM_NAME;
        static const std::string FORM_ID;
        static const std::string EVENT_TYPE_BASE;
        static const std::string EVENT_TYPE_READY;
        static const std::string EVENT_TYPE_OK;
        static const std::string EVENT_TYPE_UPDATE;

       public:
        /**
         * @brief Constructor
         */
        Redo();

        /**
         * @brief Reset internal data of the form that is not relevant after it is closed
         */
        void ResetFormData() override;

        /**
         * @brief Open the form to edit all items in a document
         */
        void OpenRedoForm();

        /**
         * @brief This function is called once the ui is loaded. We send the data to the ui here
         */
        void CallbackFormReady(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback when the form is not canceled
         */
        void CallbackOk(const csxs::event::Event* const eventParam);

        /**
         * \brief Send data to the form
         */
        ASErr SendData() override;

       private:
        //! Vector with all LaTeX2AI items
        std::vector<AIArtHandle> all_items_;

        //! Vector with all selected LaTeX2AI items
        std::vector<AIArtHandle> selected_items_;
    };
}  // namespace L2A::UI
#endif
