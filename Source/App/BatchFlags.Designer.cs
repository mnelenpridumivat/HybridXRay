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
            this.DynamicGroupBox = new System.Windows.Forms.GroupBox();
            this.SoCInfluence = new System.Windows.Forms.CheckBox();
            this.ObjectScaleTextBox = new System.Windows.Forms.TextBox();
            this.ScaleCenterOfMassCheckBox = new System.Windows.Forms.CheckBox();
            this.ObjectScaleLabel = new System.Windows.Forms.Label();
            this.ModelFlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.SmoothCoP = new System.Windows.Forms.RadioButton();
            this.SmoothSoC = new System.Windows.Forms.RadioButton();
            this.SplitNormalsChbx = new System.Windows.Forms.CheckBox();
            this.StripifyMeshes = new System.Windows.Forms.CheckBox();
            this.HQGeometry = new System.Windows.Forms.RadioButton();
            this.HQGeometryPlus = new System.Windows.Forms.RadioButton();
            this.ProgressiveMeshes = new System.Windows.Forms.CheckBox();
            this.OptimizeSurfaces = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.MotionFlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.BuildInMotionsExport = new System.Windows.Forms.CheckBox();
            this.AnimsNoCompress = new System.Windows.Forms.RadioButton();
            this.Anims8Bit = new System.Windows.Forms.RadioButton();
            this.Anims16Bit = new System.Windows.Forms.RadioButton();
            this.FlagsGroupBox.SuspendLayout();
            this.DynamicGroupBox.SuspendLayout();
            this.ModelFlagsGroupBox.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.MotionFlagsGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // FlagsGroupBox
            // 
            this.FlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FlagsGroupBox.Controls.Add(this.DynamicGroupBox);
            this.FlagsGroupBox.Controls.Add(this.ModelFlagsGroupBox);
            this.FlagsGroupBox.Controls.Add(this.button1);
            this.FlagsGroupBox.Controls.Add(this.MotionFlagsGroupBox);
            this.FlagsGroupBox.Location = new System.Drawing.Point(12, 12);
            this.FlagsGroupBox.Name = "FlagsGroupBox";
            this.FlagsGroupBox.Size = new System.Drawing.Size(354, 256);
            this.FlagsGroupBox.TabIndex = 17;
            this.FlagsGroupBox.TabStop = false;
            this.FlagsGroupBox.Text = "Edit export flags";
            // 
            // DynamicGroupBox
            // 
            this.DynamicGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DynamicGroupBox.Controls.Add(this.SoCInfluence);
            this.DynamicGroupBox.Controls.Add(this.ObjectScaleTextBox);
            this.DynamicGroupBox.Controls.Add(this.ScaleCenterOfMassCheckBox);
            this.DynamicGroupBox.Controls.Add(this.ObjectScaleLabel);
            this.DynamicGroupBox.Location = new System.Drawing.Point(6, 127);
            this.DynamicGroupBox.Name = "DynamicGroupBox";
            this.DynamicGroupBox.Size = new System.Drawing.Size(160, 96);
            this.DynamicGroupBox.TabIndex = 25;
            this.DynamicGroupBox.TabStop = false;
            this.DynamicGroupBox.Text = "Dynamic params";
            // 
            // SoCInfluence
            // 
            this.SoCInfluence.AutoSize = true;
            this.SoCInfluence.Location = new System.Drawing.Point(8, 68);
            this.SoCInfluence.Name = "SoCInfluence";
            this.SoCInfluence.Size = new System.Drawing.Size(105, 17);
            this.SoCInfluence.TabIndex = 20;
            this.SoCInfluence.Text = "SoC bone export";
            this.SoCInfluence.UseVisualStyleBackColor = true;
            // 
            // ObjectScaleTextBox
            // 
            this.ObjectScaleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ObjectScaleTextBox.Location = new System.Drawing.Point(80, 19);
            this.ObjectScaleTextBox.Name = "ObjectScaleTextBox";
            this.ObjectScaleTextBox.Size = new System.Drawing.Size(73, 20);
            this.ObjectScaleTextBox.TabIndex = 16;
            this.ObjectScaleTextBox.Text = "1";
            this.ObjectScaleTextBox.TextChanged += new System.EventHandler(this.ScaleTextChanged);
            this.ObjectScaleTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ScaleKeyDown);
            this.ObjectScaleTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ScaleKeyPress);
            // 
            // ScaleCenterOfMassCheckBox
            // 
            this.ScaleCenterOfMassCheckBox.AutoSize = true;
            this.ScaleCenterOfMassCheckBox.Checked = true;
            this.ScaleCenterOfMassCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ScaleCenterOfMassCheckBox.Location = new System.Drawing.Point(8, 45);
            this.ScaleCenterOfMassCheckBox.Name = "ScaleCenterOfMassCheckBox";
            this.ScaleCenterOfMassCheckBox.Size = new System.Drawing.Size(125, 17);
            this.ScaleCenterOfMassCheckBox.TabIndex = 17;
            this.ScaleCenterOfMassCheckBox.Text = "Scale center of mass";
            this.ScaleCenterOfMassCheckBox.UseVisualStyleBackColor = true;
            // 
            // ObjectScaleLabel
            // 
            this.ObjectScaleLabel.AutoSize = true;
            this.ObjectScaleLabel.Location = new System.Drawing.Point(5, 22);
            this.ObjectScaleLabel.Name = "ObjectScaleLabel";
            this.ObjectScaleLabel.Size = new System.Drawing.Size(69, 13);
            this.ObjectScaleLabel.TabIndex = 18;
            this.ObjectScaleLabel.Text = "Object scale:";
            // 
            // ModelFlagsGroupBox
            // 
            this.ModelFlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ModelFlagsGroupBox.Controls.Add(this.groupBox3);
            this.ModelFlagsGroupBox.Controls.Add(this.SplitNormalsChbx);
            this.ModelFlagsGroupBox.Controls.Add(this.StripifyMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometry);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometryPlus);
            this.ModelFlagsGroupBox.Controls.Add(this.ProgressiveMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.OptimizeSurfaces);
            this.ModelFlagsGroupBox.Location = new System.Drawing.Point(172, 15);
            this.ModelFlagsGroupBox.Name = "ModelFlagsGroupBox";
            this.ModelFlagsGroupBox.Size = new System.Drawing.Size(175, 208);
            this.ModelFlagsGroupBox.TabIndex = 24;
            this.ModelFlagsGroupBox.TabStop = false;
            this.ModelFlagsGroupBox.Text = "Model export";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.SmoothCoP);
            this.groupBox3.Controls.Add(this.SmoothSoC);
            this.groupBox3.Location = new System.Drawing.Point(6, 158);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(163, 44);
            this.groupBox3.TabIndex = 24;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Smooth Type";
            // 
            // SmoothCoP
            // 
            this.SmoothCoP.AutoSize = true;
            this.SmoothCoP.Checked = true;
            this.SmoothCoP.Location = new System.Drawing.Point(75, 19);
            this.SmoothCoP.Name = "SmoothCoP";
            this.SmoothCoP.Size = new System.Drawing.Size(64, 17);
            this.SmoothCoP.TabIndex = 1;
            this.SmoothCoP.TabStop = true;
            this.SmoothCoP.Text = "CS\\CoP";
            this.SmoothCoP.UseVisualStyleBackColor = true;
            // 
            // SmoothSoC
            // 
            this.SmoothSoC.AutoSize = true;
            this.SmoothSoC.Location = new System.Drawing.Point(24, 19);
            this.SmoothSoC.Name = "SmoothSoC";
            this.SmoothSoC.Size = new System.Drawing.Size(45, 17);
            this.SmoothSoC.TabIndex = 0;
            this.SmoothSoC.Text = "SoC";
            this.SmoothSoC.UseVisualStyleBackColor = true;
            // 
            // SplitNormalsChbx
            // 
            this.SplitNormalsChbx.AutoSize = true;
            this.SplitNormalsChbx.Checked = true;
            this.SplitNormalsChbx.CheckState = System.Windows.Forms.CheckState.Checked;
            this.SplitNormalsChbx.Location = new System.Drawing.Point(6, 135);
            this.SplitNormalsChbx.Name = "SplitNormalsChbx";
            this.SplitNormalsChbx.Size = new System.Drawing.Size(105, 17);
            this.SplitNormalsChbx.TabIndex = 22;
            this.SplitNormalsChbx.Text = "Use split normals";
            this.SplitNormalsChbx.UseVisualStyleBackColor = true;
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
            // OptimizeSurfaces
            // 
            this.OptimizeSurfaces.AutoSize = true;
            this.OptimizeSurfaces.Location = new System.Drawing.Point(6, 112);
            this.OptimizeSurfaces.Name = "OptimizeSurfaces";
            this.OptimizeSurfaces.Size = new System.Drawing.Size(109, 17);
            this.OptimizeSurfaces.TabIndex = 14;
            this.OptimizeSurfaces.Text = "Optimize surfaces";
            this.OptimizeSurfaces.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(6, 227);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(342, 23);
            this.button1.TabIndex = 23;
            this.button1.Text = "Start";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // MotionFlagsGroupBox
            // 
            this.MotionFlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MotionFlagsGroupBox.Controls.Add(this.BuildInMotionsExport);
            this.MotionFlagsGroupBox.Controls.Add(this.AnimsNoCompress);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims8Bit);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims16Bit);
            this.MotionFlagsGroupBox.Location = new System.Drawing.Point(6, 15);
            this.MotionFlagsGroupBox.Name = "MotionFlagsGroupBox";
            this.MotionFlagsGroupBox.Size = new System.Drawing.Size(160, 111);
            this.MotionFlagsGroupBox.TabIndex = 22;
            this.MotionFlagsGroupBox.TabStop = false;
            this.MotionFlagsGroupBox.Text = "Motion export";
            // 
            // BuildInMotionsExport
            // 
            this.BuildInMotionsExport.AutoSize = true;
            this.BuildInMotionsExport.Checked = true;
            this.BuildInMotionsExport.CheckState = System.Windows.Forms.CheckState.Checked;
            this.BuildInMotionsExport.Location = new System.Drawing.Point(8, 88);
            this.BuildInMotionsExport.Name = "BuildInMotionsExport";
            this.BuildInMotionsExport.Size = new System.Drawing.Size(120, 17);
            this.BuildInMotionsExport.TabIndex = 12;
            this.BuildInMotionsExport.Text = "Use build-in motions";
            this.BuildInMotionsExport.UseVisualStyleBackColor = true;
            // 
            // AnimsNoCompress
            // 
            this.AnimsNoCompress.AutoSize = true;
            this.AnimsNoCompress.Checked = true;
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
            this.Anims16Bit.Location = new System.Drawing.Point(8, 42);
            this.Anims16Bit.Name = "Anims16Bit";
            this.Anims16Bit.Size = new System.Drawing.Size(51, 17);
            this.Anims16Bit.TabIndex = 9;
            this.Anims16Bit.Text = "16 bit";
            this.Anims16Bit.UseVisualStyleBackColor = true;
            // 
            // BatchFlags
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(378, 280);
            this.Controls.Add(this.FlagsGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.Name = "BatchFlags";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "BatchFlags";
            this.FlagsGroupBox.ResumeLayout(false);
            this.DynamicGroupBox.ResumeLayout(false);
            this.DynamicGroupBox.PerformLayout();
            this.ModelFlagsGroupBox.ResumeLayout(false);
            this.ModelFlagsGroupBox.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.MotionFlagsGroupBox.ResumeLayout(false);
            this.MotionFlagsGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox FlagsGroupBox;
        private System.Windows.Forms.GroupBox MotionFlagsGroupBox;
        private System.Windows.Forms.RadioButton AnimsNoCompress;
        private System.Windows.Forms.RadioButton Anims8Bit;
        private System.Windows.Forms.RadioButton Anims16Bit;
        private System.Windows.Forms.Label ObjectScaleLabel;
        private System.Windows.Forms.CheckBox ScaleCenterOfMassCheckBox;
        private System.Windows.Forms.TextBox ObjectScaleTextBox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox ModelFlagsGroupBox;
        private System.Windows.Forms.CheckBox SplitNormalsChbx;
        private System.Windows.Forms.CheckBox StripifyMeshes;
        private System.Windows.Forms.RadioButton HQGeometry;
        private System.Windows.Forms.RadioButton HQGeometryPlus;
        private System.Windows.Forms.CheckBox ProgressiveMeshes;
        private System.Windows.Forms.CheckBox OptimizeSurfaces;
        private System.Windows.Forms.CheckBox BuildInMotionsExport;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton SmoothCoP;
        private System.Windows.Forms.RadioButton SmoothSoC;
        private System.Windows.Forms.GroupBox DynamicGroupBox;
        private System.Windows.Forms.CheckBox SoCInfluence;
    }
}