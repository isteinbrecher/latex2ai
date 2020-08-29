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

            // Check if arguments are given. Exactly 3 are expected.
            string[] arguments = Environment.GetCommandLineArgs();
            if (arguments.Length != 4) L2A.ERR.ExceptionClass.Exception("Exactly 3 input arguments are expected.");

            // Convert the xml to a parameter list.
            L2A.UTIL.ParameterList parameter_list = new L2A.UTIL.ParameterList(arguments[2]);

            // Get the path of the return xml file.
            string return_path = arguments[3];

            // Open the desired form.
            if (arguments[1] == "l2a_item")
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
    }
}
