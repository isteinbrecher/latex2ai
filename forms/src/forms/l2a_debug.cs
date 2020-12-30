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
    enum LatexCreationType { create, edit, redo_all };

    public partial class Debug : BaseForm
    {
        public Debug(L2A.UTIL.ParameterList parameter_list, string return_xml) : base(return_xml)
        {
            InitializeComponent();

            // Get the creation type.
            extra_label.Text = "";
            string creation_type = parameter_list.options_["creation_type"];
            if ("create" == creation_type)
            {
                button_cancel.Text = "Cancel item creation";
            }
            else if ("edit" == creation_type)
            {
                button_cancel.Text = "Cancel edit item";
            }
            else if ("redo_all" == creation_type)
            {
                extra_label.Text = "Could not create the combined Latex file for the redo mode." + Environment.NewLine + "This usually happens when something in the header changes " + Environment.NewLine + "or the document is compiled on a different system than before.";
                button_cancel.Text = "Continue";
                button_ok.Visible = false;
            }
            else
                L2A.ERR.ExceptionClass.Exception("The creation type \"" + creation_type + "\" is not defiend.");

            latex_log_file_ = parameter_list.options_["log_file"];
        }

        private void OkClick(object sender, EventArgs e)
        {
            this.form_result_ = "ok";
            this.StoreValues();
            this.Close();
        }

        private void CancelClick(object sender, EventArgs e)
        {
            this.Close();
        }

        private void DebugFolderClick(object sender, EventArgs e)
        {
            this.form_result_ = "ok";
            this.StoreValues();

            // Overwrite the default option for the creation of a debug folder.
            return_parameter_list_.options_["create_debug_folder"] = "1";

            this.Close();
        }

        protected override void StoreValues()
        {
            base.StoreValues();

            // Default values.
            return_parameter_list_.options_["create_debug_folder"] = "0";
        }

        public override void ThisFormClosed(object sender, FormClosedEventArgs e)
        {
            base.ThisFormClosed(sender, e);
        }

        private void OpenLog(object sender, EventArgs e)
        {
            Process.Start(latex_log_file_);
        }

        //! Path to LaTeX log file.
        string latex_log_file_;
    }
}
