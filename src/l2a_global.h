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
 * \brief A global object for L2A to store parameters.
 */


#ifndef L2A_GLOBAL_H_
#define L2A_GLOBAL_H_


#include "AppContext.hpp"


// Forward declarations.
class L2APlugin;
namespace L2A
{
    namespace UTIL
    {
        class ParameterList;
    }
}  // namespace L2A


namespace L2A
{
    namespace GLOBAL
    {
        /**
         * \brief Class for the global object.
         */
        class Global
        {
           public:
            /**
             * \brief Empty constructor.
             */
            Global();

            /**
             * \brief Destructor. Here the options will be stored in an application file.
             */
            ~Global();

            /**
             * \brief Convert this object to a parameter list.
             */
            void ToParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const;

            /**
             * \brief Get a parameter list with the default options.
             */
            void GetDefaultParameterList(std::shared_ptr<L2A::UTIL::ParameterList>& parameter_list) const;

           private:
            /**
             * \brief Convert this object to a xml string.
             */
            ai::UnicodeString ToString() const;

            /**
             * \brief Set the values for this object from a parameter list.
             *
             * Return false if not all parameters could be set.
             */
            bool SetFromParameterList(const L2A::UTIL::ParameterList& parameter_list);

           public:
            //! File that stores global application data.
            ai::FilePath application_data_path_;

            //! File that stores last item input.
            ai::FilePath l2a_item_last_input_;

            //! Flag if testing is currently active.
            bool is_testing_;

            //! From here on are the "actual" options

            //! Path to the latex executables.
            ai::FilePath latex_bin_path_;

            //! Which latex engine to use.
            ai::UnicodeString latex_engine_;

            //! Options for the latex command.
            ai::UnicodeString latex_command_options_;

            //! Command for ghostscript in the shell.
            ai::UnicodeString gs_command_;

            //! Flag if item UI form can be finished by pressing Enter
            //! If this is false, it can be finished by pressing Shift+Enter
            bool item_ui_finish_on_enter_;

            //! Flag for warning if ai file is not saved.
            bool warning_ai_not_saved_;

            //! Flag for warning if boundary boxes are not OK.
            bool warning_boundary_boxes_;
        };

        /**
         * \brief Check if global object is set correctly
         */
        void CheckGlobal();

        /**
         * \brief Define global instance of Global, this object should not be used, only accessed via the Get
         * functions.
         */
        extern Global* _l2a_global;

        /**
         * \brief Define global pointer to the plugin, this object should not be used, only accessed via the Get
         * functions.
         */
        extern L2APlugin* _l2a_plugin;

    }  // namespace GLOBAL

    /**
     * \brief Return the global object.
     */
    const L2A::GLOBAL::Global& Global();

    /**
     * \brief Return the global object mutable.
     */
    L2A::GLOBAL::Global& GlobalMutable();

    /**
     * \brief Return the plugin pointer mutable.
     */
    L2APlugin& GlobalPluginMutable();

    /**
     * \brief Return the app context of the plugin
     */
    AppContext GlobalPluginAppContext();

}  // namespace L2A

#endif
