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
 * \brief Base class for a UI form
 */

#ifndef L2A_UI_BASE_H_
#define L2A_UI_BASE_H_

#include "HtmlUIController.h"

#include "l2a_error.h"

namespace L2A::UI
{
    /**
     * @brief Container for event listener data
     */
    struct EventListenerData
    {
        //! Identifier for this event
        std::string event_id_;

        //! Function that will be called when this event is triggered
        const csxs::event::EventListenerFn event_listener_;

        //! Flag it the forms object should be passed as context
        bool has_context_;
    };

    /**
     * @brief Base class for a LaTeX2AI UI form
     */
    class FormBase : public HtmlUIController
    {
       public:
        /**
         * @brief Constructor
         */
        FormBase(const char* form_extension_id)
            : HtmlUIController(form_extension_id), form_extension_id_(form_extension_id){};

        /**
         * @brief Set the event listener data
         */
        void SetEventListeners(std::vector<EventListenerData>& event_listener_data);

        /**
         * \brief Registers the events this plug-in will listen for with PlugPlug
         */
        csxs::event::EventErrorCode RegisterCSXSEventListeners() override;

        /**
         * \brief Removes the previously added event listeners from PlugPlug
         */
        csxs::event::EventErrorCode RemoveEventListeners() override;

        /**
         * \brief Overload this abstract method
         */
        ASErr SendData() override { return 0; };

        /**
         * \brief Overload this abstract method
         */
        void ParseData(const char* eventData) override{};

       protected:
        //! Name of the current form
        std::string form_extension_id_;

       private:
        //! Vector with all event listeners
        std::vector<EventListenerData> event_listener_data_;
    };
}  // namespace L2A::UI
#endif
