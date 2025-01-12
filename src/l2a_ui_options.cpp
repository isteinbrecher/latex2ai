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

#include "IllustratorSDK.h"

#include "l2a_ui_options.h"

#include "l2a_ai_functions.h"
#include "l2a_constants.h"
#include "l2a_execute.h"
#include "l2a_file_system.h"
#include "l2a_global.h"
#include "l2a_latex.h"
#include "l2a_parameter_list.h"
#include "l2a_string_functions.h"

/**
 * \brief Set the names for item forms
 */
const std::string L2A::UI::Options::FORM_NAME = "LaTeX2AI options";
const std::string L2A::UI::Options::FORM_ID = "com.adobe.illustrator.latex2aiui.dialog_options";
const std::string L2A::UI::Options::EVENT_TYPE_BASE = "com.adobe.csxs.events.latex2ai.options";
const std::string L2A::UI::Options::EVENT_TYPE_READY = L2A::UI::Options::EVENT_TYPE_BASE + ".ready";
const std::string L2A::UI::Options::EVENT_TYPE_SAVE = L2A::UI::Options::EVENT_TYPE_BASE + ".save";
const std::string L2A::UI::Options::EVENT_TYPE_UPDATE = L2A::UI::Options::EVENT_TYPE_BASE + ".update";
const std::string L2A::UI::Options::EVENT_TYPE_GET_DEFAULT_VALUES =
    L2A::UI::Options::EVENT_TYPE_BASE + ".get_default_values";
const std::string L2A::UI::Options::EVENT_TYPE_SELECT_GHOST_SCRIPT = L2A::UI::Options::EVENT_TYPE_BASE + ".select_gs";
const std::string L2A::UI::Options::EVENT_TYPE_SELECT_LATEX_BIN = L2A::UI::Options::EVENT_TYPE_BASE + ".select_tex";
const std::string L2A::UI::Options::EVENT_TYPE_OPEN_HEADER = L2A::UI::Options::EVENT_TYPE_BASE + ".open_header";
const std::string L2A::UI::Options::EVENT_TYPE_CREATE_DEFAULT_HEADER =
    L2A::UI::Options::EVENT_TYPE_BASE + ".create_default_header";

/**
 *
 */
L2A::UI::Options::Options() : FormBase(FORM_NAME, FORM_ID.c_str(), EVENT_TYPE_BASE)
{
    // If we don't do this this way, we get a compiler error
    std::vector<EventListenerData> event_listener_data = {
        {EVENT_TYPE_READY, CallbackHandler<Options, &Options::CallbackFormReady>()},                           //
        {EVENT_TYPE_SAVE, CallbackHandler<Options, &Options::CallbackSave>()},                                 //
        {EVENT_TYPE_GET_DEFAULT_VALUES, CallbackHandler<Options, &Options::CallbackGetDefaultValues>()},       //
        {EVENT_TYPE_SELECT_GHOST_SCRIPT, CallbackHandler<Options, &Options::CallbackSelectGhostScript>()},     //
        {EVENT_TYPE_SELECT_LATEX_BIN, CallbackHandler<Options, &Options::CallbackSelectLatex>()},              //
        {EVENT_TYPE_OPEN_HEADER, CallbackHandler<Options, &Options::CallbackOpenHeader>()},                    //
        {EVENT_TYPE_CREATE_DEFAULT_HEADER, CallbackHandler<Options, &Options::CallbackCreateDefaultHeader>()}  //
    };
    event_listener_data_ = std::move(event_listener_data);
}

/**
 *
 */
void L2A::UI::Options::OpenOptionsForm() { LoadForm(); }

/**
 *
 */
ASErr L2A::UI::Options::SendData(const std::shared_ptr<L2A::UTIL::ParameterList>& form_data)
{
    // TODO: is this really needed?
    ASErr error = kNoErr;

    SendDataWrapper(form_data, EVENT_TYPE_UPDATE);

    return error;
}

/**
 *
 */
void L2A::UI::Options::CallbackFormReady(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    // Setup the parameter list that will be send to the form
    auto form_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();

    // Get LaTeX2AI information
    auto latex_2ai_info = form_parameter_list->SetSubList(ai::UnicodeString("latex2ai_info"));
    latex_2ai_info->SetOption(ai::UnicodeString("version"), ai::UnicodeString(L2A_VERSION_STRING_));
    latex_2ai_info->SetOption(ai::UnicodeString("git_sha"), ai::UnicodeString(L2A_VERSION_GIT_SHA_HEAD_));

    // Get the global options as a parameter list and send it to the form
    auto global_options_parameter_list = form_parameter_list->SetSubList(ai::UnicodeString("latex2ai_options"));
    L2A::Global().ToParameterList(global_options_parameter_list);

    // Set the header data
    SetHeaderData(form_parameter_list);

    // Send data to form
    SendData(form_parameter_list);
}

/**
 *
 */
void L2A::UI::Options::CallbackSave(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    // Convert the return data to a parameter list
    const auto options_form = CallbackDataToParameterList(eventParam);

    // Store the data in the global object
    auto& global_mutable = L2A::GlobalMutable();
    global_mutable.latex_engine_ = options_form->GetStringOption(ai::UnicodeString("tex_engine"));
    global_mutable.latex_command_options_ = options_form->GetStringOption(ai::UnicodeString("latex_command_options"));
    global_mutable.latex_bin_path_ = ai::FilePath(options_form->GetStringOption(ai::UnicodeString("latex_bin_path")));
    global_mutable.gs_command_ = options_form->GetStringOption(ai::UnicodeString("gs_command"));
    global_mutable.item_ui_finish_on_enter_ =
        options_form->GetIntOption(ai::UnicodeString("item_ui_finish_on_enter")) == 1;
    global_mutable.warning_boundary_boxes_ =
        options_form->GetIntOption(ai::UnicodeString("warning_boundary_boxes")) == 1;
    global_mutable.warning_ai_not_saved_ = options_form->GetIntOption(ai::UnicodeString("warning_ai_not_saved")) == 1;

    CloseForm();
}

/**
 *
 */
void L2A::UI::Options::CallbackGetDefaultValues(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    auto default_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    auto default_options_parameter_list = default_parameter_list->SetSubList(ai::UnicodeString("latex2ai_options"));
    L2A::Global().GetDefaultParameterList(default_options_parameter_list);

    const auto [latex_bin_path_ok, auto_latex_bin_path] = L2A::LATEX::GetDefaultLatexPath();
    if (latex_bin_path_ok)
    {
        default_options_parameter_list->SetOption(ai::UnicodeString("latex_bin_path"), auto_latex_bin_path);
    }

    const auto [gs_command_ok, auto_gs_command] = L2A::LATEX::GetDefaultGhostScriptCommand();
    if (gs_command_ok)
    {
        default_options_parameter_list->SetOption(ai::UnicodeString("gs_command"), auto_gs_command);
    }

    SendData(default_parameter_list);
}

/**
 *
 */
void L2A::UI::Options::CallbackSelectGhostScript(const csxs::event::Event* const eventParam)
{
    // Convert the return data to a parameter list
    const auto options_form = CallbackDataToParameterList(eventParam);

    // Get the gs path currently set
    ai::FilePath gs_path(options_form->GetStringOption(ai::UnicodeString("gs_command")));

    // Open a window and ask the user to select the execuable
    AIFileDialogFilters options;
#ifdef WIN_ENV
    options.AddFilter(ai::UnicodeString("Executable (*.exe)"), ai::UnicodeString("*.exe"));
#endif
    AIErr error = sAIUser->GetFileDialog(ai::UnicodeString("Select Ghostscript executable"), &options, gs_path);
    if (error == 'STOP')
    {
        // The user canceled, so we can return here
        return;
    }
    l2a_check_ai_error(error);

    // Send the new ghostscript path back
    auto parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    auto l2a_options = parameter_list->SetSubList(ai::UnicodeString("latex2ai_options"));
    l2a_options->SetOption(ai::UnicodeString("gs_command"), gs_path);
    SendData(parameter_list);
}

/**
 *
 */
void L2A::UI::Options::CallbackSelectLatex(const csxs::event::Event* const eventParam)
{
    // Convert the return data to a parameter list
    const auto options_form = CallbackDataToParameterList(eventParam);

    // Get the gs path currently set
    ai::FilePath latex_bin_path(options_form->GetStringOption(ai::UnicodeString("latex_bin_path")));

    // Open a window and ask the user to select the bin path
    AIErr error = sAIUser->GetDirectoryDialog(ai::UnicodeString("Select LaTeX binary directory"), latex_bin_path);
    if (error == 'STOP')
    {
        // The user canceled, so we can return here
        return;
    }
    l2a_check_ai_error(error);

    // Send the new path back
    std::shared_ptr<L2A::UTIL::ParameterList> parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    auto l2a_options = parameter_list->SetSubList(ai::UnicodeString("latex2ai_options"));
    l2a_options->SetOption(ai::UnicodeString("latex_bin_path"), latex_bin_path);
    SendData(parameter_list);
}

/**
 *
 */
void L2A::UI::Options::CallbackOpenHeader(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    const bool create_default_if_not_exist = false;
    L2A::UTIL::OpenFileWithDefaultApplication(L2A::LATEX::GetHeaderPath(create_default_if_not_exist));
}

/**
 *
 */
void L2A::UI::Options::CallbackCreateDefaultHeader(const csxs::event::Event* const eventParam)
{
    // We need to activate the app context here, because otherwise functions like the GetDocumentName will not work
    auto app_context = L2A::GlobalPluginAppContext();

    const bool create_default_if_not_exist = true;
    L2A::LATEX::GetHeaderPath(create_default_if_not_exist);
    auto form_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    SetHeaderData(form_parameter_list);
    SendData(form_parameter_list);
}

/**
 *
 */
std::shared_ptr<L2A::UTIL::ParameterList> L2A::UI::Options::CallbackDataToParameterList(
    const csxs::event::Event* const eventParam)
{
    L2A::UTIL::ParameterList form_return_data(L2A::UTIL::StringStdToAi(eventParam->data));
    return form_return_data.GetSubListMutable(ai::UnicodeString("l2a_options"));
}

/**
 *
 */
void L2A::UI::Options::SetHeaderData(const std::shared_ptr<L2A::UTIL::ParameterList> form_parameter_list)
{
    // Check if the document is saved, i.e., if there is a possible header path
    auto header_parameter_list = form_parameter_list->SetSubList(ai::UnicodeString("document_header"));
    const ai::UnicodeString document_state_option_name("document_state");
    const ai::UnicodeString document_header_path_option_name("header_path");
    auto n_documents = L2A::AI::GetDocumentCount();
    if (n_documents > 0)
    {
        // Add the header path
        ai::FilePath document_path = L2A::UTIL::GetDocumentPath(false);
        if (L2A::UTIL::IsFile(document_path))
        {
            header_parameter_list->SetOption(document_state_option_name, ai::UnicodeString("saved"));

            ai::FilePath header_path = L2A::LATEX::GetHeaderPath(false);
            if (L2A::UTIL::IsFile(header_path))
            {
                header_parameter_list->SetOption(document_header_path_option_name, header_path);
            }
            else
            {
                header_parameter_list->SetOption(document_header_path_option_name, ai::UnicodeString(""));
            }
        }
        else
        {
            header_parameter_list->SetOption(document_state_option_name, ai::UnicodeString("not_saved"));
        }
    }
    else
    {
        header_parameter_list->SetOption(document_state_option_name, ai::UnicodeString("no_documents"));
    }
}
