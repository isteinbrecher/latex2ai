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
 * \brief Interact with the LaTeX2AI UI
 */

#ifndef L2A_UI_MANAGER_H_
#define L2A_UI_MANAGER_H_

#include "l2a_ui_base.h"
#include "l2a_ui_item.h"

#include <map>
#include <memory>

namespace L2A::UI
{
    /**
     * @brief Flags for all forms in LaTeX2AI
     */
    enum class FormsEnum
    {
        none_,
        item_
    };

    /**
     * @brief Class that manages all forms in LaTeX2AI
     */
    class Manager
    {
       public:
        /**
         * @brief Constructor
         */
        Manager();

        /**
         * @brief Register events of all forms
         */
        void RegisterCSXSEventListeners();

        /**
         * @brief Remove events of all forms
         */
        void RemoveEventListeners();

        /**
         * @brief Return pointer to the item form
         */
        Item& GetItemForm()
        {
            Item* form = dynamic_cast<Item*>(forms_[FormsEnum::item].get());
            return *(form);
        }

       private:
        //! Map containing all forms for LaTeX2AI
        std::map<FormsEnum, std::unique_ptr<FormBase>> forms_;
    };
}  // namespace L2A::UI
#endif
