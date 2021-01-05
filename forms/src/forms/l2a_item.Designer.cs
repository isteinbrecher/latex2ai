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

namespace L2A.FORMS
{
    partial class Item
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
            this.button_cancel = new System.Windows.Forms.Button();
            this.button_ok = new System.Windows.Forms.Button();
            this.pos_11 = new System.Windows.Forms.RadioButton();
            this.pos_0 = new System.Windows.Forms.RadioButton();
            this.pos_10 = new System.Windows.Forms.RadioButton();
            this.group_placement = new System.Windows.Forms.GroupBox();
            this.pos_1 = new System.Windows.Forms.RadioButton();
            this.pos_9 = new System.Windows.Forms.RadioButton();
            this.pos_2 = new System.Windows.Forms.RadioButton();
            this.pos_8 = new System.Windows.Forms.RadioButton();
            this.pos_3 = new System.Windows.Forms.RadioButton();
            this.pos_7 = new System.Windows.Forms.RadioButton();
            this.pos_4 = new System.Windows.Forms.RadioButton();
            this.pos_6 = new System.Windows.Forms.RadioButton();
            this.pos_5 = new System.Windows.Forms.RadioButton();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.group_text = new System.Windows.Forms.GroupBox();
            this.textbox = new System.Windows.Forms.RichTextBox();
            this.clip_check_box = new System.Windows.Forms.CheckBox();
            this.original_size = new System.Windows.Forms.RadioButton();
            this.strech = new System.Windows.Forms.RadioButton();
            this.group_type = new System.Windows.Forms.GroupBox();
            this.rebo_boundary_box = new System.Windows.Forms.Button();
            this.group_boundary_box = new System.Windows.Forms.GroupBox();
            this.boundary_box_status = new System.Windows.Forms.Label();
            this.group_placement.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.group_text.SuspendLayout();
            this.group_type.SuspendLayout();
            this.group_boundary_box.SuspendLayout();
            this.SuspendLayout();
            // 
            // button_cancel
            // 
            this.button_cancel.Location = new System.Drawing.Point(576, 34);
            this.button_cancel.Name = "button_cancel";
            this.button_cancel.Size = new System.Drawing.Size(75, 23);
            this.button_cancel.TabIndex = 29;
            this.button_cancel.Text = "Cancel";
            this.button_cancel.UseVisualStyleBackColor = true;
            this.button_cancel.Click += new System.EventHandler(this.CancelClick);
            // 
            // button_ok
            // 
            this.button_ok.Location = new System.Drawing.Point(576, 5);
            this.button_ok.Name = "button_ok";
            this.button_ok.Size = new System.Drawing.Size(75, 23);
            this.button_ok.TabIndex = 28;
            this.button_ok.Text = "OK";
            this.button_ok.UseVisualStyleBackColor = true;
            this.button_ok.Click += new System.EventHandler(this.OkClick);
            // 
            // pos_11
            // 
            this.pos_11.AutoSize = true;
            this.pos_11.Location = new System.Drawing.Point(140, 128);
            this.pos_11.Name = "pos_11";
            this.pos_11.Size = new System.Drawing.Size(14, 13);
            this.pos_11.TabIndex = 14;
            this.pos_11.TabStop = true;
            this.pos_11.UseVisualStyleBackColor = true;
            // 
            // pos_0
            // 
            this.pos_0.AutoSize = true;
            this.pos_0.Location = new System.Drawing.Point(6, 19);
            this.pos_0.Name = "pos_0";
            this.pos_0.Size = new System.Drawing.Size(14, 13);
            this.pos_0.TabIndex = 3;
            this.pos_0.TabStop = true;
            this.pos_0.UseVisualStyleBackColor = true;
            // 
            // pos_10
            // 
            this.pos_10.AutoSize = true;
            this.pos_10.Location = new System.Drawing.Point(71, 128);
            this.pos_10.Name = "pos_10";
            this.pos_10.Size = new System.Drawing.Size(14, 13);
            this.pos_10.TabIndex = 13;
            this.pos_10.TabStop = true;
            this.pos_10.UseVisualStyleBackColor = true;
            // 
            // group_placement
            // 
            this.group_placement.Controls.Add(this.pos_11);
            this.group_placement.Controls.Add(this.pos_0);
            this.group_placement.Controls.Add(this.pos_10);
            this.group_placement.Controls.Add(this.pos_1);
            this.group_placement.Controls.Add(this.pos_9);
            this.group_placement.Controls.Add(this.pos_2);
            this.group_placement.Controls.Add(this.pos_8);
            this.group_placement.Controls.Add(this.pos_3);
            this.group_placement.Controls.Add(this.pos_7);
            this.group_placement.Controls.Add(this.pos_4);
            this.group_placement.Controls.Add(this.pos_6);
            this.group_placement.Controls.Add(this.pos_5);
            this.group_placement.Controls.Add(this.pictureBox1);
            this.group_placement.Location = new System.Drawing.Point(12, 63);
            this.group_placement.Name = "group_placement";
            this.group_placement.Size = new System.Drawing.Size(160, 147);
            this.group_placement.TabIndex = 30;
            this.group_placement.TabStop = false;
            this.group_placement.Text = "Placement";
            // 
            // pos_1
            // 
            this.pos_1.AutoSize = true;
            this.pos_1.Location = new System.Drawing.Point(71, 19);
            this.pos_1.Name = "pos_1";
            this.pos_1.Size = new System.Drawing.Size(14, 13);
            this.pos_1.TabIndex = 4;
            this.pos_1.TabStop = true;
            this.pos_1.UseVisualStyleBackColor = true;
            // 
            // pos_9
            // 
            this.pos_9.AutoSize = true;
            this.pos_9.Location = new System.Drawing.Point(6, 128);
            this.pos_9.Name = "pos_9";
            this.pos_9.Size = new System.Drawing.Size(14, 13);
            this.pos_9.TabIndex = 12;
            this.pos_9.TabStop = true;
            this.pos_9.UseVisualStyleBackColor = true;
            // 
            // pos_2
            // 
            this.pos_2.AutoSize = true;
            this.pos_2.Location = new System.Drawing.Point(140, 19);
            this.pos_2.Name = "pos_2";
            this.pos_2.Size = new System.Drawing.Size(14, 13);
            this.pos_2.TabIndex = 5;
            this.pos_2.TabStop = true;
            this.pos_2.UseVisualStyleBackColor = true;
            // 
            // pos_8
            // 
            this.pos_8.AutoSize = true;
            this.pos_8.Location = new System.Drawing.Point(140, 97);
            this.pos_8.Name = "pos_8";
            this.pos_8.Size = new System.Drawing.Size(14, 13);
            this.pos_8.TabIndex = 11;
            this.pos_8.TabStop = true;
            this.pos_8.UseVisualStyleBackColor = true;
            // 
            // pos_3
            // 
            this.pos_3.AutoSize = true;
            this.pos_3.Location = new System.Drawing.Point(6, 76);
            this.pos_3.Name = "pos_3";
            this.pos_3.Size = new System.Drawing.Size(14, 13);
            this.pos_3.TabIndex = 6;
            this.pos_3.TabStop = true;
            this.pos_3.UseVisualStyleBackColor = true;
            // 
            // pos_7
            // 
            this.pos_7.AutoSize = true;
            this.pos_7.Location = new System.Drawing.Point(71, 97);
            this.pos_7.Name = "pos_7";
            this.pos_7.Size = new System.Drawing.Size(14, 13);
            this.pos_7.TabIndex = 10;
            this.pos_7.TabStop = true;
            this.pos_7.UseVisualStyleBackColor = true;
            // 
            // pos_4
            // 
            this.pos_4.AutoSize = true;
            this.pos_4.Location = new System.Drawing.Point(71, 76);
            this.pos_4.Name = "pos_4";
            this.pos_4.Size = new System.Drawing.Size(14, 13);
            this.pos_4.TabIndex = 7;
            this.pos_4.TabStop = true;
            this.pos_4.UseVisualStyleBackColor = true;
            // 
            // pos_6
            // 
            this.pos_6.AutoSize = true;
            this.pos_6.Location = new System.Drawing.Point(6, 97);
            this.pos_6.Name = "pos_6";
            this.pos_6.Size = new System.Drawing.Size(14, 13);
            this.pos_6.TabIndex = 9;
            this.pos_6.TabStop = true;
            this.pos_6.UseVisualStyleBackColor = true;
            // 
            // pos_5
            // 
            this.pos_5.AutoSize = true;
            this.pos_5.Location = new System.Drawing.Point(140, 76);
            this.pos_5.Name = "pos_5";
            this.pos_5.Size = new System.Drawing.Size(14, 13);
            this.pos_5.TabIndex = 8;
            this.pos_5.TabStop = true;
            this.pos_5.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.ErrorImage = null;
            this.pictureBox1.Image = global::LaTeX2AIForms.Properties.Resources.LaTeX2AI_font_placement;
            this.pictureBox1.InitialImage = null;
            this.pictureBox1.Location = new System.Drawing.Point(6, 19);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(148, 122);
            this.pictureBox1.TabIndex = 15;
            this.pictureBox1.TabStop = false;
            // 
            // group_text
            // 
            this.group_text.Controls.Add(this.textbox);
            this.group_text.Location = new System.Drawing.Point(178, 63);
            this.group_text.Name = "group_text";
            this.group_text.Size = new System.Drawing.Size(479, 147);
            this.group_text.TabIndex = 31;
            this.group_text.TabStop = false;
            this.group_text.Text = "LaTeX code - for newline press SHIFT+ENTER";
            // 
            // textbox
            // 
            this.textbox.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textbox.Location = new System.Drawing.Point(6, 19);
            this.textbox.Name = "textbox";
            this.textbox.Size = new System.Drawing.Size(467, 122);
            this.textbox.TabIndex = 2;
            this.textbox.Text = "";
            this.textbox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FormKeyDown);
            // 
            // clip_check_box
            // 
            this.clip_check_box.AutoSize = true;
            this.clip_check_box.Location = new System.Drawing.Point(218, 18);
            this.clip_check_box.Name = "clip_check_box";
            this.clip_check_box.Size = new System.Drawing.Size(42, 17);
            this.clip_check_box.TabIndex = 21;
            this.clip_check_box.Text = "clip";
            this.clip_check_box.UseVisualStyleBackColor = true;
            // 
            // original_size
            // 
            this.original_size.AutoSize = true;
            this.original_size.Location = new System.Drawing.Point(99, 18);
            this.original_size.Name = "original_size";
            this.original_size.Size = new System.Drawing.Size(113, 17);
            this.original_size.TabIndex = 19;
            this.original_size.Text = "remain original size";
            this.original_size.UseVisualStyleBackColor = true;
            this.original_size.CheckedChanged += new System.EventHandler(this.PlacementCheckedChanged);
            // 
            // strech
            // 
            this.strech.AutoSize = true;
            this.strech.Location = new System.Drawing.Point(19, 17);
            this.strech.Name = "strech";
            this.strech.Size = new System.Drawing.Size(54, 17);
            this.strech.TabIndex = 20;
            this.strech.Text = "strech";
            this.strech.UseVisualStyleBackColor = true;
            this.strech.CheckedChanged += new System.EventHandler(this.PlacementCheckedChanged);
            // 
            // group_type
            // 
            this.group_type.Controls.Add(this.clip_check_box);
            this.group_type.Controls.Add(this.original_size);
            this.group_type.Controls.Add(this.strech);
            this.group_type.Location = new System.Drawing.Point(12, 12);
            this.group_type.Name = "group_type";
            this.group_type.Size = new System.Drawing.Size(284, 45);
            this.group_type.TabIndex = 32;
            this.group_type.TabStop = false;
            this.group_type.Text = "Select behaviour of the item when it is streched";
            // 
            // rebo_boundary_box
            // 
            this.rebo_boundary_box.Location = new System.Drawing.Point(130, 14);
            this.rebo_boundary_box.Name = "rebo_boundary_box";
            this.rebo_boundary_box.Size = new System.Drawing.Size(125, 23);
            this.rebo_boundary_box.TabIndex = 33;
            this.rebo_boundary_box.Text = "Redo Boundary Box";
            this.rebo_boundary_box.UseVisualStyleBackColor = true;
            this.rebo_boundary_box.Click += new System.EventHandler(this.RedoBoundaryClick);
            // 
            // group_boundary_box
            // 
            this.group_boundary_box.Controls.Add(this.boundary_box_status);
            this.group_boundary_box.Controls.Add(this.rebo_boundary_box);
            this.group_boundary_box.Location = new System.Drawing.Point(302, 12);
            this.group_boundary_box.Name = "group_boundary_box";
            this.group_boundary_box.Size = new System.Drawing.Size(261, 45);
            this.group_boundary_box.TabIndex = 33;
            this.group_boundary_box.TabStop = false;
            this.group_boundary_box.Text = "Boundary Box";
            this.group_boundary_box.Visible = false;
            // 
            // boundary_box_status
            // 
            this.boundary_box_status.AutoSize = true;
            this.boundary_box_status.Location = new System.Drawing.Point(6, 22);
            this.boundary_box_status.Name = "boundary_box_status";
            this.boundary_box_status.Size = new System.Drawing.Size(65, 13);
            this.boundary_box_status.TabIndex = 34;
            this.boundary_box_status.Text = "UNKNOWN";
            // 
            // Item
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(670, 222);
            this.Controls.Add(this.group_boundary_box);
            this.Controls.Add(this.button_cancel);
            this.Controls.Add(this.button_ok);
            this.Controls.Add(this.group_placement);
            this.Controls.Add(this.group_text);
            this.Controls.Add(this.group_type);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Item";
            this.Text = "LaTeX2AI Create Item";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ThisFormClosed);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.FormKeyDown);
            this.group_placement.ResumeLayout(false);
            this.group_placement.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.group_text.ResumeLayout(false);
            this.group_type.ResumeLayout(false);
            this.group_type.PerformLayout();
            this.group_boundary_box.ResumeLayout(false);
            this.group_boundary_box.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button_cancel;
        private System.Windows.Forms.Button button_ok;
        private System.Windows.Forms.RadioButton pos_11;
        private System.Windows.Forms.RadioButton pos_0;
        private System.Windows.Forms.RadioButton pos_10;
        private System.Windows.Forms.GroupBox group_placement;
        private System.Windows.Forms.RadioButton pos_1;
        private System.Windows.Forms.RadioButton pos_9;
        private System.Windows.Forms.RadioButton pos_2;
        private System.Windows.Forms.RadioButton pos_8;
        private System.Windows.Forms.RadioButton pos_3;
        private System.Windows.Forms.RadioButton pos_7;
        private System.Windows.Forms.RadioButton pos_4;
        private System.Windows.Forms.RadioButton pos_6;
        private System.Windows.Forms.RadioButton pos_5;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.GroupBox group_text;
        private System.Windows.Forms.RichTextBox textbox;
        private System.Windows.Forms.CheckBox clip_check_box;
        private System.Windows.Forms.RadioButton original_size;
        private System.Windows.Forms.RadioButton strech;
        private System.Windows.Forms.GroupBox group_type;
        private System.Windows.Forms.Button rebo_boundary_box;
        private System.Windows.Forms.GroupBox group_boundary_box;
        private System.Windows.Forms.Label boundary_box_status;
    }
}
