namespace Object_tool
{
    partial class BatchFlags
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
            this.FlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.MotionFlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.AnimsNoCompress = new System.Windows.Forms.RadioButton();
            this.Anims8Bit = new System.Windows.Forms.RadioButton();
            this.Anims16Bit = new System.Windows.Forms.RadioButton();
            this.ModelFlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.StripifyMeshes = new System.Windows.Forms.CheckBox();
            this.HQGeometry = new System.Windows.Forms.RadioButton();
            this.HQGeometryPlus = new System.Windows.Forms.RadioButton();
            this.ProgressiveMeshes = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.ObjectScaleLabel = new System.Windows.Forms.Label();
            this.ScaleCenterOfMassCheckBox = new System.Windows.Forms.CheckBox();
            this.ObjectScaleTextBox = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.FlagsGroupBox.SuspendLayout();
            this.MotionFlagsGroupBox.SuspendLayout();
            this.ModelFlagsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // FlagsGroupBox
            // 
            this.FlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FlagsGroupBox.Controls.Add(this.button1);
            this.FlagsGroupBox.Controls.Add(this.MotionFlagsGroupBox);
            this.FlagsGroupBox.Controls.Add(this.ModelFlagsGroupBox);
            this.FlagsGroupBox.Controls.Add(this.ObjectScaleLabel);
            this.FlagsGroupBox.Controls.Add(this.ScaleCenterOfMassCheckBox);
            this.FlagsGroupBox.Controls.Add(this.ObjectScaleTextBox);
            this.FlagsGroupBox.Location = new System.Drawing.Point(12, 12);
            this.FlagsGroupBox.Name = "FlagsGroupBox";
            this.FlagsGroupBox.Size = new System.Drawing.Size(354, 184);
            this.FlagsGroupBox.TabIndex = 17;
            this.FlagsGroupBox.TabStop = false;
            this.FlagsGroupBox.Text = "Edit export flags";
            // 
            // MotionFlagsGroupBox
            // 
            this.MotionFlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MotionFlagsGroupBox.Controls.Add(this.AnimsNoCompress);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims8Bit);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims16Bit);
            this.MotionFlagsGroupBox.Location = new System.Drawing.Point(6, 15);
            this.MotionFlagsGroupBox.Name = "MotionFlagsGroupBox";
            this.MotionFlagsGroupBox.Size = new System.Drawing.Size(160, 90);
            this.MotionFlagsGroupBox.TabIndex = 22;
            this.MotionFlagsGroupBox.TabStop = false;
            this.MotionFlagsGroupBox.Text = "Motion export";
            // 
            // AnimsNoCompress
            // 
            this.AnimsNoCompress.AutoSize = true;
            this.AnimsNoCompress.Location = new System.Drawing.Point(8, 65);
            this.AnimsNoCompress.Name = "AnimsNoCompress";
            this.AnimsNoCompress.Size = new System.Drawing.Size(87, 17);
            this.AnimsNoCompress.TabIndex = 10;
            this.AnimsNoCompress.TabStop = true;
            this.AnimsNoCompress.Text = "No compress";
            this.AnimsNoCompress.UseVisualStyleBackColor = true;
            // 
            // Anims8Bit
            // 
            this.Anims8Bit.AutoSize = true;
            this.Anims8Bit.Location = new System.Drawing.Point(8, 19);
            this.Anims8Bit.Name = "Anims8Bit";
            this.Anims8Bit.Size = new System.Drawing.Size(45, 17);
            this.Anims8Bit.TabIndex = 8;
            this.Anims8Bit.Text = "8 bit";
            this.Anims8Bit.UseVisualStyleBackColor = true;
            // 
            // Anims16Bit
            // 
            this.Anims16Bit.AutoSize = true;
            this.Anims16Bit.Checked = true;
            this.Anims16Bit.Location = new System.Drawing.Point(8, 42);
            this.Anims16Bit.Name = "Anims16Bit";
            this.Anims16Bit.Size = new System.Drawing.Size(51, 17);
            this.Anims16Bit.TabIndex = 9;
            this.Anims16Bit.TabStop = true;
            this.Anims16Bit.Text = "16 bit";
            this.Anims16Bit.UseVisualStyleBackColor = true;
            // 
            // ModelFlagsGroupBox
            // 
            this.ModelFlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ModelFlagsGroupBox.Controls.Add(this.StripifyMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometry);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometryPlus);
            this.ModelFlagsGroupBox.Controls.Add(this.ProgressiveMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.checkBox2);
            this.ModelFlagsGroupBox.Location = new System.Drawing.Point(173, 15);
            this.ModelFlagsGroupBox.Name = "ModelFlagsGroupBox";
            this.ModelFlagsGroupBox.Size = new System.Drawing.Size(175, 137);
            this.ModelFlagsGroupBox.TabIndex = 21;
            this.ModelFlagsGroupBox.TabStop = false;
            this.ModelFlagsGroupBox.Text = "Model export";
            // 
            // StripifyMeshes
            // 
            this.StripifyMeshes.AutoSize = true;
            this.StripifyMeshes.Location = new System.Drawing.Point(6, 89);
            this.StripifyMeshes.Name = "StripifyMeshes";
            this.StripifyMeshes.Size = new System.Drawing.Size(124, 17);
            this.StripifyMeshes.TabIndex = 21;
            this.StripifyMeshes.Text = "Make stripify meshes";
            this.StripifyMeshes.UseVisualStyleBackColor = true;
            this.StripifyMeshes.CheckedChanged += new System.EventHandler(this.StripifyMeshes_CheckedChanged);
            // 
            // HQGeometry
            // 
            this.HQGeometry.AutoSize = true;
            this.HQGeometry.Location = new System.Drawing.Point(6, 19);
            this.HQGeometry.Name = "HQGeometry";
            this.HQGeometry.Size = new System.Drawing.Size(89, 17);
            this.HQGeometry.TabIndex = 19;
            this.HQGeometry.Text = "HQ Geometry";
            this.HQGeometry.UseVisualStyleBackColor = true;
            // 
            // HQGeometryPlus
            // 
            this.HQGeometryPlus.AutoSize = true;
            this.HQGeometryPlus.Checked = true;
            this.HQGeometryPlus.Location = new System.Drawing.Point(6, 42);
            this.HQGeometryPlus.Name = "HQGeometryPlus";
            this.HQGeometryPlus.Size = new System.Drawing.Size(98, 17);
            this.HQGeometryPlus.TabIndex = 20;
            this.HQGeometryPlus.TabStop = true;
            this.HQGeometryPlus.Text = "HQ Geometry+ ";
            this.HQGeometryPlus.UseVisualStyleBackColor = true;
            // 
            // ProgressiveMeshes
            // 
            this.ProgressiveMeshes.AutoSize = true;
            this.ProgressiveMeshes.Location = new System.Drawing.Point(6, 65);
            this.ProgressiveMeshes.Name = "ProgressiveMeshes";
            this.ProgressiveMeshes.Size = new System.Drawing.Size(149, 17);
            this.ProgressiveMeshes.TabIndex = 13;
            this.ProgressiveMeshes.Text = "Make progressive meshes";
            this.ProgressiveMeshes.UseVisualStyleBackColor = true;
            this.ProgressiveMeshes.CheckedChanged += new System.EventHandler(this.ProgressiveMeshes_CheckedChanged);
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(6, 112);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(109, 17);
            this.checkBox2.TabIndex = 14;
            this.checkBox2.Text = "Optimize surfaces";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // ObjectScaleLabel
            // 
            this.ObjectScaleLabel.AutoSize = true;
            this.ObjectScaleLabel.Location = new System.Drawing.Point(3, 112);
            this.ObjectScaleLabel.Name = "ObjectScaleLabel";
            this.ObjectScaleLabel.Size = new System.Drawing.Size(69, 13);
            this.ObjectScaleLabel.TabIndex = 18;
            this.ObjectScaleLabel.Text = "Object scale:";
            // 
            // ScaleCenterOfMassCheckBox
            // 
            this.ScaleCenterOfMassCheckBox.AutoSize = true;
            this.ScaleCenterOfMassCheckBox.Checked = true;
            this.ScaleCenterOfMassCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ScaleCenterOfMassCheckBox.Location = new System.Drawing.Point(6, 135);
            this.ScaleCenterOfMassCheckBox.Name = "ScaleCenterOfMassCheckBox";
            this.ScaleCenterOfMassCheckBox.Size = new System.Drawing.Size(125, 17);
            this.ScaleCenterOfMassCheckBox.TabIndex = 17;
            this.ScaleCenterOfMassCheckBox.Text = "Scale center of mass";
            this.ScaleCenterOfMassCheckBox.UseVisualStyleBackColor = true;
            // 
            // ObjectScaleTextBox
            // 
            this.ObjectScaleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ObjectScaleTextBox.Location = new System.Drawing.Point(78, 109);
            this.ObjectScaleTextBox.Name = "ObjectScaleTextBox";
            this.ObjectScaleTextBox.Size = new System.Drawing.Size(88, 20);
            this.ObjectScaleTextBox.TabIndex = 16;
            this.ObjectScaleTextBox.Text = "1";
            this.ObjectScaleTextBox.TextChanged += new System.EventHandler(this.ScaleTextChanged);
            this.ObjectScaleTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ScaleKeyDown);
            this.ObjectScaleTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ScaleKeyPress);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(6, 155);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(342, 23);
            this.button1.TabIndex = 23;
            this.button1.Text = "Start";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // BatchFlags
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(378, 208);
            this.Controls.Add(this.FlagsGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.Name = "BatchFlags";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "BatchFlags";
            this.FlagsGroupBox.ResumeLayout(false);
            this.FlagsGroupBox.PerformLayout();
            this.MotionFlagsGroupBox.ResumeLayout(false);
            this.MotionFlagsGroupBox.PerformLayout();
            this.ModelFlagsGroupBox.ResumeLayout(false);
            this.ModelFlagsGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox FlagsGroupBox;
        private System.Windows.Forms.GroupBox MotionFlagsGroupBox;
        private System.Windows.Forms.RadioButton AnimsNoCompress;
        private System.Windows.Forms.RadioButton Anims8Bit;
        private System.Windows.Forms.RadioButton Anims16Bit;
        private System.Windows.Forms.GroupBox ModelFlagsGroupBox;
        private System.Windows.Forms.CheckBox StripifyMeshes;
        private System.Windows.Forms.RadioButton HQGeometry;
        private System.Windows.Forms.RadioButton HQGeometryPlus;
        private System.Windows.Forms.CheckBox ProgressiveMeshes;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.Label ObjectScaleLabel;
        private System.Windows.Forms.CheckBox ScaleCenterOfMassCheckBox;
        private System.Windows.Forms.TextBox ObjectScaleTextBox;
        private System.Windows.Forms.Button button1;
    }
}