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
        /**
         * @brief Constructor
         */
        Item();

        /**
         * @brief Load the item form
         */
        AIErr LoadForm();

        /**
         * @brief Create a new item at the given cursor position
         * TODO: Check return type of this method and error handling
         */
        void CreateNewItem(const AIRealPoint& position);

        /**
         * @brief Edit an existing item
         * TODO: Check return type of this method and error handling
         */
        void EditItem(const AIArtHandle& art_handle);

        /**
         * @brief Compile the data from the form to a new L2A item
         */
        void CompileNewItem(const L2A::UTIL::ParameterList& item_data_from_form);

        /**
         * @brief Edit an existing L2A item
         */
        void EditItem(const ai::UnicodeString& return_value, const L2A::UTIL::ParameterList& item_data_from_form);

        /**
         * @brief This function is called once the ui is loaded. we send the data to the ui here
         */
        static void ItemReady(const csxs::event::Event* const eventParam, void* const context);

        /**
         * @brief This function is called once the form is closed and we possibly need to recompile the latex item
         */
        static void CompileItemCallback(const csxs::event::Event* const eventParam, void* const context);

        /**
         * \brief Send data to the form
         */
        ASErr SendData() override;

        /**
         * \brief Close the form
         */
        void CloseForm();

       private:
        /**
         * @brief Define what the current action of the form is
         */
        enum class ActionType
        {
            none,
            create_item,
            edit_item
        };

        //! Current action type of the UI
        ActionType action_type_ = ActionType::none;

        //! Property of the last item change
        L2A::Property property_;

        //! Pointer to the item that is currently being edited
        std::unique_ptr<L2A::Item> change_item_;

        //! Position in the document where a new item shall be inserted
        AIRealPoint new_item_insertion_point_;
    };

    /**
     * @brief Create the form with the event listeners
     */
    std::unique_ptr<Item> ItemFactory();

}  // namespace L2A::UI
#endif
