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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace L2A.FORMS
{
    public partial class Redo : BaseForm
    {
        public Redo(L2A.UTIL.ParameterList parameter_list, string return_xml) : base(return_xml)
        {
            InitializeComponent();

            // Set the checkbox items.
            check_box_latex.Checked = false;
            check_box_boundary.Checked = true;
            all_items.Checked = true;
            selected_items.Checked = false;

            // Add number to form.
            string n_all_items = parameter_list.options_["n_all_items"];
            string n_selected_items = parameter_list.options_["n_selected_items"];
            all_items.Text = "All LaTeX2AI Items in the document (" + n_all_items + ")";
            selected_items.Text = "Selected LaTeX2AI Items (" + n_selected_items + ")";

            RedoCheckedChanged();
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

        public override void ThisFormClosed(object sender, FormClosedEventArgs e)
        {
            base.ThisFormClosed(sender, e);
        }

        private void RedoCheckedChanged(object sender, EventArgs e)
        {
            RedoCheckedChanged();
        }

        private void RedoCheckedChanged()
        {
            // If the item is deactivated, the cliped box is also deactivated.
            if (this.check_box_latex.Checked == true)
            {
                this.check_box_boundary.Checked = true;
                this.check_box_boundary.Enabled = false;
            }
            else
            {
                this.check_box_boundary.Enabled = true;
            }
        }

        protected override void StoreValues()
        {
            base.StoreValues();

            return_parameter_list_.options_["redo_latex"] = Convert.ToInt32(check_box_latex.Checked).ToString();
            return_parameter_list_.options_["redo_boundary"] = Convert.ToInt32(check_box_boundary.Checked).ToString();
            if (selected_items.Checked)
                return_parameter_list_.options_["item_type"] = "selected";
            else
                return_parameter_list_.options_["item_type"] = "all";
        }
    }
}
