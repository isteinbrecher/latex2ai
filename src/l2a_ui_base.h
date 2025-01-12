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
 * \brief Base class for a UI form
 */

#ifndef L2A_UI_BASE_H_
#define L2A_UI_BASE_H_

#include "HtmlUIController.h"

#include "l2a_error.h"
#include "l2a_parameter_list.h"


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
        FormBase(const std::string& form_name, const char* form_extension_id, const std::string& event_name_base)
            : HtmlUIController(form_extension_id),
              form_name_(form_name),
              form_extension_id_(form_extension_id),
              event_name_base_(event_name_base) {};

        /**
         * @brief Load the form
         */
        AIErr LoadForm();

        /**
         * @brief Close the form
         */
        AIErr CloseForm();

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
        ASErr SendData() override { return kNoErr; };

        /**
         * @brief Wrapper for sending data to the UI
         */
        void SendDataWrapper(const std::shared_ptr<L2A::UTIL::ParameterList>& form_data, const std::string& event_name);

        /**
         * \brief Overload this abstract method
         */
        void ParseData(const char* eventData) override {};

        /**
         * @brief Reset internal data of the form that is not relevant after it is closed
         */
        virtual void ResetFormData() {};

       protected:
        //! Human readable name of the current form
        std::string form_name_;

        //! AI internal ID of the current form
        std::string form_extension_id_;

        //! Base name of events sent and received from this form
        std::string event_name_base_;

        //! Vector with all event listeners
        std::vector<EventListenerData> event_listener_data_;
    };

    /**
     * @brief Return a function object that handles callbacks and calls the correct method on the correct object
     */
    template <typename T, void (T::*CallbackMethod)(const csxs::event::Event* const)>
    csxs::event::EventListenerFn CallbackHandler()
    {
        return [](const csxs::event::Event* const event_parameter, void* const context)
        {
            // Get the correct form object from the function arguments
            auto form_object = (T*)context;

            // Make sure to catch errors, so we can close the form
            try
            {
                (form_object->*CallbackMethod)(event_parameter);
            }
            catch (L2A::ERR::Exception&)
            {
                sAIUser->MessageAlert(ai::UnicodeString("Error in CallbackHandler"));
                form_object->CloseForm();
            }
        };
    }
}  // namespace L2A::UI
#endif
