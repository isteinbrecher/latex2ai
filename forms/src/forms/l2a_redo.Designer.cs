// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Ivo Steinbrecher (https://github.com/stoani89/LaTeX2AI)
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

﻿namespace L2A.FORMS
{
    partial class Redo
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.check_box_latex = new System.Windows.Forms.CheckBox();
            this.check_box_boundary = new System.Windows.Forms.CheckBox();
            this.button_ok = new System.Windows.Forms.Button();
            this.button_cancel = new System.Windows.Forms.Button();
            this.group_text = new System.Windows.Forms.GroupBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.selected_items = new System.Windows.Forms.RadioButton();
            this.all_items = new System.Windows.Forms.RadioButton();
            this.group_text.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // check_box_latex
            // 
            this.check_box_latex.AutoSize = true;
            this.check_box_latex.Location = new System.Drawing.Point(6, 19);
            this.check_box_latex.Name = "check_box_latex";
            this.check_box_latex.Size = new System.Drawing.Size(58, 17);
            this.check_box_latex.TabIndex = 1;
            this.check_box_latex.Text = "LaTeX";
            this.check_box_latex.UseVisualStyleBackColor = true;
            this.check_box_latex.Click += new System.EventHandler(this.RedoCheckedChanged);
            // 
            // check_box_boundary
            // 
            this.check_box_boundary.AutoSize = true;
            this.check_box_boundary.Location = new System.Drawing.Point(6, 44);
            this.check_box_boundary.Name = "check_box_boundary";
            this.check_box_boundary.Size = new System.Drawing.Size(92, 17);
            this.check_box_boundary.TabIndex = 2;
            this.check_box_boundary.Text = "Boundary Box";
            this.check_box_boundary.UseVisualStyleBackColor = true;
            this.check_box_boundary.Click += new System.EventHandler(this.RedoCheckedChanged);
            // 
            // button_ok
            // 
            this.button_ok.Location = new System.Drawing.Point(12, 95);
            this.button_ok.Name = "button_ok";
            this.button_ok.Size = new System.Drawing.Size(184, 23);
            this.button_ok.TabIndex = 3;
            this.button_ok.Text = "OK";
            this.button_ok.UseVisualStyleBackColor = true;
            this.button_ok.Click += new System.EventHandler(this.OkClick);
            // 
            // button_cancel
            // 
            this.button_cancel.Location = new System.Drawing.Point(202, 95);
            this.button_cancel.Name = "button_cancel";
            this.button_cancel.Size = new System.Drawing.Size(192, 23);
            this.button_cancel.TabIndex = 4;
            this.button_cancel.Text = "Cancel";
            this.button_cancel.UseVisualStyleBackColor = true;
            this.button_cancel.Click += new System.EventHandler(this.CancelClick);
            // 
            // group_text
            // 
            this.group_text.Controls.Add(this.check_box_latex);
            this.group_text.Controls.Add(this.check_box_boundary);
            this.group_text.Location = new System.Drawing.Point(12, 12);
            this.group_text.Name = "group_text";
            this.group_text.Size = new System.Drawing.Size(104, 71);
            this.group_text.TabIndex = 32;
            this.group_text.TabStop = false;
            this.group_text.Text = "Redo action";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.selected_items);
            this.groupBox1.Controls.Add(this.all_items);
            this.groupBox1.Location = new System.Drawing.Point(122, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(277, 71);
            this.groupBox1.TabIndex = 33;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "LaTeX2AI Items";
            // 
            // selected_items
            // 
            this.selected_items.AutoSize = true;
            this.selected_items.Checked = true;
            this.selected_items.Location = new System.Drawing.Point(6, 43);
            this.selected_items.Name = "selected_items";
            this.selected_items.Size = new System.Drawing.Size(85, 17);
            this.selected_items.TabIndex = 34;
            this.selected_items.TabStop = true;
            this.selected_items.Text = "radioButton1";
            this.selected_items.UseVisualStyleBackColor = true;
            // 
            // all_items
            // 
            this.all_items.AutoSize = true;
            this.all_items.Location = new System.Drawing.Point(6, 18);
            this.all_items.Name = "all_items";
            this.all_items.Size = new System.Drawing.Size(85, 17);
            this.all_items.TabIndex = 35;
            this.all_items.TabStop = true;
            this.all_items.Text = "radioButton2";
            this.all_items.UseVisualStyleBackColor = true;
            // 
            // Redo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(406, 131);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.group_text);
            this.Controls.Add(this.button_cancel);
            this.Controls.Add(this.button_ok);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Redo";
            this.Text = "Redo";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ThisFormClosed);
            this.group_text.ResumeLayout(false);
            this.group_text.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.CheckBox check_box_latex;
        private System.Windows.Forms.CheckBox check_box_boundary;
        private System.Windows.Forms.Button button_ok;
        private System.Windows.Forms.Button button_cancel;
        private System.Windows.Forms.GroupBox group_text;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton selected_items;
        private System.Windows.Forms.RadioButton all_items;
    }
}
