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
 * \brief Create functions that interact with the forms application.
 */


#include "IllustratorSDK.h"
#include "l2a_forms.h"

#include "l2a_global/l2a_global.h"
#include "l2a_error/l2a_error.h"
#include "l2a_constants.h"
#include "utility/file_system.h"
#include "utility/parameter_list.h"
#include "utility/string_functions.h"


/**
 *
 */
L2A::FormReturnValue L2A::Form(const ai::UnicodeString& form_type, const L2A::UTIL::ParameterList& input_parameter_list,
    std::shared_ptr<L2A::UTIL::ParameterList>& return_parameter_list)
{
    L2A::FormReturnValue return_value;

    // If testing is activated, skip the actual form path and return the parameter list from the global object.
    if (L2A::Global().is_testing_)
    {
        return_parameter_list = L2A::Global().testing_form_return_parameter_list_;
        return L2A::Global().testing_form_return_value_;
    }

    // Get the filepath for the input and output files.
    ai::FilePath input_path = L2A::Global().path_temp_;
    input_path.AddComponent(ai::UnicodeString("l2a_forms_input.xml"));
    ai::FilePath output_path = L2A::Global().path_temp_;
    output_path.AddComponent(ai::UnicodeString("l2a_forms_output.xml"));

    // Delte the both files that are used to communicate with the forms.
    L2A::UTIL::RemoveFile(input_path, false);
    L2A::UTIL::RemoveFile(output_path, false);

    // Create input file.
    L2A::UTIL::WriteFileUTF8(input_path, input_parameter_list.ToXMLString(ai::UnicodeString("LaTeX2AI_form_input")));

    // Get the form command.
    ai::UnicodeString form_command_string("\"");
    form_command_string += L2A::Global().path_form_exe_.GetFullPath();
    form_command_string += "\" \"";
    form_command_string += form_type;
    form_command_string += "\" \"";
    form_command_string += input_path.GetFullPath();
    form_command_string += "\" \"";
    form_command_string += output_path.GetFullPath();
    form_command_string += "\" \"";
    form_command_string += L2A_VERSION_GIT_SHA_HEAD_;
    form_command_string += "\" \"";
#ifdef _DEBUG
    form_command_string += "debug";
#else
    form_command_string += "release";
#endif
    form_command_string += "\"";

    // Call the form.
    L2A::UTIL::ExecuteCommandLine(form_command_string);

    // Check if a return file was created.
    if (L2A::UTIL::IsFile(output_path))
    {
        // Load the return parameter list.
        L2A::UTIL::ParameterList form_parameter_list(L2A::UTIL::ReadFileUTF8(output_path));

        // Check if the form was canceled.
        ai::UnicodeString form_result = form_parameter_list.GetStringOption(ai::UnicodeString("form_result"));
        if (form_result == ai::UnicodeString("cancel"))
            return_value.canceled = true;
        else
        {
            return_parameter_list = form_parameter_list.GetSubListMutable(ai::UnicodeString("LaTeX2AI_form_result"));
            return_value.canceled = false;
        }

        return_value.return_string = form_result;
        return return_value;
    }
    else
        l2a_error("No file written in form application!");
}

/**
 *
 */
void L2A::MessageAlert(const ai::UnicodeString& message_string)
{
    if (!L2A::Global().is_testing_) sAIUser->MessageAlert(message_string);
}
