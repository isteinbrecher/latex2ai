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
 * \brief Errors for l2a applications.
 */


#include "IllustratorSDK.h"

#include "l2a_global.h"

#include "l2a_ai_functions.h"
#include "l2a_constants.h"
#include "l2a_error.h"
#include "l2a_execute.h"
#include "l2a_file_system.h"
#include "l2a_latex.h"
#include "l2a_parameter_list.h"
#include "l2a_plugin.h"
#include "l2a_string_functions.h"
#include "l2a_version.h"

/**
 * Set the global variables to a null pointer
 */
L2A::GLOBAL::Global* L2A::GLOBAL::_l2a_global = nullptr;
L2APlugin* L2A::GLOBAL::_l2a_plugin = nullptr;


/**
 *
 */
L2A::GLOBAL::Global::Global() : is_testing_(false)
{
    // Check if a new version of LaTeX2AI is available. Do this at the beginning in case there is an error in the set
    // and get path functions later on and it is fixed in a future release.
    L2A::UTIL::CheckGithubVersion();

    // In case something happens later on, set the default parameters, so we at least have sensible values set.
    std::shared_ptr<L2A::UTIL::ParameterList> default_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    GetDefaultParameterList(default_parameter_list);
    SetFromParameterList(*default_parameter_list);

    // Load the parameter list stored in the application data directory.
    {
        // Application data path.
        ai::FilePath application_data_directory = L2A::UTIL::GetApplicationDataDirectory();
        L2A::UTIL::CreateDirectoryL2A(application_data_directory);
        application_data_path_ = application_data_directory;
        application_data_path_.AddComponent(ai::UnicodeString("LaTeX2AI_application_data.xml"));

        //! File with last input for item.
        l2a_item_last_input_ = application_data_directory;
        l2a_item_last_input_.AddComponent(ai::UnicodeString("LaTeX2AI_last_input.xml"));

        if (L2A::UTIL::IsFile(application_data_path_))
        {
            // Try to load the data from the xml file.
            const ai::UnicodeString xml_string = L2A::UTIL::ReadFileUTF8(application_data_path_);
            try
            {
                L2A::UTIL::ParameterList data_list(xml_string);
                if (!SetFromParameterList(data_list))
                {
                    // Could not set all parameters -> warn the user about it.
                    L2A::AI::WarningAlert(ai::UnicodeString(
                        "Not all Plug-In settings of LaTeX2AI could be loaded. This can happen if a new version is "
                        "used. Please check if all options are set to your preferences."));
                }
            }
            catch (L2A::ERR::Exception&)
            {
                // Something went wrong, remove the application file and continue with the default values.
                L2A::UTIL::RemoveFile(application_data_path_);
            }
        }
    }

    // Clean the temporary directory.
    L2A::UTIL::ClearTemporaryDirectory();

    // We are now at a stage where we have the variables for gs and latex, either from the default parameters or from
    // the settings file. In either case we now do some basic checks if the paths are correct. If they are not we try to
    // find them automatically.
    {
        if (!L2A::LATEX::CheckGhostscriptCommand(gs_command_))
        {
            const auto [gs_command_ok, auto_gs_command] = L2A::LATEX::GetDefaultGhostScriptCommand();
            if (gs_command_ok)
            {
                gs_command_ = auto_gs_command;
            }
        }
        if (!L2A::LATEX::CheckLatexCommand(latex_bin_path_))
        {
            const auto [latex_bin_path_ok, auto_latex_bin_path] = L2A::LATEX::GetDefaultLatexPath();
            if (latex_bin_path_ok)
            {
                latex_bin_path_ = auto_latex_bin_path;
            }
        }
    }
}

/**
 *
 */
L2A::GLOBAL::Global::~Global()
{
    // Save the options in a file.
    L2A::UTIL::WriteFileUTF8(application_data_path_, ToString(), true);
}

/**
 *
 */
void L2A::GLOBAL::Global::ToParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const
{
    parameter_list->SetOption(ai::UnicodeString("latex_bin_path"), latex_bin_path_);
    parameter_list->SetOption(ai::UnicodeString("latex_engine"), latex_engine_);
    parameter_list->SetOption(ai::UnicodeString("latex_command_options"), latex_command_options_);
    parameter_list->SetOption(ai::UnicodeString("gs_command"), gs_command_);
    parameter_list->SetOption(ai::UnicodeString("item_ui_finish_on_enter"), item_ui_finish_on_enter_);
    parameter_list->SetOption(ai::UnicodeString("warning_boundary_boxes"), warning_boundary_boxes_);
    parameter_list->SetOption(ai::UnicodeString("warning_ai_not_saved"), warning_ai_not_saved_);
}

/**
 *
 */
void L2A::GLOBAL::Global::GetDefaultParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const
{
    parameter_list->SetOption(ai::UnicodeString("latex_bin_path"), ai::UnicodeString(""));
    parameter_list->SetOption(ai::UnicodeString("latex_engine"), ai::UnicodeString("pdflatex"));
    parameter_list->SetOption(ai::UnicodeString("latex_command_options"),
        ai::UnicodeString("-interaction nonstopmode -halt-on-error -file-line-error"));
    parameter_list->SetOption(ai::UnicodeString("gs_command"), ai::UnicodeString(""));
    parameter_list->SetOption(ai::UnicodeString("item_ui_finish_on_enter"), false);
    parameter_list->SetOption(ai::UnicodeString("warning_boundary_boxes"), true);
    parameter_list->SetOption(ai::UnicodeString("warning_ai_not_saved"), true);
}

/**
 *
 */
ai::UnicodeString L2A::GLOBAL::Global::ToString() const
{
    std::shared_ptr<L2A::UTIL::ParameterList> parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    ToParameterList(parameter_list);
    return parameter_list->ToXMLString(ai::UnicodeString("LaTeX2AI_options"));
}

/**
 *
 */
bool L2A::GLOBAL::Global::SetFromParameterList(const L2A::UTIL::ParameterList& parameter_list)
{
    // Function to convert the key from the parameter list to a file path
    auto conversion_file_path = [](const L2A::UTIL::ParameterList& parameter_list, const ai::UnicodeString& key)
    { return ai::FilePath(parameter_list.GetStringOption(key)); };

    // Function to convert the key from the parameter list to a bool
    auto conversion_bool = [](const L2A::UTIL::ParameterList& parameter_list, const ai::UnicodeString& key)
    { return bool(parameter_list.GetIntOption(key)); };

    // Function to set the variable from one of the possibly multiple given keys. If the key is in the parameter list
    // multiple times, an error will be thrown.
    auto set_variable_from_keys =
        [&](auto& variable, const std::vector<ai::UnicodeString>& keys, const bool set_all, auto conversion_function)
    {
        const auto [is_found, key] = parameter_list.OptionExistsMultipleKeys(keys);

        if (is_found)
        {
            variable = conversion_function(parameter_list, key);
            return set_all;
        }
        else
        {
            return false;
        }
    };

    // Overload of the previous function that uses the
    auto set_variable_from_keys_default =
        [&](auto& variable, const std::vector<ai::UnicodeString>& keys, const bool set_all)
    {
        return set_variable_from_keys(variable, keys, set_all,
            [](const L2A::UTIL::ParameterList& parameter_list, const ai::UnicodeString& key)
            { return parameter_list.GetStringOption(key); });
    };

    bool set_all = true;
    set_all = set_variable_from_keys(latex_bin_path_,
        {ai::UnicodeString("latex_bin_path"), ai::UnicodeString("path_latex")}, set_all, conversion_file_path);
    set_all = set_variable_from_keys_default(
        latex_engine_, {ai::UnicodeString("latex_engine"), ai::UnicodeString("command_latex")}, set_all);
    set_all = set_variable_from_keys_default(latex_command_options_,
        {ai::UnicodeString("latex_command_options"), ai::UnicodeString("command_latex_options")}, set_all);
    set_all = set_variable_from_keys_default(
        gs_command_, {ai::UnicodeString("gs_command"), ai::UnicodeString("command_gs")}, set_all);
    set_all = set_variable_from_keys(
        item_ui_finish_on_enter_, {ai::UnicodeString("item_ui_finish_on_enter")}, set_all, conversion_bool);
    set_all = set_variable_from_keys(
        warning_boundary_boxes_, {ai::UnicodeString("warning_boundary_boxes")}, set_all, conversion_bool);
    set_all = set_variable_from_keys(
        warning_ai_not_saved_, {ai::UnicodeString("warning_ai_not_saved")}, set_all, conversion_bool);

    return set_all;
}

/**
 *
 */
void L2A::GLOBAL::CheckGlobal()
{
    if (_l2a_global == nullptr) l2a_error("The global object is not defined!");
}

/**
 *
 */
const L2A::GLOBAL::Global& L2A::Global() { return L2A::GlobalMutable(); }

/**
 *
 */
L2A::GLOBAL::Global& L2A::GlobalMutable()
{
    L2A::GLOBAL::CheckGlobal();
    return *L2A::GLOBAL::_l2a_global;
}

/**
 *
 */
L2APlugin& L2A::GlobalPluginMutable()
{
    L2A::GLOBAL::CheckGlobal();
    return *L2A::GLOBAL::_l2a_plugin;
}

/**
 *
 */
AppContext L2A::GlobalPluginAppContext() { return AppContext(L2A::GlobalPluginMutable().GetPluginRef()); }
