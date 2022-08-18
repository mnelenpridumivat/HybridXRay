namespace Object_tool
{
    partial class Settings
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Settings));
            this.NoCompress = new System.Windows.Forms.CheckBox();
            this.Debug = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ScaleCenterOfMassCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.HQGeometryPlus = new System.Windows.Forms.RadioButton();
            this.HQGeometry = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.DefaultsCoP = new System.Windows.Forms.RadioButton();
            this.DefaultsSoC = new System.Windows.Forms.RadioButton();
            this.SplitNormalsChbx = new System.Windows.Forms.CheckBox();
            this.SoCInfluence = new System.Windows.Forms.CheckBox();
            this.MotionFlagsGroupBox = new System.Windows.Forms.GroupBox();
            this.BuildInMotionsExport = new System.Windows.Forms.CheckBox();
            this.AnimsNoCompress = new System.Windows.Forms.RadioButton();
            this.Anims8Bit = new System.Windows.Forms.RadioButton();
            this.Anims16Bit = new System.Windows.Forms.RadioButton();
            this.SmoothTypeGroupBox = new System.Windows.Forms.GroupBox();
            this.AutoSmooth = new System.Windows.Forms.RadioButton();
            this.SmoothCoP = new System.Windows.Forms.RadioButton();
            this.SmoothSoC = new System.Windows.Forms.RadioButton();
            this.OptimizeSurfaces = new System.Windows.Forms.CheckBox();
            this.StripifyMeshes = new System.Windows.Forms.CheckBox();
            this.ProgressiveMeshes = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.GameMtlPath = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.NoCompressLinkLabel = new System.Windows.Forms.LinkLabel();
            this.button2 = new System.Windows.Forms.Button();
            this.FSLtxPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.TexturesPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.ForceViewport = new System.Windows.Forms.CheckBox();
            this.button4 = new System.Windows.Forms.Button();
            this.ImagePath = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.MotionFlagsGroupBox.SuspendLayout();
            this.SmoothTypeGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // NoCompress
            // 
            this.NoCompress.AutoSize = true;
            this.NoCompress.Location = new System.Drawing.Point(19, 242);
            this.NoCompress.Name = "NoCompress";
            this.NoCompress.Size = new System.Drawing.Size(253, 17);
            this.NoCompress.TabIndex = 0;
            this.NoCompress.Text = "Use No Compress motions (need STCoP reader)";
            this.NoCompress.UseVisualStyleBackColor = true;
            this.NoCompress.CheckedChanged += new System.EventHandler(this.NoCompress_CheckedChanged);
            // 
            // Debug
            // 
            this.Debug.AutoSize = true;
            this.Debug.Location = new System.Drawing.Point(19, 265);
            this.Debug.Name = "Debug";
            this.Debug.Size = new System.Drawing.Size(118, 17);
            this.Debug.TabIndex = 1;
            this.Debug.Text = "Program debugging";
            this.Debug.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ScaleCenterOfMassCheckBox);
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.SplitNormalsChbx);
            this.groupBox1.Controls.Add(this.SoCInfluence);
            this.groupBox1.Controls.Add(this.MotionFlagsGroupBox);
            this.groupBox1.Controls.Add(this.SmoothTypeGroupBox);
            this.groupBox1.Controls.Add(this.OptimizeSurfaces);
            this.groupBox1.Controls.Add(this.StripifyMeshes);
            this.groupBox1.Controls.Add(this.ProgressiveMeshes);
            this.groupBox1.Location = new System.Drawing.Point(13, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(361, 221);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Default Params";
            // 
            // ScaleCenterOfMassCheckBox
            // 
            this.ScaleCenterOfMassCheckBox.AutoSize = true;
            this.ScaleCenterOfMassCheckBox.Checked = true;
            this.ScaleCenterOfMassCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.ScaleCenterOfMassCheckBox.Location = new System.Drawing.Point(6, 193);
            this.ScaleCenterOfMassCheckBox.Name = "ScaleCenterOfMassCheckBox";
            this.ScaleCenterOfMassCheckBox.Size = new System.Drawing.Size(125, 17);
            this.ScaleCenterOfMassCheckBox.TabIndex = 29;
            this.ScaleCenterOfMassCheckBox.Text = "Scale center of mass";
            this.ScaleCenterOfMassCheckBox.UseVisualStyleBackColor = true;
            this.ScaleCenterOfMassCheckBox.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.HQGeometryPlus);
            this.groupBox3.Controls.Add(this.HQGeometry);
            this.groupBox3.Location = new System.Drawing.Point(180, 171);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(175, 44);
            this.groupBox3.TabIndex = 28;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Export Quality";
            // 
            // HQGeometryPlus
            // 
            this.HQGeometryPlus.AutoSize = true;
            this.HQGeometryPlus.Checked = true;
            this.HQGeometryPlus.Location = new System.Drawing.Point(82, 20);
            this.HQGeometryPlus.Name = "HQGeometryPlus";
            this.HQGeometryPlus.Size = new System.Drawing.Size(47, 17);
            this.HQGeometryPlus.TabIndex = 1;
            this.HQGeometryPlus.TabStop = true;
            this.HQGeometryPlus.Text = "HQ+";
            this.HQGeometryPlus.UseVisualStyleBackColor = true;
            this.HQGeometryPlus.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // HQGeometry
            // 
            this.HQGeometry.AutoSize = true;
            this.HQGeometry.Location = new System.Drawing.Point(31, 20);
            this.HQGeometry.Name = "HQGeometry";
            this.HQGeometry.Size = new System.Drawing.Size(41, 17);
            this.HQGeometry.TabIndex = 0;
            this.HQGeometry.Text = "HQ";
            this.HQGeometry.UseVisualStyleBackColor = true;
            this.HQGeometry.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.DefaultsCoP);
            this.groupBox2.Controls.Add(this.DefaultsSoC);
            this.groupBox2.Location = new System.Drawing.Point(6, 15);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(151, 55);
            this.groupBox2.TabIndex = 27;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Presets";
            // 
            // DefaultsCoP
            // 
            this.DefaultsCoP.AutoSize = true;
            this.DefaultsCoP.Checked = true;
            this.DefaultsCoP.Location = new System.Drawing.Point(76, 22);
            this.DefaultsCoP.Name = "DefaultsCoP";
            this.DefaultsCoP.Size = new System.Drawing.Size(64, 17);
            this.DefaultsCoP.TabIndex = 3;
            this.DefaultsCoP.TabStop = true;
            this.DefaultsCoP.Text = "CS\\CoP";
            this.DefaultsCoP.UseVisualStyleBackColor = true;
            this.DefaultsCoP.CheckedChanged += new System.EventHandler(this.DefaultsCoP_CheckedChanged);
            // 
            // DefaultsSoC
            // 
            this.DefaultsSoC.AutoSize = true;
            this.DefaultsSoC.Location = new System.Drawing.Point(15, 22);
            this.DefaultsSoC.Name = "DefaultsSoC";
            this.DefaultsSoC.Size = new System.Drawing.Size(45, 17);
            this.DefaultsSoC.TabIndex = 2;
            this.DefaultsSoC.Text = "SoC";
            this.DefaultsSoC.UseVisualStyleBackColor = true;
            this.DefaultsSoC.CheckedChanged += new System.EventHandler(this.DefaultsSoC_CheckedChanged);
            // 
            // SplitNormalsChbx
            // 
            this.SplitNormalsChbx.AutoSize = true;
            this.SplitNormalsChbx.Checked = true;
            this.SplitNormalsChbx.CheckState = System.Windows.Forms.CheckState.Checked;
            this.SplitNormalsChbx.Location = new System.Drawing.Point(6, 170);
            this.SplitNormalsChbx.Name = "SplitNormalsChbx";
            this.SplitNormalsChbx.Size = new System.Drawing.Size(105, 17);
            this.SplitNormalsChbx.TabIndex = 26;
            this.SplitNormalsChbx.Text = "Use split normals";
            this.SplitNormalsChbx.UseVisualStyleBackColor = true;
            this.SplitNormalsChbx.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // SoCInfluence
            // 
            this.SoCInfluence.AutoSize = true;
            this.SoCInfluence.Location = new System.Drawing.Point(6, 147);
            this.SoCInfluence.Name = "SoCInfluence";
            this.SoCInfluence.Size = new System.Drawing.Size(105, 17);
            this.SoCInfluence.TabIndex = 20;
            this.SoCInfluence.Text = "SoC bone export";
            this.SoCInfluence.UseVisualStyleBackColor = true;
            this.SoCInfluence.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // MotionFlagsGroupBox
            // 
            this.MotionFlagsGroupBox.Controls.Add(this.BuildInMotionsExport);
            this.MotionFlagsGroupBox.Controls.Add(this.AnimsNoCompress);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims8Bit);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims16Bit);
            this.MotionFlagsGroupBox.Location = new System.Drawing.Point(180, 15);
            this.MotionFlagsGroupBox.Name = "MotionFlagsGroupBox";
            this.MotionFlagsGroupBox.Size = new System.Drawing.Size(175, 105);
            this.MotionFlagsGroupBox.TabIndex = 25;
            this.MotionFlagsGroupBox.TabStop = false;
            this.MotionFlagsGroupBox.Text = "Motion Export";
            // 
            // BuildInMotionsExport
            // 
            this.BuildInMotionsExport.AutoSize = true;
            this.BuildInMotionsExport.Checked = true;
            this.BuildInMotionsExport.CheckState = System.Windows.Forms.CheckState.Checked;
            this.BuildInMotionsExport.Location = new System.Drawing.Point(6, 84);
            this.BuildInMotionsExport.Name = "BuildInMotionsExport";
            this.BuildInMotionsExport.Size = new System.Drawing.Size(120, 17);
            this.BuildInMotionsExport.TabIndex = 11;
            this.BuildInMotionsExport.Text = "Use build-in motions";
            this.BuildInMotionsExport.UseVisualStyleBackColor = true;
            this.BuildInMotionsExport.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // AnimsNoCompress
            // 
            this.AnimsNoCompress.AutoSize = true;
            this.AnimsNoCompress.Location = new System.Drawing.Point(6, 61);
            this.AnimsNoCompress.Name = "AnimsNoCompress";
            this.AnimsNoCompress.Size = new System.Drawing.Size(87, 17);
            this.AnimsNoCompress.TabIndex = 10;
            this.AnimsNoCompress.TabStop = true;
            this.AnimsNoCompress.Text = "No compress";
            this.AnimsNoCompress.UseVisualStyleBackColor = true;
            this.AnimsNoCompress.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // Anims8Bit
            // 
            this.Anims8Bit.AutoSize = true;
            this.Anims8Bit.Location = new System.Drawing.Point(6, 15);
            this.Anims8Bit.Name = "Anims8Bit";
            this.Anims8Bit.Size = new System.Drawing.Size(45, 17);
            this.Anims8Bit.TabIndex = 8;
            this.Anims8Bit.Text = "8 bit";
            this.Anims8Bit.UseVisualStyleBackColor = true;
            this.Anims8Bit.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // Anims16Bit
            // 
            this.Anims16Bit.AutoSize = true;
            this.Anims16Bit.Checked = true;
            this.Anims16Bit.Location = new System.Drawing.Point(6, 38);
            this.Anims16Bit.Name = "Anims16Bit";
            this.Anims16Bit.Size = new System.Drawing.Size(51, 17);
            this.Anims16Bit.TabIndex = 9;
            this.Anims16Bit.TabStop = true;
            this.Anims16Bit.Text = "16 bit";
            this.Anims16Bit.UseVisualStyleBackColor = true;
            this.Anims16Bit.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // SmoothTypeGroupBox
            // 
            this.SmoothTypeGroupBox.Controls.Add(this.AutoSmooth);
            this.SmoothTypeGroupBox.Controls.Add(this.SmoothCoP);
            this.SmoothTypeGroupBox.Controls.Add(this.SmoothSoC);
            this.SmoothTypeGroupBox.Location = new System.Drawing.Point(180, 122);
            this.SmoothTypeGroupBox.Name = "SmoothTypeGroupBox";
            this.SmoothTypeGroupBox.Size = new System.Drawing.Size(175, 44);
            this.SmoothTypeGroupBox.TabIndex = 24;
            this.SmoothTypeGroupBox.TabStop = false;
            this.SmoothTypeGroupBox.Text = "Smooth Type";
            // 
            // AutoSmooth
            // 
            this.AutoSmooth.AutoSize = true;
            this.AutoSmooth.Checked = true;
            this.AutoSmooth.Location = new System.Drawing.Point(6, 19);
            this.AutoSmooth.Name = "AutoSmooth";
            this.AutoSmooth.Size = new System.Drawing.Size(47, 17);
            this.AutoSmooth.TabIndex = 2;
            this.AutoSmooth.TabStop = true;
            this.AutoSmooth.Text = "Auto";
            this.AutoSmooth.UseVisualStyleBackColor = true;
            this.AutoSmooth.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // SmoothCoP
            // 
            this.SmoothCoP.AutoSize = true;
            this.SmoothCoP.Location = new System.Drawing.Point(110, 19);
            this.SmoothCoP.Name = "SmoothCoP";
            this.SmoothCoP.Size = new System.Drawing.Size(64, 17);
            this.SmoothCoP.TabIndex = 1;
            this.SmoothCoP.Text = "CS\\CoP";
            this.SmoothCoP.UseVisualStyleBackColor = true;
            this.SmoothCoP.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // SmoothSoC
            // 
            this.SmoothSoC.AutoSize = true;
            this.SmoothSoC.Location = new System.Drawing.Point(59, 19);
            this.SmoothSoC.Name = "SmoothSoC";
            this.SmoothSoC.Size = new System.Drawing.Size(45, 17);
            this.SmoothSoC.TabIndex = 0;
            this.SmoothSoC.Text = "SoC";
            this.SmoothSoC.UseVisualStyleBackColor = true;
            this.SmoothSoC.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // OptimizeSurfaces
            // 
            this.OptimizeSurfaces.AutoSize = true;
            this.OptimizeSurfaces.Location = new System.Drawing.Point(6, 124);
            this.OptimizeSurfaces.Name = "OptimizeSurfaces";
            this.OptimizeSurfaces.Size = new System.Drawing.Size(109, 17);
            this.OptimizeSurfaces.TabIndex = 6;
            this.OptimizeSurfaces.Text = "Optimize surfaces";
            this.OptimizeSurfaces.UseVisualStyleBackColor = true;
            this.OptimizeSurfaces.CheckedChanged += new System.EventHandler(this.SyncForm);
            // 
            // StripifyMeshes
            // 
            this.StripifyMeshes.AutoSize = true;
            this.StripifyMeshes.Location = new System.Drawing.Point(6, 101);
            this.StripifyMeshes.Name = "StripifyMeshes";
            this.StripifyMeshes.Size = new System.Drawing.Size(126, 17);
            this.StripifyMeshes.TabIndex = 5;
            this.StripifyMeshes.Text = "Make Stripify meshes";
            this.StripifyMeshes.UseVisualStyleBackColor = true;
            this.StripifyMeshes.CheckedChanged += new System.EventHandler(this.SyncForm);
            this.StripifyMeshes.CheckStateChanged += new System.EventHandler(this.StripifyCheck);
            // 
            // ProgressiveMeshes
            // 
            this.ProgressiveMeshes.AutoSize = true;
            this.ProgressiveMeshes.Location = new System.Drawing.Point(6, 77);
            this.ProgressiveMeshes.Name = "ProgressiveMeshes";
            this.ProgressiveMeshes.Size = new System.Drawing.Size(150, 17);
            this.ProgressiveMeshes.TabIndex = 4;
            this.ProgressiveMeshes.Text = "Make Progressive meshes";
            this.ProgressiveMeshes.UseVisualStyleBackColor = true;
            this.ProgressiveMeshes.CheckedChanged += new System.EventHandler(this.SyncForm);
            this.ProgressiveMeshes.CheckStateChanged += new System.EventHandler(this.ProgressiveCheck);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 392);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Game Mtl path:";
            // 
            // GameMtlPath
            // 
            this.GameMtlPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.GameMtlPath.Location = new System.Drawing.Point(102, 389);
            this.GameMtlPath.Name = "GameMtlPath";
            this.GameMtlPath.Size = new System.Drawing.Size(234, 20);
            this.GameMtlPath.TabIndex = 5;
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button1.Location = new System.Drawing.Point(342, 387);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(42, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "Find";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.FindGameMtlPath);
            // 
            // NoCompressLinkLabel
            // 
            this.NoCompressLinkLabel.AutoSize = true;
            this.NoCompressLinkLabel.Location = new System.Drawing.Point(272, 243);
            this.NoCompressLinkLabel.Name = "NoCompressLinkLabel";
            this.NoCompressLinkLabel.Size = new System.Drawing.Size(57, 13);
            this.NoCompressLinkLabel.TabIndex = 7;
            this.NoCompressLinkLabel.TabStop = true;
            this.NoCompressLinkLabel.Text = "View code";
            this.NoCompressLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // button2
            // 
            this.button2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button2.Location = new System.Drawing.Point(342, 334);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(42, 23);
            this.button2.TabIndex = 10;
            this.button2.Text = "Find";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.FindFsPath);
            // 
            // FSLtxPath
            // 
            this.FSLtxPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.FSLtxPath.Location = new System.Drawing.Point(102, 336);
            this.FSLtxPath.Name = "FSLtxPath";
            this.FSLtxPath.Size = new System.Drawing.Size(234, 20);
            this.FSLtxPath.TabIndex = 9;
            this.FSLtxPath.TextChanged += new System.EventHandler(this.FsPathTextChanged);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 339);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "FS Ltx path:";
            // 
            // button3
            // 
            this.button3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button3.Location = new System.Drawing.Point(342, 360);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(42, 23);
            this.button3.TabIndex = 13;
            this.button3.Text = "Find";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.FindTexturesPath);
            // 
            // TexturesPath
            // 
            this.TexturesPath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.TexturesPath.Location = new System.Drawing.Point(102, 362);
            this.TexturesPath.Name = "TexturesPath";
            this.TexturesPath.Size = new System.Drawing.Size(234, 20);
            this.TexturesPath.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 365);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Textures path:";
            // 
            // ForceViewport
            // 
            this.ForceViewport.AutoSize = true;
            this.ForceViewport.Location = new System.Drawing.Point(19, 289);
            this.ForceViewport.Name = "ForceViewport";
            this.ForceViewport.Size = new System.Drawing.Size(119, 17);
            this.ForceViewport.TabIndex = 14;
            this.ForceViewport.Text = "Force viewport load";
            this.ForceViewport.UseVisualStyleBackColor = true;
            // 
            // button4
            // 
            this.button4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.button4.Location = new System.Drawing.Point(342, 308);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(42, 23);
            this.button4.TabIndex = 17;
            this.button4.Text = "Find";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.FindImagePath);
            // 
            // ImagePath
            // 
            this.ImagePath.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.ImagePath.Location = new System.Drawing.Point(102, 310);
            this.ImagePath.Name = "ImagePath";
            this.ImagePath.Size = new System.Drawing.Size(234, 20);
            this.ImagePath.TabIndex = 16;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 313);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(63, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Image path:";
            // 
            // Settings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(386, 412);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.ImagePath);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.ForceViewport);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.TexturesPath);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.FSLtxPath);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.NoCompressLinkLabel);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.GameMtlPath);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.Debug);
            this.Controls.Add(this.NoCompress);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Settings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Settings";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SaveParams);
            this.Load += new System.EventHandler(this.Settings_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.MotionFlagsGroupBox.ResumeLayout(false);
            this.MotionFlagsGroupBox.PerformLayout();
            this.SmoothTypeGroupBox.ResumeLayout(false);
            this.SmoothTypeGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox NoCompress;
        private System.Windows.Forms.CheckBox Debug;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton DefaultsCoP;
        private System.Windows.Forms.RadioButton DefaultsSoC;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox GameMtlPath;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.LinkLabel NoCompressLinkLabel;
        private System.Windows.Forms.CheckBox OptimizeSurfaces;
        private System.Windows.Forms.CheckBox StripifyMeshes;
        private System.Windows.Forms.CheckBox ProgressiveMeshes;
        private System.Windows.Forms.GroupBox SmoothTypeGroupBox;
        private System.Windows.Forms.RadioButton SmoothCoP;
        private System.Windows.Forms.RadioButton SmoothSoC;
        private System.Windows.Forms.GroupBox MotionFlagsGroupBox;
        private System.Windows.Forms.CheckBox BuildInMotionsExport;
        private System.Windows.Forms.RadioButton AnimsNoCompress;
        private System.Windows.Forms.RadioButton Anims8Bit;
        private System.Windows.Forms.RadioButton Anims16Bit;
        private System.Windows.Forms.CheckBox SoCInfluence;
        private System.Windows.Forms.CheckBox SplitNormalsChbx;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton HQGeometryPlus;
        private System.Windows.Forms.RadioButton HQGeometry;
        private System.Windows.Forms.CheckBox ScaleCenterOfMassCheckBox;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox FSLtxPath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox TexturesPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.RadioButton AutoSmooth;
        private System.Windows.Forms.CheckBox ForceViewport;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.TextBox ImagePath;
        private System.Windows.Forms.Label label4;
    }
}