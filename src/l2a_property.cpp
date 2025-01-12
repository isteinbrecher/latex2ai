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
 * \brief An object that stores all options for a l2a item.
 */


#include "IllustratorSDK.h"

#include "l2a_property.h"

#include "l2a_constants.h"
#include "l2a_error.h"
#include "l2a_file_system.h"
#include "l2a_global.h"
#include "l2a_parameter_list.h"
#include "l2a_string_functions.h"
#include "l2a_utils.h"

/**
 *
 */
L2A::Property::Property() : version_(0)
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

    // Default latex code.
    latex_code_ = ai::UnicodeString("$a^2+b^2=c^2$");

    // Cursor position in the form.
    cursor_position_ = 0;

    // PDF file contents.
    pdf_file_encoded_ = ai::UnicodeString("");
    pdf_file_hash_ = ai::UnicodeString("");
    pdf_file_hash_method_ = HashMethod::none;
}

/**
 *
 */
void L2A::Property::SetFromParameterList(const L2A::UTIL::ParameterList& property_parameter_list)
{
    // Get the LaTeX2AI version information used to last create the property.
    std::string version_string;
    if (property_parameter_list.OptionExists(ai::UnicodeString("latex2ai_version")))
    {
        version_string =
            L2A::UTIL::StringAiToStd(property_parameter_list.GetStringOption(ai::UnicodeString("latex2ai_version")));
    }
    else
    {
        version_string = "0.0.0";
    }
    version_ = L2A::UTIL::ParseVersion(version_string);

    // Set the placement options
    text_align_horizontal_ = L2A::UTIL::KeyToValue(TextAlignHorizontalStrings(), TextAlignHorizontalEnums(),
        property_parameter_list.GetStringOption(ai::UnicodeString("text_align_horizontal")));
    text_align_vertical_ = L2A::UTIL::KeyToValue(TextAlignVerticalStrings(), TextAlignVerticalEnums(),
        property_parameter_list.GetStringOption(ai::UnicodeString("text_align_vertical")));

    // Set the latex code.
    latex_code_ = property_parameter_list.GetSubList(ai::UnicodeString("latex"))->GetMainOption();

    // Cursor position.
    cursor_position_ = property_parameter_list.GetSubList(ai::UnicodeString("latex"))
                           ->GetIntOption(ai::UnicodeString("cursor_position"));

    if (property_parameter_list.SubListExists(ai::UnicodeString("pdf_file_contents")))
    {
        const std::shared_ptr<const L2A::UTIL::ParameterList>& pdf_sub_list =
            property_parameter_list.GetSubList(ai::UnicodeString("pdf_file_contents"));
        pdf_file_encoded_ = pdf_sub_list->GetMainOption();
        pdf_file_hash_ = pdf_sub_list->GetStringOption(ai::UnicodeString("hash"));

        if (pdf_sub_list->OptionExists(ai::UnicodeString("hash_method")))
        {
            pdf_file_hash_method_ = L2A::UTIL::KeyToValue(HashMethodStrings(), HashMethodEnums(),
                pdf_sub_list->GetStringOption(ai::UnicodeString("hash_method")));
        }
        else
        {
            pdf_file_hash_method_ = HashMethod::none;
        }

        if (pdf_file_hash_method_ != HashMethod::crc64)
        {
            // The current hash method is crc64 if this is not the one that the has was created with, recalculate the
            // hash and set the hash method accordingly.
            pdf_file_hash_ = L2A::UTIL::StringHash(pdf_file_encoded_);
            pdf_file_hash_method_ = HashMethod::crc64;
        }
        else
        {
#ifdef _DEBUG
            // Safety check that the pdf hash is correct
            if (pdf_file_hash_ != L2A::UTIL::StringHash(pdf_file_encoded_))
                l2a_error("Hash and pdf contents do not match. This should not happen!");
#endif
        }
    }
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
L2A::UTIL::ParameterList L2A::Property::ToParameterList(const bool write_pdf_content) const
{
    // Create an ParameterList object and fill it up with the options.
    L2A::UTIL::ParameterList property_parameter_list;

    // Add the options.
    property_parameter_list.SetOption(ai::UnicodeString("text_align_horizontal"),
        L2A::UTIL::KeyToValue(TextAlignHorizontalEnums(), TextAlignHorizontalStrings(), text_align_horizontal_));
    property_parameter_list.SetOption(ai::UnicodeString("text_align_vertical"),
        L2A::UTIL::KeyToValue(TextAlignVerticalEnums(), TextAlignVerticalStrings(), text_align_vertical_));

    // Add the latex text.
    std::shared_ptr<L2A::UTIL::ParameterList> tex_sub_list =
        property_parameter_list.SetSubList(ai::UnicodeString("latex"));
    tex_sub_list->SetMainOption(latex_code_);

    // Cursor position.
    tex_sub_list->SetOption(ai::UnicodeString("cursor_position"), cursor_position_);

    if (write_pdf_content && !pdf_file_hash_.empty())
    {
        // Add the encoded pdf file to the parameter list.
        std::shared_ptr<L2A::UTIL::ParameterList> pdf_sub_list =
            property_parameter_list.SetSubList(ai::UnicodeString("pdf_file_contents"));
        pdf_sub_list->SetMainOption(pdf_file_encoded_);
        pdf_sub_list->SetOption(ai::UnicodeString("hash"), pdf_file_hash_, true);
        pdf_sub_list->SetOption(ai::UnicodeString("hash_method"),
            L2A::UTIL::KeyToValue(HashMethodEnums(), HashMethodStrings(), pdf_file_hash_method_));
    }

    // We add the current version, i.e., each time a property is saved to an item, we add the version of the plugin that
    // was used to create the item.
    property_parameter_list.SetOption(ai::UnicodeString("latex2ai_version"), ai::UnicodeString(L2A_VERSION_STRING_));

    return property_parameter_list;
}

/**
 *
 */
ai::UnicodeString L2A::Property::ToString(const bool write_pdf_content) const
{
    return ToParameterList(write_pdf_content).ToXMLString(ai::UnicodeString("LaTeX2AI_item"));
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

    // Compare the cursor position.
    if (cursor_position_ != other_property.cursor_position_) output.changed_cursor = true;

    // Return the compare value.
    return output;
}

/**
 *
 */
ai::UnicodeString L2A::Property::GetLaTeXCode() const { return this->latex_code_; }

/**
 *
 */
PlaceAlignment L2A::Property::GetAIAlignment() const
{
    return L2A::UTIL::KeyToValue(TextAlignPairs(), TextAlignEnumsAI(), {text_align_horizontal_, text_align_vertical_});
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
        // TODO: Replace this with a try where we simply check if the parameter list could be created from the xml
        if (L2A::UTIL::IsValidXML(xml_string))
        {
            L2A::UTIL::ParameterList property_parameter_list(xml_string);
            SetFromParameterList(property_parameter_list);
        }
        else
            L2A::UTIL::RemoveFile(last_input);
    }
}

/**
 *
 */
void L2A::Property::SetPDFFile(const ai::FilePath& pdf_file)
{
    // Encode the pdf file.
    pdf_file_encoded_ = ai::UnicodeString(L2A::UTIL::encode_file_base64(pdf_file));

    // Set the hash.
    pdf_file_hash_ = L2A::UTIL::StringHash(pdf_file_encoded_);
    pdf_file_hash_method_ = HashMethod::crc64;
}
