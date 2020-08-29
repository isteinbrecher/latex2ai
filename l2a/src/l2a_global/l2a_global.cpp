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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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

#include "l2a_error/l2a_error.h"
#include "utility/file_system.h"
#include "utility/string_functions.h"
#include "utility/parameter_list.h"
#include "l2a_forms/l2a_forms.h"
#include "l2a_constants.h"


/**
 * Set the global variable to a null pointer.
 */
L2A::GLOBAL::Global* L2A::GLOBAL::_l2a_global = NULL;

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
void L2A::GLOBAL::Global::SetUp()
{
    // Get default parameter list.
    std::shared_ptr<L2A::UTIL::ParameterList> default_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    GetDefaultParameterList(default_parameter_list);

    // Get directories for this system.
    path_temp_ = L2A::UTIL::GetTemporaryDirectory();
    path_temp_.AddComponent(ai::UnicodeString("LaTeX2AI"));
    L2A::UTIL::CreateDirectoryL2A(path_temp_);

    // Application data path.
    application_data_directory_ = L2A::UTIL::GetApplicationDataDirectory();
    application_data_directory_.AddComponent(ai::UnicodeString("Adobe"));
    application_data_directory_.AddComponent(ai::UnicodeString("Illustrator"));
    application_data_directory_.AddComponent(ai::UnicodeString("LaTeX2AI"));
    L2A::UTIL::CreateDirectoryL2A(application_data_directory_);
    application_data_path_ = application_data_directory_;
    application_data_path_.AddComponent(ai::UnicodeString("LaTeX2AI_application_data.xml"));

    //! File with last input for item.
    l2a_item_last_input_ = application_data_directory_;
    l2a_item_last_input_.AddComponent(ai::UnicodeString("LaTeX2AI_last_input.xml"));

    // Load the data from the xml file.
    if (L2A::UTIL::IsFile(application_data_path_))
    {
        const ai::UnicodeString xml_string = L2A::UTIL::ReadFileUTF8(application_data_path_);
        if (L2A::UTIL::IsValidXML(xml_string))
            SetFromParameterList(xml_string);
        else
            L2A::UTIL::RemoveFile(application_data_path_);
    }

    // Check if the path to the forms exists, if not, try the default one. If this one can not be found, the function
    // returns false and the plug in will not be activated, as without the forms application, latex2ai currently does
    // not work.
    if (!L2A::UTIL::IsFile(path_form_exe_))
    {
        path_form_exe_ = L2A::UTIL::GetFormsPath();
        if (!L2A::UTIL::IsFile(path_form_exe_))
        {
            AIBoolean form_result = true;
            ai::UnicodeString form_string(
                "The path to the forms executable could not be found. Please select the path, otherwise LaTeX2AI can "
                "not be used!");
            form_result = sAIUser->OKCancelAlert(form_string, true, NULL);
            if (!form_result) return;

            // Ask the user to pick the file.
            ai::FilePath temp;
            AIFileDialogFilters options;
            options.AddFilter(ai::UnicodeString("Executable (*.exe)"), ai::UnicodeString("*.exe"));
            AIErr err =
                sAIUser->GetFileDialog(ai::UnicodeString("Select *.exe for the forms Application"), &options, temp);

            // Check if the form was canceled.
            if (err == kCanceledErr)
                return;
            else
                path_form_exe_ = temp;
        }
    }

    // Command paths.
    if (command_latex_ == ai::UnicodeString(""))
        command_latex_ = default_parameter_list->GetStringOption(ai::UnicodeString("command_latex"));
    if (command_gs_ == ai::UnicodeString(""))
        command_gs_ = default_parameter_list->GetStringOption(ai::UnicodeString("command_gs"));

    // Check that values were found.
    if (command_latex_ == ai::UnicodeString(""))
        throw L2A::ERR::Exception(ai::UnicodeString("Could not find command for LaTeX!"));
    if (command_gs_ == ai::UnicodeString(""))
        throw L2A::ERR::Exception(ai::UnicodeString("Could not find command for ghostscript!"));

    // Set default latex compiler options.
    if (command_latex_options_ == ai::UnicodeString(""))
        command_latex_options_ = default_parameter_list->GetStringOption(ai::UnicodeString("command_latex_options"));

    // Everything was ok.
    is_setup_ = true;
}

/**
 *
 */
void L2A::GLOBAL::Global::SetFromUserForm()
{
    // Get a parameter list with the current options and the default options.
    std::shared_ptr<L2A::UTIL::ParameterList> form_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    ToParameterList(form_parameter_list);
    std::shared_ptr<L2A::UTIL::ParameterList> default_list =
        form_parameter_list->SetSubList(ai::UnicodeString("default_options"));
    GetDefaultParameterList(default_list);
    form_parameter_list->SetOption(ai::UnicodeString("git_sha"), ai::UnicodeString(L2A_VERSION_GIT_SHA_HEAD_));

    // Call the form.
    std::shared_ptr<L2A::UTIL::ParameterList> form_return_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    if (L2A::Form(ai::UnicodeString("l2a_options"), *form_parameter_list, form_return_parameter_list))
        SetFromParameterList(*form_return_parameter_list);
}

/**
 *
 */
ai::UnicodeString L2A::GLOBAL::Global::GetLatexCommand() const
{
    if (L2A::UTIL::IsDirectory(path_latex_))
    {
        ai::FilePath exe_path = path_latex_;
        exe_path.AddComponent(command_latex_ + ".exe");
        return exe_path.GetFullPath();
    }
    else
        return command_latex_;
}

/**
 *
 */
void L2A::GLOBAL::Global::ToParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const
{
    parameter_list->SetOption(ai::UnicodeString("path_latex"), path_latex_);
    parameter_list->SetOption(ai::UnicodeString("command_latex"), command_latex_);
    parameter_list->SetOption(ai::UnicodeString("command_latex_options"), command_latex_options_);
    parameter_list->SetOption(ai::UnicodeString("command_gs"), command_gs_);
    parameter_list->SetOption(ai::UnicodeString("path_form_exe"), path_form_exe_);
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
void L2A::GLOBAL::Global::GetDefaultParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const
{
    parameter_list->SetOption(ai::UnicodeString("path_latex"), ai::UnicodeString(""));
    parameter_list->SetOption(ai::UnicodeString("command_latex"), ai::UnicodeString("pdflatex"));
    parameter_list->SetOption(ai::UnicodeString("command_latex_options"),
        ai::UnicodeString("-interaction nonstopmode -halt-on-error -file-line-error"));
    parameter_list->SetOption(ai::UnicodeString("command_gs"), L2A::UTIL::GetGhostScriptCommand());
    parameter_list->SetOption(ai::UnicodeString("path_form_exe"), ai::UnicodeString(""));
}

/**
 *
 */
void L2A::GLOBAL::Global::SetFromParameterList(const L2A::UTIL::ParameterList& parameter_list)
{
    path_latex_ = ai::FilePath(parameter_list.GetStringOption(ai::UnicodeString("path_latex")));
    command_latex_ = parameter_list.GetStringOption(ai::UnicodeString("command_latex"));
    command_latex_options_ = parameter_list.GetStringOption(ai::UnicodeString("command_latex_options"));
    command_gs_ = parameter_list.GetStringOption(ai::UnicodeString("command_gs"));
    if (parameter_list.OptionExists(ai::UnicodeString("path_form_exe")))
        path_form_exe_ = ai::FilePath(parameter_list.GetStringOption(ai::UnicodeString("path_form_exe")));
}

/**
 *
 */
void L2A::GLOBAL::CheckGlobal()
{
    if (_l2a_global == NULL)
        throw L2A::ERR::Exception(ai::UnicodeString("L2A::GLOBAL::CheckGlobal The global object is not defined!"));
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
