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

#include "IllustratorSDK.h"

#include "l2a_ui_item.h"

#include "l2a_ai_functions.h"
#include "l2a_global.h"
#include "l2a_latex.h"
#include "l2a_parameter_list.h"
#include "l2a_string_functions.h"



// Define names for this form
const char* FORM_NAME = "com.adobe.illustrator.latex2aiui.dialog_item";
const char* EVENT_TYPE_ITEM_READY = "com.adobe.csxs.events.latex2ai.item.ready";
const char* EVENT_TYPE_ITEM_COMPILE = "com.adobe.csxs.events.latex2ai.item.compile";
const char* EVENT_TYPE_ITEM_CLOSE = "com.adobe.csxs.events.latex2ai.item.close";


/**
 *
 */
L2A::UI::Item::Item() : FormBase(FORM_NAME) {}

/**
 *
 */
AIErr L2A::UI::Item::LoadForm()
{
    PlugPlugErrorCode result = LoadExtension();
    if (result != PlugPlugErrorCode_success)
    {
        l2a_error("LaTeX2AI create/edit item UI could not be loaded");
        return kCantHappenErr;
    }
    else
    {
        return kNoErr;
    }
}

/**
 *
 */
void L2A::UI::Item::CreateNewItem(const AIRealPoint& position)
{
    // TODO:Check if document is saved

    action_type_ = ActionType::create_item;
    new_item_insertion_point_ = position;

    // Set the property to the last successfull newly created item value
    property_.SetFromLastInput();

    LoadForm();
}

/**
 *
 */
void L2A::UI::Item::EditItem(const AIArtHandle& art_handle)
{
    // TODO:Check if document is saved

    action_type_ = ActionType::edit_item;
    change_item_ = std::make_unique<L2A::Item>(art_handle);
    property_ = change_item_->GetProperty();

    // Load the item UI
    PlugPlugErrorCode result = LoadExtension();
    if (result != PlugPlugErrorCode_success)
    {
        l2a_error("LaTeX2AI create/edit item UI could not be loaded");
        return kCantHappenErr;
    }

    return kNoErr;
}

/**
 *
 */
void L2A::UI::Item::CompileNewItem(const L2A::UTIL::ParameterList& item_data_from_form)
{
    L2A::AI::SetUndoText(
        ai::UnicodeString("Undo Create LaTeX2AI Item"), ai::UnicodeString("Undo Create LaTeX2AI Item"));

    property_.SetFromParameterList(item_data_from_form);
    ai::FilePath pdf_file;
    if (L2A::LATEX::CreateLatexDocument(property_.GetLaTeXCode(), pdf_file))
    {
        // PDF could be created, now call GhostScript to split the pages. We only have a single page, but still need
        // to do this here, because otherwise the items will have a slightly different frame margin after Redo-All
        // is called.
        pdf_file = L2A::LATEX::SplitPdfPages(pdf_file, 1).at(0);

        // Add the PDF hash to the property
        property_.SetPDFFile(pdf_file);
        L2A::Item(new_item_insertion_point_, property_);

        // Everything worked fine, we can close the form now
        CloseForm();
    }
    else
    {
        // The pdf could not be created, ask the user how to proceed
        ai::UnicodeString form_string("The pdf file could not be created, do you want to re-edit the item?");
        AIBoolean form_result = sAIUser->OKCancelAlert(form_string, true, nullptr);
        if (form_result)
        {
            SendData();
        }
        else
        {
            // The user does not wan to re-edit the item, we can close the form now
            CloseForm();
        }
    }
}

/**
 *
 */
void L2A::UI::Item::EditItem(const ai::UnicodeString& return_value, const L2A::UTIL::ParameterList& item_data_from_form)
{
    L2A::AI::SetUndoText(
        ai::UnicodeString("Undo Change LaTeX2AI Item"), ai::UnicodeString("Redo Change LaTeX2AI Item"));

    property_.SetFromParameterList(item_data_from_form);
    auto change_item_result = change_item_->Change(return_value, property_);
    if (change_item_result == L2A::ItemChangeResult::ok or change_item_result == L2A::ItemChangeResult::cancel)
    {
        CloseForm();
        return;
    }
    else if (change_item_result == L2A::ItemChangeResult::get_new_user_input)
    {
        SendData();
        return;
    }
}

/**
 *
 */
void L2A::UI::Item::ItemReady(const csxs::event::Event* const eventParam, void* const context)
{
    auto item_form = (L2A::UI::Item*)context;
    item_form->SendData();
}

/**
 *
 */
void L2A::UI::Item::CompileItemCallback(const csxs::event::Event* const eventParam, void* const context)
{
    // Get the ui controller from the function arguments
    auto item_form = (L2A::UI::Item*)context;

    try
    {
        // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
        auto app_context = L2A::GlobalPluginAppContext();

        // Convert the return data to a parameter list
        L2A::UTIL::ParameterList form_return_data(L2A::UTIL::StringStdToAi(eventParam->data));

        if (item_form->action_type_ == ActionType::create_item)
        {
            item_form->CompileNewItem(*form_return_data.GetSubList(ai::UnicodeString("l2a_item")));
        }
        else if (item_form->action_type_ == ActionType::edit_item)
        {
            item_form->EditItem(form_return_data.GetStringOption(ai::UnicodeString("return_value")),
                *form_return_data.GetSubList(ai::UnicodeString("l2a_item")));
        }
        else
        {
            l2a_error("Got unexpected ActionType");
        }
    }
    catch (L2A::ERR::Exception&)
    {
        sAIUser->MessageAlert(ai::UnicodeString("Error in L2A::UI::ItemUI::CompileItemCallback"));
        item_form->CloseForm();
    }
}

/**
 *
 */
ASErr L2A::UI::Item::SendData()
{
    ASErr error = kNoErr;

    L2A::UTIL::ParameterList form_parameter_list;

    // Set data about the item not contained in the item property
    ai::UnicodeString key_latex("latex_exists");
    if (action_type_ == ActionType::create_item)
    {
        form_parameter_list.SetOption(key_latex, false);
    }
    else if (action_type_ == ActionType::edit_item)
    {
        ai::UnicodeString key_boundary_box("boundary_box_state");
        form_parameter_list.SetOption(key_latex, true);
        if (change_item_->IsDiamond())
        {
            form_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("diamond"));
        }
        else if (change_item_->IsStreched())
        {
            form_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("stretched"));
        }
        else
        {
            form_parameter_list.SetOption(key_boundary_box, ai::UnicodeString("ok"));
        }
    }
    else
        l2a_error("Got unexpected ActionType");

    // Add the item property
    form_parameter_list.SetSubList(ai::UnicodeString("LaTeX2AI_item"), property_.ToParameterList());

    // Get the string containing all data for the form and sent it
    std::string xml_string =
        L2A::UTIL::StringAiToStd(form_parameter_list.ToXMLString(ai::UnicodeString("full_item_data")));
    csxs::event::Event event = {"com.adobe.csxs.events.latex2ai.item.update", csxs::event::kEventScope_Application,
        "LaTeX2AI", NULL, xml_string.c_str()};
    csxs::event::EventErrorCode result = htmlPPLib.DispatchEvent(&event);
    if (result != csxs::event::kEventErrorCode_Success)
    {
        l2a_error("Data could not be sent to LaTeX2AI create/edit item UI");
        error = kCantHappenErr;
    }

    return error;
}

/**
 *
 */
void L2A::UI::Item::CloseForm()
{
    // Reset the state of the ui controller
    action_type_ = ActionType::none;

    // Send callback to close the form
    csxs::event::Event event = {EVENT_TYPE_ITEM_CLOSE, csxs::event::kEventScope_Application, "LaTeX2AI", NULL, ""};
    csxs::event::EventErrorCode result = htmlPPLib.DispatchEvent(&event);
    if (result != csxs::event::kEventErrorCode_Success)
    {
        l2a_error("Form could not be closed!");
    }
}

/**
 *
 */
std::unique_ptr<L2A::UI::Item> L2A::UI::ItemFactory()
{
    auto return_ptr = std::make_unique<Item>();
    std::vector<EventListenerData> event_listener_data = {//
        {EVENT_TYPE_ITEM_READY, Item::ItemReady, true},   //
        {EVENT_TYPE_ITEM_COMPILE, Item::CompileItemCallback, true}};
    return_ptr->SetEventListeners(event_listener_data);
    return return_ptr;
}
