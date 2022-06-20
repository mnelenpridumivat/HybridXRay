
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
            this.OpenObjectDialog = new System.Windows.Forms.OpenFileDialog();
            this.OpenSklsDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveSklsDialog = new System.Windows.Forms.SaveFileDialog();
            this.TabControl = new System.Windows.Forms.TabControl();
            this.FlagsPage = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.FlagsHelpButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.ShapesPage = new System.Windows.Forms.TabPage();
            this.MenuPanel = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.oGFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.oMFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sklSklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveSklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DeletesklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.shapeParamsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateShapesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.typeHelperToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allNoneToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.allBoxToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.allSphereToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.allCylinderToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.StatusPanel = new System.Windows.Forms.StatusStrip();
            this.FileLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.StatusFile = new System.Windows.Forms.ToolStripStatusLabel();
            this.bonesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenBonesDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveBonesDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveObjDialog = new System.Windows.Forms.SaveFileDialog();
            this.TabControl.SuspendLayout();
            this.FlagsPage.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.MenuPanel.SuspendLayout();
            this.StatusPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // OpenObjectDialog
            // 
            this.OpenObjectDialog.Filter = "Object file|*.object";
            // 
            // OpenSklsDialog
            // 
            this.OpenSklsDialog.Filter = "Skls file|*.skls";
            // 
            // SaveSklsDialog
            // 
            this.SaveSklsDialog.Filter = "Skls file|*.skls";
            // 
            // TabControl
            // 
            this.TabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.TabControl.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.TabControl.Controls.Add(this.FlagsPage);
            this.TabControl.Controls.Add(this.ShapesPage);
            this.TabControl.Location = new System.Drawing.Point(12, 27);
            this.TabControl.Name = "TabControl";
            this.TabControl.SelectedIndex = 0;
            this.TabControl.Size = new System.Drawing.Size(347, 166);
            this.TabControl.TabIndex = 10;
            this.TabControl.SelectedIndexChanged += new System.EventHandler(this.IndexChanged);
            // 
            // FlagsPage
            // 
            this.FlagsPage.Controls.Add(this.groupBox1);
            this.FlagsPage.Location = new System.Drawing.Point(4, 25);
            this.FlagsPage.Name = "FlagsPage";
            this.FlagsPage.Size = new System.Drawing.Size(339, 137);
            this.FlagsPage.TabIndex = 0;
            this.FlagsPage.Text = "Flags";
            this.FlagsPage.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)));
            this.groupBox1.Controls.Add(this.FlagsHelpButton);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.checkBox2);
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Controls.Add(this.checkBox1);
            this.groupBox1.Controls.Add(this.radioButton2);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.radioButton3);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(333, 131);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Edit export flags";
            // 
            // FlagsHelpButton
            // 
            this.FlagsHelpButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.FlagsHelpButton.Location = new System.Drawing.Point(252, 102);
            this.FlagsHelpButton.Name = "FlagsHelpButton";
            this.FlagsHelpButton.Size = new System.Drawing.Size(75, 23);
            this.FlagsHelpButton.TabIndex = 15;
            this.FlagsHelpButton.Text = "Help";
            this.FlagsHelpButton.UseVisualStyleBackColor = true;
            this.FlagsHelpButton.Click += new System.EventHandler(this.FlagsHelpButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(74, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Motion export:";
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Location = new System.Drawing.Point(167, 70);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(109, 17);
            this.checkBox2.TabIndex = 14;
            this.checkBox2.Text = "Optimize surfaces";
            this.checkBox2.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(19, 46);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(45, 17);
            this.radioButton1.TabIndex = 8;
            this.radioButton1.Text = "8 bit";
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(167, 47);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(142, 17);
            this.checkBox1.TabIndex = 13;
            this.checkBox1.Text = "Make progressive bones";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Checked = true;
            this.radioButton2.Location = new System.Drawing.Point(19, 69);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(51, 17);
            this.radioButton2.TabIndex = 9;
            this.radioButton2.TabStop = true;
            this.radioButton2.Text = "16 bit";
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(164, 24);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Model export:";
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(19, 92);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(110, 17);
            this.radioButton3.TabIndex = 10;
            this.radioButton3.Text = "Without compress";
            this.radioButton3.UseVisualStyleBackColor = true;
            // 
            // ShapesPage
            // 
            this.ShapesPage.AutoScroll = true;
            this.ShapesPage.Location = new System.Drawing.Point(4, 25);
            this.ShapesPage.Name = "ShapesPage";
            this.ShapesPage.Size = new System.Drawing.Size(339, 137);
            this.ShapesPage.TabIndex = 2;
            this.ShapesPage.Text = "Shapes";
            this.ShapesPage.UseVisualStyleBackColor = true;
            // 
            // MenuPanel
            // 
            this.MenuPanel.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.shapeParamsToolStripMenuItem});
            this.MenuPanel.Location = new System.Drawing.Point(0, 0);
            this.MenuPanel.Name = "MenuPanel";
            this.MenuPanel.Size = new System.Drawing.Size(372, 24);
            this.MenuPanel.TabIndex = 11;
            this.MenuPanel.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exportToolStripMenuItem,
            this.deleteToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.oGFToolStripMenuItem,
            this.oMFToolStripMenuItem,
            this.objToolStripMenuItem});
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.exportToolStripMenuItem.Text = "Export";
            // 
            // oGFToolStripMenuItem
            // 
            this.oGFToolStripMenuItem.Name = "oGFToolStripMenuItem";
            this.oGFToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.oGFToolStripMenuItem.Text = "OGF";
            this.oGFToolStripMenuItem.Click += new System.EventHandler(this.ExportOGF_Click);
            // 
            // oMFToolStripMenuItem
            // 
            this.oMFToolStripMenuItem.Name = "oMFToolStripMenuItem";
            this.oMFToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.oMFToolStripMenuItem.Text = "OMF";
            this.oMFToolStripMenuItem.Click += new System.EventHandler(this.ExportOMF_Click);
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.objectToolStripMenuItem,
            this.sklSklsToolStripMenuItem,
            this.bonesToolStripMenuItem});
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.loadToolStripMenuItem.Text = "Load";
            // 
            // objectToolStripMenuItem
            // 
            this.objectToolStripMenuItem.Name = "objectToolStripMenuItem";
            this.objectToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.objectToolStripMenuItem.Text = "Object";
            this.objectToolStripMenuItem.Click += new System.EventHandler(this.objectToolStripMenuItem_Click);
            // 
            // sklSklsToolStripMenuItem
            // 
            this.sklSklsToolStripMenuItem.Name = "sklSklsToolStripMenuItem";
            this.sklSklsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.sklSklsToolStripMenuItem.Text = "Skl\\Skls";
            this.sklSklsToolStripMenuItem.Click += new System.EventHandler(this.LoadSkls_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.objectToolStripMenuItem1,
            this.SaveSklsToolStripMenuItem,
            this.bonesToolStripMenuItem1});
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveToolStripMenuItem.Text = "Save";
            // 
            // objectToolStripMenuItem1
            // 
            this.objectToolStripMenuItem1.Name = "objectToolStripMenuItem1";
            this.objectToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.objectToolStripMenuItem1.Text = "Object";
            this.objectToolStripMenuItem1.Click += new System.EventHandler(this.objectToolStripMenuItem1_Click);
            // 
            // SaveSklsToolStripMenuItem
            // 
            this.SaveSklsToolStripMenuItem.Name = "SaveSklsToolStripMenuItem";
            this.SaveSklsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.SaveSklsToolStripMenuItem.Text = "Skls";
            this.SaveSklsToolStripMenuItem.Click += new System.EventHandler(this.SaveMotionsButton_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.DeletesklsToolStripMenuItem});
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            // 
            // DeletesklsToolStripMenuItem
            // 
            this.DeletesklsToolStripMenuItem.Name = "DeletesklsToolStripMenuItem";
            this.DeletesklsToolStripMenuItem.Size = new System.Drawing.Size(94, 22);
            this.DeletesklsToolStripMenuItem.Text = "Skls";
            this.DeletesklsToolStripMenuItem.Click += new System.EventHandler(this.DeleteMotionsButton_Click);
            // 
            // shapeParamsToolStripMenuItem
            // 
            this.shapeParamsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateShapesToolStripMenuItem,
            this.typeHelperToolStripMenuItem});
            this.shapeParamsToolStripMenuItem.Name = "shapeParamsToolStripMenuItem";
            this.shapeParamsToolStripMenuItem.Size = new System.Drawing.Size(93, 20);
            this.shapeParamsToolStripMenuItem.Text = "Shape Params";
            // 
            // generateShapesToolStripMenuItem
            // 
            this.generateShapesToolStripMenuItem.Name = "generateShapesToolStripMenuItem";
            this.generateShapesToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.generateShapesToolStripMenuItem.Text = "Generate shapes";
            this.generateShapesToolStripMenuItem.Click += new System.EventHandler(this.generateShapesToolStripMenuItem_Click);
            // 
            // typeHelperToolStripMenuItem
            // 
            this.typeHelperToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.allNoneToolStripMenuItem1,
            this.allBoxToolStripMenuItem1,
            this.allSphereToolStripMenuItem1,
            this.allCylinderToolStripMenuItem1});
            this.typeHelperToolStripMenuItem.Name = "typeHelperToolStripMenuItem";
            this.typeHelperToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.typeHelperToolStripMenuItem.Text = "Type helper";
            // 
            // allNoneToolStripMenuItem1
            // 
            this.allNoneToolStripMenuItem1.Name = "allNoneToolStripMenuItem1";
            this.allNoneToolStripMenuItem1.Size = new System.Drawing.Size(135, 22);
            this.allNoneToolStripMenuItem1.Text = "All None";
            this.allNoneToolStripMenuItem1.Click += new System.EventHandler(this.allNoneToolStripMenuItem_Click);
            // 
            // allBoxToolStripMenuItem1
            // 
            this.allBoxToolStripMenuItem1.Name = "allBoxToolStripMenuItem1";
            this.allBoxToolStripMenuItem1.Size = new System.Drawing.Size(135, 22);
            this.allBoxToolStripMenuItem1.Text = "All Box";
            this.allBoxToolStripMenuItem1.Click += new System.EventHandler(this.allBoxToolStripMenuItem_Click);
            // 
            // allSphereToolStripMenuItem1
            // 
            this.allSphereToolStripMenuItem1.Name = "allSphereToolStripMenuItem1";
            this.allSphereToolStripMenuItem1.Size = new System.Drawing.Size(135, 22);
            this.allSphereToolStripMenuItem1.Text = "All Sphere";
            this.allSphereToolStripMenuItem1.Click += new System.EventHandler(this.allSphereToolStripMenuItem_Click);
            // 
            // allCylinderToolStripMenuItem1
            // 
            this.allCylinderToolStripMenuItem1.Name = "allCylinderToolStripMenuItem1";
            this.allCylinderToolStripMenuItem1.Size = new System.Drawing.Size(135, 22);
            this.allCylinderToolStripMenuItem1.Text = "All Cylinder";
            this.allCylinderToolStripMenuItem1.Click += new System.EventHandler(this.allCylinderToolStripMenuItem_Click);
            // 
            // StatusPanel
            // 
            this.StatusPanel.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileLabel,
            this.StatusFile});
            this.StatusPanel.Location = new System.Drawing.Point(0, 194);
            this.StatusPanel.Name = "StatusPanel";
            this.StatusPanel.Size = new System.Drawing.Size(372, 22);
            this.StatusPanel.TabIndex = 31;
            this.StatusPanel.Text = "statusStrip1";
            // 
            // FileLabel
            // 
            this.FileLabel.Name = "FileLabel";
            this.FileLabel.Size = new System.Drawing.Size(28, 17);
            this.FileLabel.Text = "File:";
            // 
            // StatusFile
            // 
            this.StatusFile.Name = "StatusFile";
            this.StatusFile.Size = new System.Drawing.Size(12, 17);
            this.StatusFile.Text = "-";
            // 
            // bonesToolStripMenuItem
            // 
            this.bonesToolStripMenuItem.Name = "bonesToolStripMenuItem";
            this.bonesToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.bonesToolStripMenuItem.Text = "Bones";
            this.bonesToolStripMenuItem.Click += new System.EventHandler(this.bonesToolStripMenuItem_Click);
            // 
            // objToolStripMenuItem
            // 
            this.objToolStripMenuItem.Name = "objToolStripMenuItem";
            this.objToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.objToolStripMenuItem.Text = "Obj";
            this.objToolStripMenuItem.Click += new System.EventHandler(this.objToolStripMenuItem_Click);
            // 
            // bonesToolStripMenuItem1
            // 
            this.bonesToolStripMenuItem1.Name = "bonesToolStripMenuItem1";
            this.bonesToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.bonesToolStripMenuItem1.Text = "Bones";
            this.bonesToolStripMenuItem1.Click += new System.EventHandler(this.bonesToolStripMenuItem1_Click);
            // 
            // OpenBonesDialog
            // 
            this.OpenBonesDialog.Filter = "Bones file|*.bones";
            // 
            // SaveBonesDialog
            // 
            this.SaveBonesDialog.Filter = "Bones file|*.bones";
            // 
            // SaveObjDialog
            // 
            this.SaveObjDialog.Filter = "Wavefront Obj file|*.obj";
            // 
            // Object_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(372, 216);
            this.Controls.Add(this.StatusPanel);
            this.Controls.Add(this.TabControl);
            this.Controls.Add(this.MenuPanel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.MenuPanel;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(388, 540);
            this.MinimumSize = new System.Drawing.Size(388, 255);
            this.Name = "Object_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Object Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ClosingForm);
            this.TabControl.ResumeLayout(false);
            this.FlagsPage.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.MenuPanel.ResumeLayout(false);
            this.MenuPanel.PerformLayout();
            this.StatusPanel.ResumeLayout(false);
            this.StatusPanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog OpenObjectDialog;
        private System.Windows.Forms.OpenFileDialog OpenSklsDialog;
        private System.Windows.Forms.SaveFileDialog SaveSklsDialog;
        private System.Windows.Forms.TabControl TabControl;
        private System.Windows.Forms.TabPage FlagsPage;
        private System.Windows.Forms.TabPage ShapesPage;
        private System.Windows.Forms.MenuStrip MenuPanel;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem oGFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem oMFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveSklsToolStripMenuItem;
        private System.Windows.Forms.Button FlagsHelpButton;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.StatusStrip StatusPanel;
        private System.Windows.Forms.ToolStripStatusLabel FileLabel;
        private System.Windows.Forms.ToolStripStatusLabel StatusFile;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sklSklsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem DeletesklsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem shapeParamsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateShapesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem typeHelperToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allNoneToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem allBoxToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem allSphereToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem allCylinderToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem bonesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bonesToolStripMenuItem1;
        private System.Windows.Forms.OpenFileDialog OpenBonesDialog;
        private System.Windows.Forms.SaveFileDialog SaveBonesDialog;
        private System.Windows.Forms.SaveFileDialog SaveObjDialog;
    }
}

