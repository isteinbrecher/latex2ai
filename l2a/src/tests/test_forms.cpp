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
 * \brief Test the communication with the forms application.
 */


#include "IllustratorSDK.h"
#include "test_forms.h"

#include "test_utlity.h"
#include "../utility/parameter_list.h"
#include "../l2a_forms/l2a_forms.h"


/**
 *
 */
void L2A::TEST::TestForms(L2A::TEST::UTIL::UnitTest& ut)
{
    // Set test name.
    ut.SetTestName(ai::UnicodeString("Forms"));

    // Generate a random integer.
    const int random_integer = std::rand();

    // Populate the parameter list.
    L2A::UTIL::ParameterList input_list;
    input_list.SetOption(ai::UnicodeString("key0"), random_integer);
    input_list.SetOption(ai::UnicodeString("key1"), random_integer);
    input_list.SetOption(ai::UnicodeString("key2"),
        ai::UnicodeString("Hier ist ein langer Text mit Umlauten ‰ˆƒ÷‹ﬂ@!\nund neue Zeilen\n\n\nthe end<>/''\"!\n"));

    // Add sub list.
    L2A::UTIL::ParameterList sub_list;
    sub_list.SetOption(ai::UnicodeString("subkey0"), random_integer);
    sub_list.SetOption(ai::UnicodeString("subkey1"), 666);
    ai::UnicodeString main_text("    \n    \n    \n    \nMainOptionText\n    \n    \n");
    sub_list.SetMainOption(main_text);
    input_list.SetSubList(ai::UnicodeString("sublist_key0"), sub_list);

    // Add second sub list.
    std::shared_ptr<L2A::UTIL::ParameterList> sub_list_2 = input_list.SetSubList(ai::UnicodeString("sublist_key1"));
    sub_list_2->SetMainOption(
        ai::UnicodeString("Hier ist ein langer Text mit Umlauten ‰ˆƒ÷‹ﬂ@!\nund neue Zeilen\n\n\nthe end<>/''\"!\n"));
    sub_list_2->SetOption(ai::UnicodeString("key0"), ai::UnicodeString("newvalue"));

    // Send to forms and read it again.
    std::shared_ptr<L2A::UTIL::ParameterList> form_return_parameter_list = std::make_shared<L2A::UTIL::ParameterList>();
    const bool form_return_value =
        L2A::Form(ai::UnicodeString("l2a_test_forms"), input_list, form_return_parameter_list);

    // Perform result checks.
    ut.CompareInt(true, form_return_value);

    ut.CompareStr(input_list.GetStringOption(ai::UnicodeString("key0")),
        form_return_parameter_list->GetStringOption(ai::UnicodeString("key0")));
    ut.CompareStr(input_list.GetStringOption(ai::UnicodeString("key1")),
        form_return_parameter_list->GetStringOption(ai::UnicodeString("key1")));
    ut.CompareStr(input_list.GetStringOption(ai::UnicodeString("key2")),
        form_return_parameter_list->GetStringOption(ai::UnicodeString("key2")));

    ut.CompareStr(
        input_list.GetSubList(ai::UnicodeString("sublist_key0"))->GetStringOption(ai::UnicodeString("subkey0")),
        form_return_parameter_list->GetSubList(ai::UnicodeString("sublist_key0"))
            ->GetStringOption(ai::UnicodeString("subkey0")));
    ut.CompareStr(
        input_list.GetSubList(ai::UnicodeString("sublist_key0"))->GetStringOption(ai::UnicodeString("subkey1")),
        form_return_parameter_list->GetSubList(ai::UnicodeString("sublist_key0"))
            ->GetStringOption(ai::UnicodeString("subkey1")));
    ut.CompareStr(input_list.GetSubList(ai::UnicodeString("sublist_key0"))->GetMainOption(),
        form_return_parameter_list->GetSubList(ai::UnicodeString("sublist_key0"))->GetMainOption());
}
