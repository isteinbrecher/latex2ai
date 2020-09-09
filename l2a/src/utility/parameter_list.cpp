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
 * \brief XML related classes and functions.
 */


#include "IllustratorSDK.h"
#include "parameter_list.h"

#include "string_functions.h"
#include "l2a_error/l2a_error.h"

#include "../../tpl/tinyxml2/tinyxml2.h"


/**
 *
 */
L2A::UTIL::ParameterList::ParameterList(const ai::UnicodeString& string) : UTIL::ParameterList()
{
    // Parse the string into an xml document.
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError xml_error = xml_doc.Parse(string.as_Platform().c_str());
    if (tinyxml2::XML_SUCCESS != xml_error) l2a_error("XML could not be parsed.\nThe string was:\n\n" + string);

    // Get the root element of the xml. This is this item.
    const tinyxml2::XMLElement* xml_root = xml_doc.RootElement();
    SetFromXML(xml_root);
}

/**
 *
 */
L2A::UTIL::ParameterList::ParameterList(const tinyxml2::XMLElement* xml_element) : UTIL::ParameterList()
{
    SetFromXML(xml_element);
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetFromXML(const tinyxml2::XMLElement* xml_element)
{
    // Loop overchild elements.
    for (const tinyxml2::XMLElement* child = xml_element->FirstChildElement(); child != NULL;
         child = child->NextSiblingElement())
        SetSubList(ai::UnicodeString(child->Name()), std::make_shared<ParameterList>(child), true);

    // Loop over options and set them.
    for (const tinyxml2::XMLAttribute* attr = xml_element->FirstAttribute(); attr != NULL; attr = attr->Next())
    {
        // If the option string contains line breaks, the forms applications returns them as \r\n line breaks instead
        // of \n line breaks. This is replaced here.
        ai::UnicodeString option_string(attr->Value());
        L2A::UTIL::StringReplaceAll(option_string, ai::UnicodeString("\r\n"), ai::UnicodeString("\n"));
        SetOption(ai::UnicodeString(attr->Name()), ai::UnicodeString(option_string), true);
    }

    // Set main option.
    ai::UnicodeString xml_text(xml_element->GetText());
    if (0 != xml_text.compare(ai::UnicodeString(""))) SetMainOption(xml_text);
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetSubList(
    const ai::UnicodeString& key, const std::shared_ptr<ParameterList>& sub_list, const bool& fail_on_overwrite)
{
    if (main_option_set_) l2a_error("Sub lists can not be set if the main option is already set!");
    if (fail_on_overwrite)
        // Check if the key already exists.
        if (sub_lists_.count(key) != 0) l2a_error("Key \"" + key + "\" already exists in sub list map!");

    // Add the value.
    sub_lists_[key] = sub_list;
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetSubList(
    const ai::UnicodeString& key, const ParameterList& sub_list, const bool& fail_on_overwrite)
{
    // Copy the sublist and add a shared pointer to this parameter list.
    SetSubList(key, std::make_shared<UTIL::ParameterList>(sub_list), fail_on_overwrite);
}

/**
 *
 */
std::shared_ptr<L2A::UTIL::ParameterList> L2A::UTIL::ParameterList::SetSubList(
    const ai::UnicodeString& key, const bool& fail_on_overwrite)
{
    // Copy the sublist and add a shared pointer to this parameter list.
    std::shared_ptr<ParameterList> new_sub_list = std::make_shared<UTIL::ParameterList>();
    SetSubList(key, new_sub_list, fail_on_overwrite);
    return new_sub_list;
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetOption(
    const ai::UnicodeString& key, const ai::UnicodeString& value, const bool& fail_on_overwrite)
{
    if (fail_on_overwrite)
        // Check if the key already exists.
        if (options_map_.count(key) != 0) l2a_error("Key \"" + key + "\" already exists in option map!");

    // Add the value.
    options_map_[key] = value;
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetOption(const ai::UnicodeString& key, const int& value, const bool& fail_on_overwrite)
{
    SetOption(key, L2A::UTIL::IntegerToString(value), fail_on_overwrite);
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetOption(
    const ai::UnicodeString& key, const ai::FilePath& path, const bool& fail_on_overwrite)
{
    SetOption(key, path.GetFullPath(), fail_on_overwrite);
}

/**
 *
 */
void L2A::UTIL::ParameterList::SetMainOption(const ai::UnicodeString& value)
{
    // This is only possible if no subsets are set.
    if (sub_lists_.size() != 0) l2a_error("Main option can not be set if size if sub lists is not 0!");
    main_option_set_ = true;
    main_option_ = value;
};

/**
 *
 */
std::shared_ptr<const L2A::UTIL::ParameterList> L2A::UTIL::ParameterList::GetSubList(const ai::UnicodeString& key) const
{
    return GetSubListMutable(key);
}

/**
 *
 */
std::shared_ptr<L2A::UTIL::ParameterList> L2A::UTIL::ParameterList::GetSubListMutable(
    const ai::UnicodeString& key) const
{
    // If sub list exists return it.
    auto it = sub_lists_.find(key);
    if (it != sub_lists_.end())
        return it->second;
    else
    {
        ai::UnicodeString error_string("Key \"");
        error_string += key;
        error_string += ai::UnicodeString("\" not found in sub list map.\nExisting keys:");
        for (auto const& sub_list_it : sub_lists_)
        {
            error_string += ai::UnicodeString("\n    ");
            error_string += sub_list_it.first;
        }
        l2a_error(error_string);
    }
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::ParameterList::GetStringOption(const ai::UnicodeString& key) const
{
    // If item exists return it.
    auto it = options_map_.find(key);
    if (it != options_map_.end())
        return it->second;
    else
    {
        ai::UnicodeString error_string("Key \"");
        error_string += key;
        error_string += ai::UnicodeString("\" not found in option map.\nExisting keys:");
        for (auto const& parameters_it : options_map_)
        {
            error_string += ai::UnicodeString("\n    ");
            error_string += parameters_it.first;
        }
        l2a_error(error_string);
    }
}

/**
 *
 */
int L2A::UTIL::ParameterList::GetIntOption(const ai::UnicodeString& key) const
{
    return L2A::UTIL::StringToInteger(GetStringOption(key));
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::ParameterList::GetMainOption() const
{
    if (!main_option_set_) l2a_error("Main option is not set!");
    return main_option_;
}

/**
 *
 */
ai::UnicodeString L2A::UTIL::ParameterList::ToXMLString(const ai::UnicodeString& root_name) const
{
    // We need to populate an xml object with all data in this parameter list.
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLElement* root_element = xml_doc.NewElement(root_name.as_Platform().c_str());
    xml_doc.InsertFirstChild(root_element);

    // Fill in the contents of the parameter lists.
    ToXML(&xml_doc, root_element);

    // Return the string from the xml file.
    tinyxml2::XMLPrinter printer;
    xml_doc.Accept(&printer);
    return ai::UnicodeString(printer.CStr());
}

/**
 *
 */
bool L2A::UTIL::ParameterList::operator==(const L2A::UTIL::ParameterList& other) const
{
    //! Compare sublists.
    if (GetNumberOfSubList() != other.GetNumberOfSubList()) return false;
    for (auto const& sub_item : sub_lists_)
    {
        if (other.SubListExists(sub_item.first))
        {
            if (!(*(sub_item.second) == *(other.GetSubList(sub_item.first)))) return false;
        }
        else
            return false;
    }

    //! Compare options.
    if (GetNumberOfOptions() != other.GetNumberOfOptions()) return false;
    for (auto const& option_item : options_map_)
    {
        if (other.OptionExists(option_item.first))
        {
            if (option_item.second != other.GetStringOption(option_item.first)) return false;
        }
        else
            return false;
    }

    //! Compare main option.
    if (GetMainOptionSet() != other.GetMainOptionSet()) return false;
    if (GetMainOptionSet() && GetMainOption() != other.GetMainOption()) return false;

    // Everything worked up to this point, the lists are equal.
    return true;
}

/**
 *
 */
void L2A::UTIL::ParameterList::ToXML(tinyxml2::XMLDocument* xml_doc, tinyxml2::XMLElement* this_xml_element) const
{
    // Loop overchild elements and create them.
    for (auto const& sub_list_it : sub_lists_)
    {
        tinyxml2::XMLElement* xml_sub_list = xml_doc->NewElement(sub_list_it.first.as_Platform().c_str());
        sub_list_it.second->ToXML(xml_doc, xml_sub_list);
        this_xml_element->InsertEndChild(xml_sub_list);
    }

    // Loop over parameters.
    for (auto const& parameters_it : options_map_)
    {
        this_xml_element->SetAttribute(
            parameters_it.first.as_Platform().c_str(), parameters_it.second.as_Platform().c_str());
    }

    if (main_option_set_)
    {
        // Set main option.
        this_xml_element->SetText(main_option_.as_Platform().c_str());
    }
}

/**
 *
 */
bool L2A::UTIL::IsValidXML(const ai::UnicodeString& string)
{
    // Parse the string into an xml document.
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError xml_error = xml_doc.Parse(string.as_Platform().c_str());
    if (tinyxml2::XML_SUCCESS != xml_error)
        return false;
    else
        return true;
}
