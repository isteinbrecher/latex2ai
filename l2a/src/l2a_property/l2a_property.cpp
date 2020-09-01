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
 * \brief An object that stores all options for a l2a item.
 */


#include "IllustratorSDK.h"
#include "l2a_property.h"

#include "utility/parameter_list.h"
#include "utility/string_functions.h"
#include "utility/utils.h"
#include "utility/file_system.h"
#include "l2a_error/l2a_error.h"
#include "l2a_forms/l2a_forms.h"
#include "l2a_global/l2a_global.h"


/**
 *
 */
L2A::Property::Property()
{
    // Set default values.
    DefaultPropertyValues();
}

/**
 *
 */
void L2A::Property::DefaultPropertyValues()
{
    // Default values that every item has.

    // Text is centre / centre.
    text_align_horizontal_ = L2A::TextAlignHorizontal::centre;
    text_align_vertical_ = L2A::TextAlignVertical::centre;

    // Do not scale the image and keep it at the same size, also clip it.
    placed_method_ = L2A::PlacedArtMethod::keep_scale_clip;

    // Default latex code.
    latex_code_ = ai::UnicodeString("$a^2+b^2=c^2$");

    // Cursor position in the form.
    cursor_position_ = 0;
}

/**
 *
 */
void L2A::Property::SetFromParameterList(const L2A::UTIL::ParameterList& property_parameter_list)
{
    // Set the options.
    text_align_horizontal_ = L2A::UTIL::KeyToValue(TextAlignHorizontalStrings(), TextAlignHorizontalEnums(),
        property_parameter_list.GetStringOption(ai::UnicodeString("text_align_horizontal")));
    text_align_vertical_ = L2A::UTIL::KeyToValue(TextAlignVerticalStrings(), TextAlignVerticalEnums(),
        property_parameter_list.GetStringOption(ai::UnicodeString("text_align_vertical")));
    placed_method_ = L2A::UTIL::KeyToValue(PlacedArtMethodStrings(), PlacedArtMethodEnums(),
        property_parameter_list.GetStringOption(ai::UnicodeString("placed_option")));

    // Set the latex code.
    latex_code_ = property_parameter_list.GetSubList(ai::UnicodeString("latex"))->GetMainOption();

    // Cursor position.
    cursor_position_ = property_parameter_list.GetSubList(ai::UnicodeString("latex"))
                           ->GetIntOption(ai::UnicodeString("cursor_position"));
}

/**
 *
 */
void L2A::Property::SetFromString(const ai::UnicodeString& string)
{
    // Convert the string to an parameter list.
    L2A::UTIL::ParameterList property_parameter_list(string);
    SetFromParameterList(property_parameter_list);
}

/**
 *
 */
L2A::UTIL::ParameterList L2A::Property::ToParameterList() const
{
    // Create an ParameterList object and fill it up with the options.
    L2A::UTIL::ParameterList property_parameter_list;

    // Add the options.
    property_parameter_list.SetOption(ai::UnicodeString("text_align_horizontal"),
        L2A::UTIL::KeyToValue(TextAlignHorizontalEnums(), TextAlignHorizontalStrings(), text_align_horizontal_));
    property_parameter_list.SetOption(ai::UnicodeString("text_align_vertical"),
        L2A::UTIL::KeyToValue(TextAlignVerticalEnums(), TextAlignVerticalStrings(), text_align_vertical_));
    property_parameter_list.SetOption(ai::UnicodeString("placed_option"),
        L2A::UTIL::KeyToValue(PlacedArtMethodEnums(), PlacedArtMethodStrings(), placed_method_));

    // Add the latex text.
    std::shared_ptr<L2A::UTIL::ParameterList> tex_sub_list =
        property_parameter_list.SetSubList(ai::UnicodeString("latex"));
    tex_sub_list->SetMainOption(latex_code_);

    // Cursor position.
    tex_sub_list->SetOption(ai::UnicodeString("cursor_position"), cursor_position_);

    return property_parameter_list;
}

/**
 *
 */
ai::UnicodeString L2A::Property::ToString() const
{
    return ToParameterList().ToXMLString(ai::UnicodeString("LaTeX2AI_item"));
}

/**
 *
 */
L2A::PropertyCompare L2A::Property::Compare(const Property& other_property) const
{
    // Compare struct to be returned.
    PropertyCompare output;

    // Compare LaTeX_codes.
    if (0 != latex_code_.compare(other_property.latex_code_)) output.changed_latex = true;

    // Compare the placements.
    if (text_align_horizontal_ != other_property.text_align_horizontal_ ||
        text_align_vertical_ != other_property.text_align_vertical_)
    {
        output.changed_align = true;

        // Check if baseline changed.
        if (IsBaseline() != other_property.IsBaseline()) output.changed_latex = true;
    }

    // Compare the placement methods (and clip).
    if (placed_method_ != other_property.placed_method_) output.changed_placement = true;

    // Compare the cursor position.
    if (cursor_position_ != other_property.cursor_position_) output.changed_cursor = true;

    // Return the compare value.
    return output;
}

/**
 *
 */
bool L2A::Property::GetUserInput()
{
    // First get the paramete list from the forms application.
    std::shared_ptr<L2A::UTIL::ParameterList> new_parameter_list;
    const bool form_return_value = L2A::Form(ai::UnicodeString("l2a_item"), ToParameterList(), new_parameter_list);
    if (form_return_value)
    {
        // Set the new parameters for this item.
        SetFromParameterList(*new_parameter_list);
    }
    return form_return_value;
}

/**
 *
 */
ai::UnicodeString L2A::Property::GetLaTeXCode() const
{
    ai::UnicodeString string;

    // Add code according if item is baseline or not.
    if (this->IsBaseline())
        string = ai::UnicodeString("\\LaTeXtoAIbase{");
    else
        string = ai::UnicodeString("\\LaTeXtoAI{");
    string += this->latex_code_;
    string += ai::UnicodeString("}");

    return string;
}

/**
 *
 */
void L2A::Property::WriteLastInput() const
{
    L2A::UTIL::WriteFileUTF8(L2A::Global().l2a_item_last_input_, ToString(), true);
}

/**
 *
 */
void L2A::Property::SetFromLastInput()
{
    ai::FilePath last_input = L2A::Global().l2a_item_last_input_;
    if (L2A::UTIL::IsFile(last_input))
    {
        const ai::UnicodeString xml_string = L2A::UTIL::ReadFileUTF8(last_input);
        if (L2A::UTIL::IsValidXML(xml_string))
        {
            L2A::UTIL::ParameterList property_parameter_list(xml_string);
            SetFromParameterList(property_parameter_list);
        }
        else
            L2A::UTIL::RemoveFile(last_input);
    }
}
