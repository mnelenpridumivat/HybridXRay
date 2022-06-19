
namespace Object_tool
{
    partial class Object_Editor
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Object_Editor));
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.openFileDialog2 = new System.Windows.Forms.OpenFileDialog();
            this.DeleteMotionsButton = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 13);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(122, 57);
            this.button1.TabIndex = 0;
            this.button1.Text = "Export OGF";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.ExportOGF_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(12, 76);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(122, 57);
            this.button2.TabIndex = 1;
            this.button2.Text = "Export OMF";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.ExportOMF_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(142, 13);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(122, 57);
            this.button3.TabIndex = 2;
            this.button3.Text = "Load motions";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.LoadSkls_Click);
            // 
            // openFileDialog2
            // 
            this.openFileDialog2.FileName = "openFileDialog2";
            // 
            // DeleteMotionsButton
            // 
            this.DeleteMotionsButton.Location = new System.Drawing.Point(142, 76);
            this.DeleteMotionsButton.Name = "DeleteMotionsButton";
            this.DeleteMotionsButton.Size = new System.Drawing.Size(122, 57);
            this.DeleteMotionsButton.TabIndex = 3;
            this.DeleteMotionsButton.Text = "Delete motions";
            this.DeleteMotionsButton.UseVisualStyleBackColor = true;
            this.DeleteMotionsButton.Click += new System.EventHandler(this.DeleteMotionsButton_Click);
            // 
            // Object_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(276, 145);
            this.Controls.Add(this.DeleteMotionsButton);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(550, 880);
            this.MinimumSize = new System.Drawing.Size(100, 100);
            this.Name = "Object_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Object Editor";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.OpenFileDialog openFileDialog2;
        private System.Windows.Forms.Button DeleteMotionsButton;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
    }
}

