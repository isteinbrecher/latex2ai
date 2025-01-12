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

#include "IllustratorSDK.h"

#include "l2a_ui_redo.h"

#include "l2a_ai_functions.h"
#include "l2a_global.h"
#include "l2a_parameter_list.h"
#include "l2a_string_functions.h"


/**
 * \brief Set the names for item forms
 */
const std::string L2A::UI::Redo::FORM_NAME = "LaTeX2AI redo items";
const std::string L2A::UI::Redo::FORM_ID = "com.adobe.illustrator.latex2aiui.dialog_redo";
const std::string L2A::UI::Redo::EVENT_TYPE_BASE = "com.adobe.csxs.events.latex2ai.redo";
const std::string L2A::UI::Redo::EVENT_TYPE_READY = L2A::UI::Redo::EVENT_TYPE_BASE + ".ready";
const std::string L2A::UI::Redo::EVENT_TYPE_OK = L2A::UI::Redo::EVENT_TYPE_BASE + ".ok";
const std::string L2A::UI::Redo::EVENT_TYPE_UPDATE = L2A::UI::Redo::EVENT_TYPE_BASE + ".update";


/**
 *
 */
L2A::UI::Redo::Redo() : FormBase(FORM_NAME, FORM_ID.c_str(), EVENT_TYPE_BASE), all_items_(), selected_items_()
{
    // If we don't do this this way, we get a compiler error
    std::vector<EventListenerData> event_listener_data = {
        {EVENT_TYPE_READY, CallbackHandler<Redo, &Redo::CallbackFormReady>()},  //
        {EVENT_TYPE_OK, CallbackHandler<Redo, &Redo::CallbackOk>()}             //
    };
    event_listener_data_ = std::move(event_listener_data);
}

/**
 *
 */
void L2A::UI::Redo::ResetFormData()
{
    all_items_.clear();
    selected_items_.clear();
}

/**
 *
 */
void L2A::UI::Redo::OpenRedoForm() { LoadForm(); }


/**
 *
 */
void L2A::UI::Redo::CallbackFormReady(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();
    SendData();
}

/**
 *
 */
void L2A::UI::Redo::CallbackOk(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    // Convert the return data to a parameter list
    L2A::UTIL::ParameterList form_return_data(L2A::UTIL::StringStdToAi(eventParam->data));

    const auto& sub_form = form_return_data.GetSubList(ai::UnicodeString("l2a_redo"));
    const auto action_type = sub_form->GetStringOption(ai::UnicodeString("action_type"));
    const auto items = sub_form->GetStringOption(ai::UnicodeString("items"));

    L2A::RedoItemsOption redo_options;
    if (action_type == "latex")
        redo_options = L2A::RedoItemsOption::latex;
    else if (action_type == "bounding_box")
        redo_options = L2A::RedoItemsOption::bounding_box;
    else
        l2a_error("Unexpected return value in redo items");

    if (items == "all")
        L2A::RedoItems(all_items_, redo_options);
    else if (items == "selected")
        L2A::RedoItems(selected_items_, redo_options);
    else
        l2a_error("Unexpected return value in redo items");

    CloseForm();
}

/**
 *
 */
ASErr L2A::UI::Redo::SendData()
{
    // TODO: is this really needed?
    ASErr error = kNoErr;

    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    L2A::AI::GetDocumentItems(all_items_, L2A::AI::SelectionState::all);
    L2A::AI::GetDocumentItems(selected_items_, L2A::AI::SelectionState::selected);

    // Add number of items to parameter list for form
    auto redo_all_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    const unsigned int n_all_items = (unsigned int)all_items_.size();
    const unsigned int n_selected_items = (unsigned int)selected_items_.size();
    redo_all_parameter_list->SetOption(ai::UnicodeString("n_all_items"), n_all_items);
    redo_all_parameter_list->SetOption(ai::UnicodeString("n_selected_items"), n_selected_items);

    SendDataWrapper(redo_all_parameter_list, EVENT_TYPE_UPDATE);

    return error;
}
