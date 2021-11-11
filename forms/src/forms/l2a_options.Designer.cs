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

namespace L2A.FORMS
{
    partial class Options
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
            this.latex_command = new System.Windows.Forms.ComboBox();
            this.button_default = new System.Windows.Forms.Button();
            this.button_ok = new System.Windows.Forms.Button();
            this.button_cancel = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.gs_path = new System.Windows.Forms.TextBox();
            this.button_select_gs_path = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.latex_path = new System.Windows.Forms.TextBox();
            this.button_select_latex_path = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.latex_command_options = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.folder_browser_dialog = new System.Windows.Forms.FolderBrowserDialog();
            this.open_file_dialog = new System.Windows.Forms.OpenFileDialog();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.version = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.git_sha = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.header_button = new System.Windows.Forms.Button();
            this.document_header = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // latex_command
            // 
            this.latex_command.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.latex_command.FormattingEnabled = true;
            this.latex_command.Location = new System.Drawing.Point(9, 42);
            this.latex_command.Name = "latex_command";
            this.latex_command.Size = new System.Drawing.Size(110, 21);
            this.latex_command.TabIndex = 0;
            // 
            // button_default
            // 
            this.button_default.Location = new System.Drawing.Point(12, 407);
            this.button_default.Name = "button_default";
            this.button_default.Size = new System.Drawing.Size(95, 23);
            this.button_default.TabIndex = 1;
            this.button_default.Text = "Default Values";
            this.button_default.UseVisualStyleBackColor = true;
            this.button_default.Click += new System.EventHandler(this.SetDefaultParameters);
            // 
            // button_ok
            // 
            this.button_ok.Location = new System.Drawing.Point(308, 407);
            this.button_ok.Name = "button_ok";
            this.button_ok.Size = new System.Drawing.Size(95, 23);
            this.button_ok.TabIndex = 2;
            this.button_ok.Text = "Save and Exit";
            this.button_ok.UseVisualStyleBackColor = true;
            this.button_ok.Click += new System.EventHandler(this.OkClick);
            // 
            // button_cancel
            // 
            this.button_cancel.Location = new System.Drawing.Point(409, 407);
            this.button_cancel.Name = "button_cancel";
            this.button_cancel.Size = new System.Drawing.Size(95, 23);
            this.button_cancel.TabIndex = 3;
            this.button_cancel.Text = "Cancel";
            this.button_cancel.UseVisualStyleBackColor = true;
            this.button_cancel.Click += new System.EventHandler(this.CancelClick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.gs_path);
            this.groupBox1.Controls.Add(this.button_select_gs_path);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.latex_path);
            this.groupBox1.Controls.Add(this.button_select_latex_path);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.latex_command_options);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.latex_command);
            this.groupBox1.Location = new System.Drawing.Point(12, 92);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(492, 229);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Global LaTeX Options";
            // 
            // gs_path
            // 
            this.gs_path.Location = new System.Drawing.Point(9, 198);
            this.gs_path.Name = "gs_path";
            this.gs_path.Size = new System.Drawing.Size(445, 20);
            this.gs_path.TabIndex = 9;
            // 
            // button_select_gs_path
            // 
            this.button_select_gs_path.Location = new System.Drawing.Point(460, 196);
            this.button_select_gs_path.Name = "button_select_gs_path";
            this.button_select_gs_path.Size = new System.Drawing.Size(26, 23);
            this.button_select_gs_path.TabIndex = 8;
            this.button_select_gs_path.Text = "...";
            this.button_select_gs_path.UseVisualStyleBackColor = true;
            this.button_select_gs_path.Click += new System.EventHandler(this.SelectGhostScriptFolder);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 182);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(260, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Ghost Script Command (gswin32c.exe, gswin64c.exe)";
            // 
            // latex_path
            // 
            this.latex_path.Location = new System.Drawing.Point(9, 146);
            this.latex_path.Name = "latex_path";
            this.latex_path.Size = new System.Drawing.Size(445, 20);
            this.latex_path.TabIndex = 6;
            // 
            // button_select_latex_path
            // 
            this.button_select_latex_path.Location = new System.Drawing.Point(460, 144);
            this.button_select_latex_path.Name = "button_select_latex_path";
            this.button_select_latex_path.Size = new System.Drawing.Size(26, 23);
            this.button_select_latex_path.TabIndex = 5;
            this.button_select_latex_path.Text = "...";
            this.button_select_latex_path.UseVisualStyleBackColor = true;
            this.button_select_latex_path.Click += new System.EventHandler(this.SelectLatexFolder);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 130);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(267, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Folder with LaTeX executables (leave blank for default)";
            // 
            // latex_command_options
            // 
            this.latex_command_options.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.latex_command_options.Location = new System.Drawing.Point(9, 96);
            this.latex_command_options.Name = "latex_command_options";
            this.latex_command_options.Size = new System.Drawing.Size(477, 20);
            this.latex_command_options.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 79);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(109, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Commandline Options";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(40, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Engine";
            // 
            // open_file_dialog
            // 
            this.open_file_dialog.Filter = "Executables|*.exe";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.version);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.git_sha);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Location = new System.Drawing.Point(12, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(492, 73);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "LaTeX2AI Information";
            // 
            // version
            // 
            this.version.Location = new System.Drawing.Point(57, 23);
            this.version.Name = "version";
            this.version.Size = new System.Drawing.Size(429, 20);
            this.version.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(6, 23);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(42, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Version";
            // 
            // git_sha
            // 
            this.git_sha.Location = new System.Drawing.Point(57, 46);
            this.git_sha.Name = "git_sha";
            this.git_sha.Size = new System.Drawing.Size(429, 20);
            this.git_sha.TabIndex = 11;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 46);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Git SHA";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.header_button);
            this.groupBox3.Controls.Add(this.document_header);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Location = new System.Drawing.Point(12, 327);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(492, 74);
            this.groupBox3.TabIndex = 11;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Document LaTeX2AI Information";
            // 
            // header_button
            // 
            this.header_button.Location = new System.Drawing.Point(400, 41);
            this.header_button.Name = "header_button";
            this.header_button.Size = new System.Drawing.Size(86, 23);
            this.header_button.TabIndex = 10;
            this.header_button.Text = "dummy";
            this.header_button.UseVisualStyleBackColor = true;
            this.header_button.Click += new System.EventHandler(this.HeaderClick);
            // 
            // document_header
            // 
            this.document_header.Location = new System.Drawing.Point(22, 43);
            this.document_header.Name = "document_header";
            this.document_header.Size = new System.Drawing.Size(372, 20);
            this.document_header.TabIndex = 12;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(6, 22);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 13);
            this.label6.TabIndex = 2;
            this.label6.Text = "LaTeX Header";
            // 
            // Options
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(516, 441);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button_cancel);
            this.Controls.Add(this.button_ok);
            this.Controls.Add(this.button_default);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Options";
            this.Text = "LaTeX2AI Options";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ThisFormClosed);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox latex_command;
        private System.Windows.Forms.Button button_default;
        private System.Windows.Forms.Button button_ok;
        private System.Windows.Forms.Button button_cancel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox latex_command_options;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button_select_latex_path;
        private System.Windows.Forms.FolderBrowserDialog folder_browser_dialog;
        private System.Windows.Forms.TextBox latex_path;
        private System.Windows.Forms.TextBox gs_path;
        private System.Windows.Forms.Button button_select_gs_path;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.OpenFileDialog open_file_dialog;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox git_sha;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button header_button;
        private System.Windows.Forms.TextBox document_header;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox version;
        private System.Windows.Forms.Label label7;
    }
}
