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
 * \brief XML related classes and functions.
 */

#ifndef UTIL_XML_H_
#define UTIL_XML_H_


#include "IllustratorSDK.h"

#include <map>


// Forward declaration.
namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}  // namespace tinyxml2


namespace L2A
{
    namespace UTIL
    {
        /**
         * \brief Class to manage parameters. Each object can have multiple parameters stored with keys and one main
         * parameter. Also each object can have several sub lists stored with keys.
         */
        class ParameterList
        {
           public:
            /**
             * \brief Empty Constructor.
             */
            ParameterList()
            {
                main_option_set_ = false;
                sub_lists_.clear();
                options_map_.clear();
            };

            /**
             * \brief Create from XML string.
             */
            ParameterList(const ai::UnicodeString& xml_string);

            /**
             * \brief Constructor from XML object.
             */
            ParameterList(const tinyxml2::XMLElement* xml_element);

            /**
             * \brief Destructor.
             */
            ~ParameterList() {};

            /**
             * \brief Add a sublist to this option.
             *
             * @param key Name of the option. Capitalization matters.
             * @param sub_list Pointer to sub list.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            void SetSubList(const ai::UnicodeString& key, const std::shared_ptr<ParameterList>& sub_list,
                const bool& fail_on_overwrite = false);

            /**
             * \brief Add a sublist to this option.
             *
             * @param key Name of the option. Capitalization matters.
             * @param sub_list Sublist that will be copied into the sublist map.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            void SetSubList(
                const ai::UnicodeString& key, const ParameterList& sub_list, const bool& fail_on_overwrite = false);

            /**
             * \brief Add a new sublist to this option, and return a shared pointer to the new list.
             *
             * @param key Name of the option. Capitalization matters.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            std::shared_ptr<L2A::UTIL::ParameterList> SetSubList(
                const ai::UnicodeString& key, const bool& fail_on_overwrite = false);

            /**
             * \brief Add integer option.
             *
             * @param key Name of the option. Capitalization matters.
             * @param value Value of the option.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            void SetOption(
                const ai::UnicodeString& key, const ai::UnicodeString& value, const bool& fail_on_overwrite = false);

            /**
             * \brief Add integer option.
             *
             * @param key Name of the option. Capitalization matters.
             * @param value Value of the option.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            void SetOption(const ai::UnicodeString& key, const int& value, const bool& fail_on_overwrite = false);

            /**
             * \brief Add a file path to the options.
             *
             * @param key Name of the option. Capitalization matters.
             * @param path File path.
             * @param fail_on_overwrite If this is true an error will be thrown if the key already exists.
             */
            void SetOption(
                const ai::UnicodeString& key, const ai::FilePath& path, const bool& fail_on_overwrite = false);

            /**
             * \brief Set the main option for this parameter list. This is only possible if no sub lists have been
             * set.
             *
             * @param value Value of the option.
             */
            void SetMainOption(const ai::UnicodeString& value);

            /**
             * \brief Get number of sublists.
             */
            size_t GetNumberOfSubList() const { return sub_lists_.size(); }

            /**
             * \brief Get const sub list of this parameter list.
             */
            std::shared_ptr<const ParameterList> GetSubList(const ai::UnicodeString& key) const;

            /**
             * \brief Get mutable sub list of this parameter list.
             */
            std::shared_ptr<ParameterList> GetSubListMutable(const ai::UnicodeString& key) const;

            /**
             * \brief Check if a sublist with a certain key exists.
             */
            bool SubListExists(const ai::UnicodeString& key) const { return sub_lists_.find(key) != sub_lists_.end(); }

            /**
             * \brief Get number of options.
             */
            size_t GetNumberOfOptions() const { return options_map_.size(); }

            /**
             * \brief Get string option.
             */
            ai::UnicodeString GetStringOption(const ai::UnicodeString& key) const;

            /**
             * \brief Get integer option.
             */
            int GetIntOption(const ai::UnicodeString& key) const;

            /**
             * \brief Get main option.
             */
            ai::UnicodeString GetMainOption() const;

            /**
             * \brief Check if main option is set.
             */
            bool GetMainOptionSet() const { return main_option_set_; }

            /**
             * \brief Return a XML string representing all options in this object.
             */
            ai::UnicodeString ToXMLString(const ai::UnicodeString& root_name) const;

            /**
             * \brief Check if an option exists in this list.
             */
            bool OptionExists(const ai::UnicodeString& key) const
            {
                return options_map_.find(key) != options_map_.end();
            }

            /**
             * \brief Check if an option exists in this list, where multiple keys can possilby name this option.
             *
             * This can be used if the name of the option changed for some reason over time to achieve backwards
             * compability. If multiple keys match, we raise an error.
             *
             * The return values are a flag if a key was found and the corresponding found key.
             */
            std::pair<bool, ai::UnicodeString> OptionExistsMultipleKeys(
                const std::vector<ai::UnicodeString>& keys) const;

            /**
             * \brief Overload the == operator for a parameter list.
             */
            bool operator==(const ParameterList& other) const;

            /**
             * \brief Overload the != operator for a parameter list.
             */
            bool operator!=(const ParameterList& other) const { return !(*this == other); }

           private:
            /**
             * \brief Set values from XML object.
             */
            void SetFromXML(const tinyxml2::XMLElement* xml_element);

            /**
             * \brief Convert this parameter list and all its children to a xml document.
             */
            void ToXML(tinyxml2::XMLDocument* xml_doc, tinyxml2::XMLElement* this_xml_element) const;

            //! Sublists with options.
            std::map<ai::UnicodeString, std::shared_ptr<ParameterList>> sub_lists_;

            //! Map to store option names and their values.
            std::map<ai::UnicodeString, ai::UnicodeString> options_map_;

            //! Main option for this parameter list. If this main option is set, no sublists can be added to the
            //! parameter list.
            ai::UnicodeString main_option_;

            //! Flag if main option is set or not.
            bool main_option_set_;
        };

        /**
         * \brief Check if a string is a valid xml document.
         *
         * @param string (in) String with the xml text.
         * @return True if the xml could be parsed.
         */
        bool IsValidXML(const ai::UnicodeString& string);

    }  // namespace UTIL
}  // namespace L2A

#endif
