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
using System.IO;


namespace L2A.FORMS
{
    public partial class Options : BaseForm
    {
        public Options(L2A.UTIL.ParameterList parameter_list, string return_xml) : base(return_xml)
        {
            InitializeComponent();

            // Add the available latex commands to the dropdown list.
            latex_command.Items.Add("pdflatex");
            latex_command.Items.Add("lualatex");
            latex_command.Items.Add("xelatex");

            // Store the default list.
            default_list_ = parameter_list.sub_lists_["default_options"];

            // Set the version and the git sha.
            version.Text = parameter_list.options_["version"];
            version.ReadOnly = true;
            version.BorderStyle = 0;
            version.BackColor = BackColor;
            version.TabStop = false;
            version.TextAlign = HorizontalAlignment.Right;

            git_sha.Text = parameter_list.options_["git_sha"];
            git_sha.ReadOnly = true;
            git_sha.BorderStyle = 0;
            git_sha.BackColor = BackColor;
            git_sha.TabStop = false;
            git_sha.TextAlign = HorizontalAlignment.Right;

            // Set the document header path.
            header_path_ = parameter_list.options_["document_header_path"];
            document_header.ReadOnly = true;
            document_header.BorderStyle = 0;
            document_header.BackColor = BackColor;
            document_header.TabStop = false;
            document_header.Enabled = false;
            if (header_path_ == "no_documents")
            {
                // In this case there are no open documents in Illustrator -> we can do nothing with the header.
                document_header.Text = "No active documents in Illustrator";
                document_header.ForeColor = Color.Gray;
                header_button.Visible = false;
            }
            else if (header_path_ == "not_saved")
            {
                // In this case the current opened document in Illustrator is not saved -> we can do nothing with the header.
                document_header.Text = "Current document not saved";
                document_header.ForeColor = Color.Gray;
                header_button.Visible = false;
            }
            else if (File.Exists(header_path_))
            {
                // In this case the document header exists -> allow the user to open it with the open header button.
                document_header.Text = header_path_;
                header_button.Text = "Open";
                document_header.Enabled = true;
            }
            else
            {
                // In this case the document header does not exists -> allow the user to create it with the header button.
                document_header.Text = "No header in the document directory";
                header_button.Text = "Create Default";
            }

            // Load the current options.
            SetFromParameterList(parameter_list);
        }

        private void SetFromParameterList(L2A.UTIL.ParameterList parameter_list)
        {
            // Set the latex path and options.
            latex_command.SelectedItem = parameter_list.options_["command_latex"];
            latex_path.Text = parameter_list.options_["path_latex"];
            latex_command_options.Text = parameter_list.options_["command_latex_options"];

            // Set the ghostscript path.
            gs_path.Text = parameter_list.options_["command_gs"];

            // Set the warning flags.
            check_box_save.Checked = Convert.ToBoolean(Int32.Parse(parameter_list.options_["warning_ai_not_saved"]));
            check_box_boundary.Checked = Convert.ToBoolean(Int32.Parse(parameter_list.options_["warning_boundary_boxes"]));
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

        private void HeaderClick(object sender, EventArgs e)
        {
            if (File.Exists(header_path_))
                // The header exists -> if the user clicks on the button open it in the system default editor.
                System.Diagnostics.Process.Start(header_path_);
            else
            {
                // The header does not exist and the button is active -> return to the plugin and create the header, then load this form again with the currently selected values.
                this.form_result_ = "create_default_header";
                this.StoreValues();
                this.Close();
            }
        }

        public override void ThisFormClosed(object sender, FormClosedEventArgs e)
        {
            base.ThisFormClosed(sender, e);
        }

        public void SelectLatexFolder(object sender, EventArgs e)
        {
            // Let the user select the path.
            folder_browser_dialog.SelectedPath = latex_path.Text;
            DialogResult result = folder_browser_dialog.ShowDialog();
            latex_path.Text = folder_browser_dialog.SelectedPath;
        }

        public void SelectGhostScriptFolder(object sender, EventArgs e)
        {
            // Let the user select the path.
            open_file_dialog.InitialDirectory = System.IO.Path.GetDirectoryName(gs_path.Text);
            open_file_dialog.FileName = System.IO.Path.GetFileName(gs_path.Text);
            DialogResult result = open_file_dialog.ShowDialog();
            if (DialogResult.OK == result)
                gs_path.Text = open_file_dialog.FileName;
        }

        public void SetDefaultParameters(object sender, EventArgs e)
        {
            SetFromParameterList(default_list_);
        }

        protected override void StoreValues()
        {
            base.StoreValues();

            return_parameter_list_.options_["path_latex"] = latex_path.Text;
            return_parameter_list_.options_["command_latex"] = latex_command.Text;
            return_parameter_list_.options_["command_latex_options"] = latex_command_options.Text;
            return_parameter_list_.options_["command_gs"] = gs_path.Text;
            return_parameter_list_.options_["warning_ai_not_saved"] = Convert.ToInt32(check_box_save.Checked).ToString();
            return_parameter_list_.options_["warning_boundary_boxes"] = Convert.ToInt32(check_box_boundary.Checked).ToString();
        }

        // Store the default parameters in case the user wants them.
        private L2A.UTIL.ParameterList default_list_;

        // Header path.
        private string header_path_;
    }
}
