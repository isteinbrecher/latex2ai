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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace L2A
{
    static class Program
    {
        /// <summary>
        /// Main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // Check if correct number of arguments are given.
            string[] arguments = Environment.GetCommandLineArgs();
            if (arguments.Length != 6) L2A.ERR.ExceptionClass.Exception("Exactly 6 input arguments are expected.");

            // Convert the xml to a parameter list.
            L2A.UTIL.ParameterList parameter_list = new L2A.UTIL.ParameterList(arguments[2]);

            // Check the git sha and the build type.
            if (arguments[4] != L2A.Constants.l2a_version_git_sha_head_)
                L2A.ERR.ExceptionClass.Exception(
                    "The git sha of the forms application: "
                    + L2A.Constants.l2a_version_git_sha_head_
                    + " does not match with the one from the LaTeX2AI plug-in: "
                    + arguments[4]);
#if DEBUG
            string build_type = "debug";
#else
            string build_type = "release";
#endif
            if (arguments[5] != build_type)
                L2A.ERR.ExceptionClass.Exception(
                    "The build types of the forms application: "
                    + build_type
                    + " does not match with the one from the LaTeX2AI plug-in: "
                    + arguments[5]);

            // Get the path of the return xml file.
            string return_path = arguments[3];

            // Open the desired form.
            if (arguments[1] == "l2a_test_forms")
            {
                TestForms(parameter_list, return_path);
            }
            else if (arguments[1] == "l2a_item")
            {
                Application.Run(new L2A.FORMS.Item(parameter_list, return_path));
            }
            else if (arguments[1] == "l2a_redo")
            {
                Application.Run(new L2A.FORMS.Redo(parameter_list, return_path));
            }
            else if (arguments[1] == "l2a_debug")
            {
                Application.Run(new L2A.FORMS.Debug(parameter_list, return_path));
            }
            else if (arguments[1] == "l2a_options")
            {
                Application.Run(new L2A.FORMS.Options(parameter_list, return_path));
            }
            else L2A.ERR.ExceptionClass.Exception("The form type \"" + arguments[1] + "\" is not defiend.");
        }

        /// <summary>
        /// Test the communication between the AI plug-in and the forms application.
        /// </summary>
        static private void TestForms(L2A.UTIL.ParameterList parameter_list, string return_xml)
        {
            var return_parameter_list = new L2A.UTIL.ParameterList();
            return_parameter_list.options_["form_result"] = "ok";
            return_parameter_list.sub_lists_["LaTeX2AI_form_result"] = parameter_list;

            // Write the parameter list to string.
            string xml_string = return_parameter_list.ToString("LaTeX2AI_form_result");
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(return_xml, false))
            {
                file.WriteLine(xml_string);
            }
        }
    }
}
