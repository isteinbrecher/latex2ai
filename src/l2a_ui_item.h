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
 * \brief Form for creating / editing an item
 */

#ifndef L2A_UI_ITEM_H_
#define L2A_UI_ITEM_H_

#include "l2a_item.h"
#include "l2a_property.h"
#include "l2a_ui_base.h"

namespace L2A::UI
{
    /**
     * @brief Form for creating / editing an item
     */
    class Item : public FormBase
    {
       public:
        // Define names for this form
        static const std::string FORM_NAME;
        static const std::string FORM_ID;
        static const std::string EVENT_TYPE_BASE;
        static const std::string EVENT_TYPE_READY;
        static const std::string EVENT_TYPE_OK;
        static const std::string EVENT_TYPE_UPDATE;
        static const std::string EVENT_TYPE_SET_CLOSE_ON_FOCUS;

       public:
        /**
         * @brief Constructor
         */
        Item();

        /**
         * @brief Reset internal data of the form that is not relevant after it is closed
         */
        void ResetFormData() override;

        /**
         * @brief Create a new item at the given cursor position
         * TODO: Check return type of this method and error handling
         */
        void OpenCreateItemForm(const AIRealPoint& position);

        /**
         * @brief Edit an existing item
         * TODO: Check return type of this method and error handling
         */
        void OpenEditItemForm(const AIArtHandle& art_handle);

        /**
         * @brief This function is called once the ui is loaded. We send the data to the ui here
         */
        void CallbackFormReady(const csxs::event::Event* const eventParam);

        /**
         * @brief Callback when the form is not canceled
         */
        void CallbackOk(const csxs::event::Event* const eventParam);

        /**
         * @brief Compile the data from the form to a new L2A item
         */
        void CreateNewItem(const L2A::UTIL::ParameterList& item_data_from_form);

        /**
         * @brief Edit an existing L2A item
         */
        void EditItem(const ai::UnicodeString& return_value, const L2A::UTIL::ParameterList& item_data_from_form);

        /**
         * \brief Send data to the form
         */
        ASErr SendData() override;

        /**
         * \brief Set the flag that controlls the "focus" behavior of the form
         */
        void SetCloseOnFocus(const bool value);

        /**
         * @brief Define what the current action of the form is
         */
        enum class ActionType
        {
            none,
            create_item,
            edit_item
        };

        /**
         * @brief Return the current action type
         */
        ActionType GetActionType() const { return action_type_; }

       private:
        //! Current action type of the UI
        ActionType action_type_ = ActionType::none;

        //! Property of the last item change
        L2A::Property property_;

        //! Pointer to the item that is currently being edited
        std::unique_ptr<L2A::Item> change_item_;

        //! Position in the document where a new item shall be inserted
        AIRealPoint new_item_insertion_point_;
    };
}  // namespace L2A::UI
#endif
