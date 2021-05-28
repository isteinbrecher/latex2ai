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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace L2A.FORMS
{
    public partial class Item : BaseForm
    {
        public Item(L2A.UTIL.ParameterList parameter_list, string return_xml) : base(return_xml)
        {
            InitializeComponent();

            input_parameter_list_ = parameter_list;
            L2A.UTIL.ParameterList property_list = input_parameter_list_.sub_lists_["property_list"];

            // Set the position item.
            SetPosition(property_list.options_["text_align_horizontal"], property_list.options_["text_align_vertical"]);

            // Set the placement options.
            SetPlacementOption(property_list.options_["placed_option"]);

            // Set the latex code.
            textbox.Text = property_list.sub_lists_["latex"].main_option_;

            // Set the position of the cursor in the text box.
            int cursor_position = Int32.Parse(property_list.sub_lists_["latex"].options_["cursor_position"]);
            textbox.SelectionStart = cursor_position;
            textbox.SelectionLength = 0;

            // Set the textbox to be active.
            ActiveControl = textbox;

            // Set the redo boundary box relatex stuff in the form.
            string boundary_box_state = input_parameter_list_.options_["boundary_box_state"];
            if (boundary_box_state == "none")
                group_boundary_box.Visible = false;
            else
            {
                group_boundary_box.Visible = true;
                if (boundary_box_state == "ok")
                {
                    boundary_box_status.Text = "Ok";
                    rebo_boundary_box.Enabled = false;
                }
                else if (boundary_box_state == "streched")
                    boundary_box_status.Text = "Streched";
                else if (boundary_box_state == "diamond")
                    boundary_box_status.Text = "Bend";
            }

            // Set the redo latex related stuff.
            string latex_exists = input_parameter_list_.options_["latex_exists"];
            if (latex_exists == "0")
                group_latex.Visible = false;
            else
                group_latex.Visible = true;
        }

        private void SetPosition(string text_align_horizontal, string text_align_vertical)
        {
            if (text_align_horizontal == "left")
            {
                if (text_align_vertical == "top")
                    pos_0.Checked = true;
                else if (text_align_vertical == "centreV")
                    pos_3.Checked = true;
                else if (text_align_vertical == "baseline")
                    pos_6.Checked = true;
                else if (text_align_vertical == "bottom")
                    pos_9.Checked = true;
                else
                    L2A.ERR.ExceptionClass.Exception("Position could not be set!");
            }
            else if (text_align_horizontal == "centreH")
            {
                if (text_align_vertical == "top")
                    pos_1.Checked = true;
                else if (text_align_vertical == "centreV")
                    pos_4.Checked = true;
                else if (text_align_vertical == "baseline")
                    pos_7.Checked = true;
                else if (text_align_vertical == "bottom")
                    pos_10.Checked = true;
                else
                    L2A.ERR.ExceptionClass.Exception("Position could not be set!");
            }
            else if (text_align_horizontal == "right")
            {
                if (text_align_vertical == "top")
                    pos_2.Checked = true;
                else if (text_align_vertical == "centreV")
                    pos_5.Checked = true;
                else if (text_align_vertical == "baseline")
                    pos_8.Checked = true;
                else if (text_align_vertical == "bottom")
                    pos_11.Checked = true;
                else
                    L2A.ERR.ExceptionClass.Exception("Position could not be set!");
            }
            else
                L2A.ERR.ExceptionClass.Exception("Position could not be set!");

        }

        private void SetPlacementOption(string placement_option)
        {
            if (placement_option == "fill_to_boundary_box")
            {
                strech.Checked = true;
                clip_check_box.Checked = false;
                return;
            }
            else if (placement_option == "keep_scale")
            {
                original_size.Checked = true;
                clip_check_box.Checked = false;
                return;
            }
            else if (placement_option == "keep_scale_clip")
            {
                original_size.Checked = true;
                clip_check_box.Checked = true;
                return;
            }
            L2A.ERR.ExceptionClass.Exception("Placement could not be set!");
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

        private bool FormIsChanged()
        {
            StoreValues();
            L2A.UTIL.ParameterList property_list = input_parameter_list_.sub_lists_["property_list"];
            return return_parameter_list_.options_["text_align_horizontal"] != property_list.options_["text_align_horizontal"] ||
                return_parameter_list_.options_["text_align_vertical"] != property_list.options_["text_align_vertical"] ||
                return_parameter_list_.options_["placed_option"] != property_list.options_["placed_option"] ||
                return_parameter_list_.sub_lists_["latex"].main_option_ != property_list.sub_lists_["latex"].main_option_;
        }

        private void RedoBoundaryClick(object sender, EventArgs e)
        {
            if (FormIsChanged())
            {
                DialogResult dialog_result = MessageBox.Show("Some values of the item changed. If you continue with Redo Boundary Box the changes will be lost. Do you want to continue?", "LaTeX2AI", MessageBoxButtons.OKCancel);
                // The user wants to continue editing the form.
                if (dialog_result == DialogResult.Cancel) return;
            }

            this.form_result_ = "redo_boundary_box";
            this.Close();
        }

        private void RedoLaTeXClick(object sender, EventArgs e)
        {
            if (FormIsChanged())
            {
                DialogResult dialog_result = MessageBox.Show("Some values of the item changed. If you continue with Redo LaTeX the changes will be lost. Do you want to continue?", "LaTeX2AI", MessageBoxButtons.OKCancel);
                // The user wants to continue editing the form.
                if (dialog_result == DialogResult.Cancel) return;
            }

            this.form_result_ = "redo_latex";
            this.Close();
        }

        public override void ThisFormClosed(object sender, FormClosedEventArgs e)
        {
            base.ThisFormClosed(sender, e);
        }

        private void FormKeyDown(object sender, KeyEventArgs e)
        {
            // Close if escape is hit.
            if (e.KeyCode == Keys.Escape)
            {
                CancelClick(sender, e);
            }

            if (e.KeyCode == Keys.Enter)
            {
                // Check if shift key is down.
                if (Control.ModifierKeys == Keys.Shift)
                {
                    return;
                }
                else
                {
                    // Press ok when enter is hit.
                    OkClick(sender, e);
                }
            }
        }

        private void PlacementCheckedChanged(object sender, EventArgs e)
        {
            // If the item is deactivated, the cliped box is also deactivated.
            if (this.original_size.Checked == true)
            {
                this.clip_check_box.Enabled = true;
            }
            else
            {
                this.clip_check_box.Enabled = false;
            }
        }

        protected override void StoreValues()
        {
            base.StoreValues();

            // Set the placement options.
            if (strech.Checked == true)
                return_parameter_list_.options_["placed_option"] = "fill_to_boundary_box";
            else if (original_size.Checked == true && clip_check_box.Checked == false)
                return_parameter_list_.options_["placed_option"] = "keep_scale";
            else if (original_size.Checked == true && clip_check_box.Checked == true)
                return_parameter_list_.options_["placed_option"] = "keep_scale_clip";
            else L2A.ERR.ExceptionClass.Exception("Can not write placed options!");

            // Set the position options.
            if (pos_0.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "left";
                return_parameter_list_.options_["text_align_vertical"] = "top";
            }
            else if (pos_1.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "centreH";
                return_parameter_list_.options_["text_align_vertical"] = "top";
            }
            else if (pos_2.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "right";
                return_parameter_list_.options_["text_align_vertical"] = "top";
            }
            else if (pos_3.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "left";
                return_parameter_list_.options_["text_align_vertical"] = "centreV";
            }
            else if (pos_4.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "centreH";
                return_parameter_list_.options_["text_align_vertical"] = "centreV";
            }
            else if (pos_5.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "right";
                return_parameter_list_.options_["text_align_vertical"] = "centreV";
            }
            else if (pos_6.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "left";
                return_parameter_list_.options_["text_align_vertical"] = "baseline";
            }
            else if (pos_7.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "centreH";
                return_parameter_list_.options_["text_align_vertical"] = "baseline";
            }
            else if (pos_8.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "right";
                return_parameter_list_.options_["text_align_vertical"] = "baseline";
            }
            else if (pos_9.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "left";
                return_parameter_list_.options_["text_align_vertical"] = "bottom";
            }
            else if (pos_10.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "centreH";
                return_parameter_list_.options_["text_align_vertical"] = "bottom";
            }
            else if (pos_11.Checked == true)
            {
                return_parameter_list_.options_["text_align_horizontal"] = "right";
                return_parameter_list_.options_["text_align_vertical"] = "bottom";
            }
            else
                L2A.ERR.ExceptionClass.Exception("Can not write position!");

            // Set the latex text options.
            return_parameter_list_.sub_lists_["latex"] = new L2A.UTIL.ParameterList();
            return_parameter_list_.sub_lists_["latex"].main_option_ = textbox.Text;
            return_parameter_list_.sub_lists_["latex"].options_["cursor_position"] = textbox.SelectionStart.ToString();
        }

        //! Parameter list with the given options from the main application.
        protected L2A.UTIL.ParameterList input_parameter_list_;
    }
}
