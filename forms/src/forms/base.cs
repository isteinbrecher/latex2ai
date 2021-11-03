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
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace L2A.FORMS
{
    public partial class BaseForm : Form
    {
        public BaseForm()
        {
            // This constructor is only here, so the forms editor in visual studio can show the derived forms.
        }

        public BaseForm(string return_xml) : base()
        {
            // Set the members and form items.
            return_xml_ = return_xml;

            // Default form result.
            form_result_ = "cancel";

            // Create an empty parameter list with values to return.
            return_parameter_list_ = new L2A.UTIL.ParameterList();

            // Set the icon.
            this.Icon = Icon.ExtractAssociatedIcon(Application.ExecutablePath);
        }

        public virtual void ThisFormClosed(object sender, FormClosedEventArgs e)
        {
            // When the form is closed, write the result xml.

            // Add the form result value.
            L2A.UTIL.ParameterList root_parameter_list = new L2A.UTIL.ParameterList();
            root_parameter_list.options_["form_result"] = form_result_;

            // If a option parameter list exits, add it.
            if (return_parameter_list_ != null)
                root_parameter_list.sub_lists_["LaTeX2AI_form_result"] = return_parameter_list_;

            // Write the parameter list to string.
            string xml_string = root_parameter_list.ToString("LaTeX2AI_form_result");
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(return_xml_, false))
            {
                file.WriteLine(xml_string);
            }
        }

        protected virtual void StoreValues() { }

        //! Parameter list with the options that should be returned.
        protected L2A.UTIL.ParameterList return_parameter_list_;

        //! Path to the xml to return.
        protected string return_xml_;

        //! String that will be put in the form result.
        protected string form_result_;

    }
}
