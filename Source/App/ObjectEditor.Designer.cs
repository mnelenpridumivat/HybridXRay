
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
            this.MenuPanel = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sklSklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesPartsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.sklToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveSklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesPartsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.oGFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.oMFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dMToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DeletesklsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bonesPartsToDefaultToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.surfaceParamsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.enableAll2SidedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.disableAll2SidedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.shapeParamsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateShapesToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.typeHelperToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.allNoneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allBoxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allSphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.allCylinderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateLodToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.StatusPanel = new System.Windows.Forms.StatusStrip();
            this.FileLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.StatusFile = new System.Windows.Forms.ToolStripStatusLabel();
            this.OpenBonesDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveBonesDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveObjDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveDmDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveOgfDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveOmfDialog = new System.Windows.Forms.SaveFileDialog();
            this.OpenOgfDialog = new System.Windows.Forms.OpenFileDialog();
            this.OpenLtxDialog = new System.Windows.Forms.OpenFileDialog();
            this.SaveLtxDialog = new System.Windows.Forms.SaveFileDialog();
            this.BonesPage = new System.Windows.Forms.TabPage();
            this.FlagsPage = new System.Windows.Forms.TabPage();
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
            this.TabControl = new System.Windows.Forms.TabControl();
            this.SurfacesPage = new System.Windows.Forms.TabPage();
            this.MotionPage = new System.Windows.Forms.TabPage();
            this.MotionTextBox = new System.Windows.Forms.RichTextBox();
            this.MotionRefsPage = new System.Windows.Forms.TabPage();
            this.MotionRefsBox = new System.Windows.Forms.RichTextBox();
            this.UserDataPage = new System.Windows.Forms.TabPage();
            this.UserDataTextBox = new System.Windows.Forms.RichTextBox();
            this.LodPage = new System.Windows.Forms.TabPage();
            this.LodTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SaveObjectDialog = new System.Windows.Forms.SaveFileDialog();
            this.SaveOgfLodDialog = new System.Windows.Forms.SaveFileDialog();
            this.cToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveCppDialog = new System.Windows.Forms.SaveFileDialog();
            this.batchConvertToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fromLtxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fromDialogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenBatchLtxDialog = new System.Windows.Forms.OpenFileDialog();
            this.OpenBatchDialog = new System.Windows.Forms.OpenFileDialog();
            this.BatchHelpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.MenuPanel.SuspendLayout();
            this.StatusPanel.SuspendLayout();
            this.FlagsPage.SuspendLayout();
            this.FlagsGroupBox.SuspendLayout();
            this.MotionFlagsGroupBox.SuspendLayout();
            this.ModelFlagsGroupBox.SuspendLayout();
            this.TabControl.SuspendLayout();
            this.MotionPage.SuspendLayout();
            this.MotionRefsPage.SuspendLayout();
            this.UserDataPage.SuspendLayout();
            this.LodPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // OpenObjectDialog
            // 
            this.OpenObjectDialog.Filter = "Object file|*.object";
            // 
            // OpenSklsDialog
            // 
            this.OpenSklsDialog.Filter = "Skl\\Skls file|*.skls;*.skl";
            this.OpenSklsDialog.Multiselect = true;
            // 
            // SaveSklsDialog
            // 
            this.SaveSklsDialog.Filter = "Skls file|*.skls";
            // 
            // MenuPanel
            // 
            this.MenuPanel.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.objectInfoToolStripMenuItem,
            this.debugToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.MenuPanel.Location = new System.Drawing.Point(0, 0);
            this.MenuPanel.Name = "MenuPanel";
            this.MenuPanel.Size = new System.Drawing.Size(401, 24);
            this.MenuPanel.TabIndex = 11;
            this.MenuPanel.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exportToolStripMenuItem,
            this.batchConvertToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadToolStripMenuItem
            // 
            this.loadToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.objectToolStripMenuItem,
            this.sklSklsToolStripMenuItem,
            this.bonesToolStripMenuItem,
            this.bonesPartsToolStripMenuItem});
            this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
            this.loadToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.loadToolStripMenuItem.Text = "Load";
            // 
            // objectToolStripMenuItem
            // 
            this.objectToolStripMenuItem.Name = "objectToolStripMenuItem";
            this.objectToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.objectToolStripMenuItem.Text = "Object";
            this.objectToolStripMenuItem.Click += new System.EventHandler(this.objectToolStripMenuItem_Click);
            // 
            // sklSklsToolStripMenuItem
            // 
            this.sklSklsToolStripMenuItem.Name = "sklSklsToolStripMenuItem";
            this.sklSklsToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.sklSklsToolStripMenuItem.Text = "Skl\\Skls";
            this.sklSklsToolStripMenuItem.Click += new System.EventHandler(this.LoadSkls_Click);
            // 
            // bonesToolStripMenuItem
            // 
            this.bonesToolStripMenuItem.Name = "bonesToolStripMenuItem";
            this.bonesToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.bonesToolStripMenuItem.Text = "Bones";
            this.bonesToolStripMenuItem.Click += new System.EventHandler(this.bonesToolStripMenuItem_Click);
            // 
            // bonesPartsToolStripMenuItem
            // 
            this.bonesPartsToolStripMenuItem.Name = "bonesPartsToolStripMenuItem";
            this.bonesPartsToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            this.bonesPartsToolStripMenuItem.Text = "Bone parts";
            this.bonesPartsToolStripMenuItem.Click += new System.EventHandler(this.bonesPartsToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.objectToolStripMenuItem1,
            this.sklToolStripMenuItem,
            this.SaveSklsToolStripMenuItem,
            this.bonesToolStripMenuItem1,
            this.bonesPartsToolStripMenuItem1});
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
            // sklToolStripMenuItem
            // 
            this.sklToolStripMenuItem.Name = "sklToolStripMenuItem";
            this.sklToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.sklToolStripMenuItem.Text = "Skl";
            this.sklToolStripMenuItem.Click += new System.EventHandler(this.sklToolStripMenuItem_Click);
            // 
            // SaveSklsToolStripMenuItem
            // 
            this.SaveSklsToolStripMenuItem.Name = "SaveSklsToolStripMenuItem";
            this.SaveSklsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.SaveSklsToolStripMenuItem.Text = "Skls";
            this.SaveSklsToolStripMenuItem.Click += new System.EventHandler(this.SaveMotionsButton_Click);
            // 
            // bonesToolStripMenuItem1
            // 
            this.bonesToolStripMenuItem1.Name = "bonesToolStripMenuItem1";
            this.bonesToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.bonesToolStripMenuItem1.Text = "Bones";
            this.bonesToolStripMenuItem1.Click += new System.EventHandler(this.bonesToolStripMenuItem1_Click);
            // 
            // bonesPartsToolStripMenuItem1
            // 
            this.bonesPartsToolStripMenuItem1.Name = "bonesPartsToolStripMenuItem1";
            this.bonesPartsToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.bonesPartsToolStripMenuItem1.Text = "Bone parts";
            this.bonesPartsToolStripMenuItem1.Click += new System.EventHandler(this.bonesPartsToolStripMenuItem1_Click);
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.oGFToolStripMenuItem,
            this.oMFToolStripMenuItem,
            this.objToolStripMenuItem,
            this.dMToolStripMenuItem,
            this.cToolStripMenuItem});
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
            // objToolStripMenuItem
            // 
            this.objToolStripMenuItem.Name = "objToolStripMenuItem";
            this.objToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.objToolStripMenuItem.Text = "Obj";
            this.objToolStripMenuItem.Click += new System.EventHandler(this.objToolStripMenuItem_Click);
            // 
            // dMToolStripMenuItem
            // 
            this.dMToolStripMenuItem.Name = "dMToolStripMenuItem";
            this.dMToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.dMToolStripMenuItem.Text = "DM";
            this.dMToolStripMenuItem.Click += new System.EventHandler(this.dMToolStripMenuItem_Click_1);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.DeletesklsToolStripMenuItem,
            this.bonesPartsToDefaultToolStripMenuItem});
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            // 
            // DeletesklsToolStripMenuItem
            // 
            this.DeletesklsToolStripMenuItem.Name = "DeletesklsToolStripMenuItem";
            this.DeletesklsToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.DeletesklsToolStripMenuItem.Text = "Skls";
            this.DeletesklsToolStripMenuItem.Click += new System.EventHandler(this.DeleteMotionsButton_Click);
            // 
            // bonesPartsToDefaultToolStripMenuItem
            // 
            this.bonesPartsToDefaultToolStripMenuItem.Name = "bonesPartsToDefaultToolStripMenuItem";
            this.bonesPartsToDefaultToolStripMenuItem.Size = new System.Drawing.Size(184, 22);
            this.bonesPartsToDefaultToolStripMenuItem.Text = "Bone parts to default";
            this.bonesPartsToDefaultToolStripMenuItem.Click += new System.EventHandler(this.bonesPartsToDefaultToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(177, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.surfaceParamsToolStripMenuItem,
            this.shapeParamsToolStripMenuItem,
            this.generateLodToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(46, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // surfaceParamsToolStripMenuItem
            // 
            this.surfaceParamsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.enableAll2SidedToolStripMenuItem,
            this.disableAll2SidedToolStripMenuItem});
            this.surfaceParamsToolStripMenuItem.Name = "surfaceParamsToolStripMenuItem";
            this.surfaceParamsToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.surfaceParamsToolStripMenuItem.Text = "Surface Params";
            // 
            // enableAll2SidedToolStripMenuItem
            // 
            this.enableAll2SidedToolStripMenuItem.Name = "enableAll2SidedToolStripMenuItem";
            this.enableAll2SidedToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.enableAll2SidedToolStripMenuItem.Text = "Enable all 2 sided";
            this.enableAll2SidedToolStripMenuItem.Click += new System.EventHandler(this.enableAll2SidedToolStripMenuItem_Click);
            // 
            // disableAll2SidedToolStripMenuItem
            // 
            this.disableAll2SidedToolStripMenuItem.Name = "disableAll2SidedToolStripMenuItem";
            this.disableAll2SidedToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.disableAll2SidedToolStripMenuItem.Text = "Disable all 2 sided";
            this.disableAll2SidedToolStripMenuItem.Click += new System.EventHandler(this.disableAll2SidedToolStripMenuItem_Click);
            // 
            // shapeParamsToolStripMenuItem
            // 
            this.shapeParamsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateShapesToolStripMenuItem1,
            this.typeHelperToolStripMenuItem1});
            this.shapeParamsToolStripMenuItem.Name = "shapeParamsToolStripMenuItem";
            this.shapeParamsToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.shapeParamsToolStripMenuItem.Text = "Shape Params";
            // 
            // generateShapesToolStripMenuItem1
            // 
            this.generateShapesToolStripMenuItem1.Name = "generateShapesToolStripMenuItem1";
            this.generateShapesToolStripMenuItem1.Size = new System.Drawing.Size(161, 22);
            this.generateShapesToolStripMenuItem1.Text = "Generate Shapes";
            this.generateShapesToolStripMenuItem1.Click += new System.EventHandler(this.generateShapesToolStripMenuItem_Click);
            // 
            // typeHelperToolStripMenuItem1
            // 
            this.typeHelperToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.allNoneToolStripMenuItem,
            this.allBoxToolStripMenuItem,
            this.allSphereToolStripMenuItem,
            this.allCylinderToolStripMenuItem});
            this.typeHelperToolStripMenuItem1.Name = "typeHelperToolStripMenuItem1";
            this.typeHelperToolStripMenuItem1.Size = new System.Drawing.Size(161, 22);
            this.typeHelperToolStripMenuItem1.Text = "Type helper";
            // 
            // allNoneToolStripMenuItem
            // 
            this.allNoneToolStripMenuItem.Name = "allNoneToolStripMenuItem";
            this.allNoneToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.allNoneToolStripMenuItem.Text = "All None";
            this.allNoneToolStripMenuItem.Click += new System.EventHandler(this.allNoneToolStripMenuItem_Click);
            // 
            // allBoxToolStripMenuItem
            // 
            this.allBoxToolStripMenuItem.Name = "allBoxToolStripMenuItem";
            this.allBoxToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.allBoxToolStripMenuItem.Text = "All Box";
            this.allBoxToolStripMenuItem.Click += new System.EventHandler(this.allBoxToolStripMenuItem_Click);
            // 
            // allSphereToolStripMenuItem
            // 
            this.allSphereToolStripMenuItem.Name = "allSphereToolStripMenuItem";
            this.allSphereToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.allSphereToolStripMenuItem.Text = "All Sphere";
            this.allSphereToolStripMenuItem.Click += new System.EventHandler(this.allSphereToolStripMenuItem_Click);
            // 
            // allCylinderToolStripMenuItem
            // 
            this.allCylinderToolStripMenuItem.Name = "allCylinderToolStripMenuItem";
            this.allCylinderToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.allCylinderToolStripMenuItem.Text = "All Cylinder";
            this.allCylinderToolStripMenuItem.Click += new System.EventHandler(this.allCylinderToolStripMenuItem_Click);
            // 
            // generateLodToolStripMenuItem
            // 
            this.generateLodToolStripMenuItem.Name = "generateLodToolStripMenuItem";
            this.generateLodToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.generateLodToolStripMenuItem.Text = "Generate lod";
            this.generateLodToolStripMenuItem.Click += new System.EventHandler(this.generateLodToolStripMenuItem_Click);
            // 
            // debugToolStripMenuItem
            // 
            this.debugToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openLogToolStripMenuItem,
            this.showWindowToolStripMenuItem});
            this.debugToolStripMenuItem.Name = "debugToolStripMenuItem";
            this.debugToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.debugToolStripMenuItem.Text = "Debug";
            // 
            // openLogToolStripMenuItem
            // 
            this.openLogToolStripMenuItem.Name = "openLogToolStripMenuItem";
            this.openLogToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.openLogToolStripMenuItem.Text = "Open log";
            this.openLogToolStripMenuItem.Click += new System.EventHandler(this.openLogToolStripMenuItem_Click);
            // 
            // showWindowToolStripMenuItem
            // 
            this.showWindowToolStripMenuItem.Name = "showWindowToolStripMenuItem";
            this.showWindowToolStripMenuItem.Size = new System.Drawing.Size(185, 22);
            this.showWindowToolStripMenuItem.Text = "Show debug window";
            this.showWindowToolStripMenuItem.Click += new System.EventHandler(this.showWindowToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            this.helpToolStripMenuItem.Click += new System.EventHandler(this.FlagsHelpButton_Click);
            // 
            // StatusPanel
            // 
            this.StatusPanel.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileLabel,
            this.StatusFile});
            this.StatusPanel.Location = new System.Drawing.Point(0, 217);
            this.StatusPanel.Name = "StatusPanel";
            this.StatusPanel.Size = new System.Drawing.Size(401, 22);
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
            this.SaveObjDialog.Filter = "Obj file|*.obj";
            // 
            // SaveDmDialog
            // 
            this.SaveDmDialog.Filter = "DM file|*.dm";
            // 
            // SaveOgfDialog
            // 
            this.SaveOgfDialog.Filter = "OGF file|*.ogf";
            // 
            // SaveOmfDialog
            // 
            this.SaveOmfDialog.Filter = "OMF file|*.omf";
            // 
            // OpenOgfDialog
            // 
            this.OpenOgfDialog.Filter = "OGF file|*.ogf";
            // 
            // OpenLtxDialog
            // 
            this.OpenLtxDialog.Filter = "LTX file|*.ltx";
            // 
            // SaveLtxDialog
            // 
            this.SaveLtxDialog.Filter = "LTX file|*.ltx";
            // 
            // BonesPage
            // 
            this.BonesPage.AutoScroll = true;
            this.BonesPage.Location = new System.Drawing.Point(4, 25);
            this.BonesPage.Name = "BonesPage";
            this.BonesPage.Size = new System.Drawing.Size(369, 158);
            this.BonesPage.TabIndex = 2;
            this.BonesPage.Text = "Bones";
            this.BonesPage.UseVisualStyleBackColor = true;
            // 
            // FlagsPage
            // 
            this.FlagsPage.Controls.Add(this.FlagsGroupBox);
            this.FlagsPage.Location = new System.Drawing.Point(4, 25);
            this.FlagsPage.Name = "FlagsPage";
            this.FlagsPage.Size = new System.Drawing.Size(369, 158);
            this.FlagsPage.TabIndex = 0;
            this.FlagsPage.Text = "Flags";
            this.FlagsPage.UseVisualStyleBackColor = true;
            // 
            // FlagsGroupBox
            // 
            this.FlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FlagsGroupBox.Controls.Add(this.MotionFlagsGroupBox);
            this.FlagsGroupBox.Controls.Add(this.ModelFlagsGroupBox);
            this.FlagsGroupBox.Controls.Add(this.ObjectScaleLabel);
            this.FlagsGroupBox.Controls.Add(this.ScaleCenterOfMassCheckBox);
            this.FlagsGroupBox.Controls.Add(this.ObjectScaleTextBox);
            this.FlagsGroupBox.Location = new System.Drawing.Point(0, 0);
            this.FlagsGroupBox.Name = "FlagsGroupBox";
            this.FlagsGroupBox.Size = new System.Drawing.Size(369, 158);
            this.FlagsGroupBox.TabIndex = 16;
            this.FlagsGroupBox.TabStop = false;
            this.FlagsGroupBox.Text = "Edit export flags";
            // 
            // MotionFlagsGroupBox
            // 
            this.MotionFlagsGroupBox.Controls.Add(this.AnimsNoCompress);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims8Bit);
            this.MotionFlagsGroupBox.Controls.Add(this.Anims16Bit);
            this.MotionFlagsGroupBox.Location = new System.Drawing.Point(6, 15);
            this.MotionFlagsGroupBox.Name = "MotionFlagsGroupBox";
            this.MotionFlagsGroupBox.Size = new System.Drawing.Size(175, 90);
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
            this.ModelFlagsGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ModelFlagsGroupBox.Controls.Add(this.StripifyMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometry);
            this.ModelFlagsGroupBox.Controls.Add(this.HQGeometryPlus);
            this.ModelFlagsGroupBox.Controls.Add(this.ProgressiveMeshes);
            this.ModelFlagsGroupBox.Controls.Add(this.checkBox2);
            this.ModelFlagsGroupBox.Location = new System.Drawing.Point(188, 15);
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
            this.ObjectScaleTextBox.Location = new System.Drawing.Point(78, 109);
            this.ObjectScaleTextBox.Name = "ObjectScaleTextBox";
            this.ObjectScaleTextBox.Size = new System.Drawing.Size(103, 20);
            this.ObjectScaleTextBox.TabIndex = 16;
            this.ObjectScaleTextBox.Text = "1";
            this.ObjectScaleTextBox.TextChanged += new System.EventHandler(this.ScaleTextChanged);
            this.ObjectScaleTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ScaleKeyDown);
            this.ObjectScaleTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ScaleKeyPress);
            // 
            // TabControl
            // 
            this.TabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TabControl.Appearance = System.Windows.Forms.TabAppearance.Buttons;
            this.TabControl.Controls.Add(this.FlagsPage);
            this.TabControl.Controls.Add(this.SurfacesPage);
            this.TabControl.Controls.Add(this.BonesPage);
            this.TabControl.Controls.Add(this.MotionPage);
            this.TabControl.Controls.Add(this.MotionRefsPage);
            this.TabControl.Controls.Add(this.UserDataPage);
            this.TabControl.Controls.Add(this.LodPage);
            this.TabControl.Location = new System.Drawing.Point(12, 27);
            this.TabControl.Name = "TabControl";
            this.TabControl.SelectedIndex = 0;
            this.TabControl.Size = new System.Drawing.Size(377, 187);
            this.TabControl.TabIndex = 10;
            this.TabControl.SelectedIndexChanged += new System.EventHandler(this.IndexChanged);
            this.TabControl.KeyDown += new System.Windows.Forms.KeyEventHandler(this.EditorKeyDown);
            // 
            // SurfacesPage
            // 
            this.SurfacesPage.AutoScroll = true;
            this.SurfacesPage.Location = new System.Drawing.Point(4, 25);
            this.SurfacesPage.Name = "SurfacesPage";
            this.SurfacesPage.Size = new System.Drawing.Size(369, 158);
            this.SurfacesPage.TabIndex = 3;
            this.SurfacesPage.Text = "Surfaces";
            this.SurfacesPage.UseVisualStyleBackColor = true;
            // 
            // MotionPage
            // 
            this.MotionPage.Controls.Add(this.MotionTextBox);
            this.MotionPage.Location = new System.Drawing.Point(4, 25);
            this.MotionPage.Name = "MotionPage";
            this.MotionPage.Size = new System.Drawing.Size(369, 158);
            this.MotionPage.TabIndex = 4;
            this.MotionPage.Text = "Motions";
            this.MotionPage.UseVisualStyleBackColor = true;
            // 
            // MotionTextBox
            // 
            this.MotionTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MotionTextBox.Location = new System.Drawing.Point(0, 0);
            this.MotionTextBox.Name = "MotionTextBox";
            this.MotionTextBox.ReadOnly = true;
            this.MotionTextBox.Size = new System.Drawing.Size(369, 155);
            this.MotionTextBox.TabIndex = 0;
            this.MotionTextBox.Text = "";
            // 
            // MotionRefsPage
            // 
            this.MotionRefsPage.Controls.Add(this.MotionRefsBox);
            this.MotionRefsPage.Location = new System.Drawing.Point(4, 25);
            this.MotionRefsPage.Name = "MotionRefsPage";
            this.MotionRefsPage.Size = new System.Drawing.Size(369, 158);
            this.MotionRefsPage.TabIndex = 7;
            this.MotionRefsPage.Text = "Motion Refs";
            this.MotionRefsPage.UseVisualStyleBackColor = true;
            // 
            // MotionRefsBox
            // 
            this.MotionRefsBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.MotionRefsBox.Location = new System.Drawing.Point(0, 0);
            this.MotionRefsBox.Name = "MotionRefsBox";
            this.MotionRefsBox.Size = new System.Drawing.Size(369, 158);
            this.MotionRefsBox.TabIndex = 0;
            this.MotionRefsBox.Text = "";
            this.MotionRefsBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.RichTextBoxImgDefender);
            // 
            // UserDataPage
            // 
            this.UserDataPage.Controls.Add(this.UserDataTextBox);
            this.UserDataPage.Location = new System.Drawing.Point(4, 25);
            this.UserDataPage.Name = "UserDataPage";
            this.UserDataPage.Size = new System.Drawing.Size(369, 158);
            this.UserDataPage.TabIndex = 6;
            this.UserDataPage.Text = "UserData";
            this.UserDataPage.UseVisualStyleBackColor = true;
            // 
            // UserDataTextBox
            // 
            this.UserDataTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.UserDataTextBox.Location = new System.Drawing.Point(0, 0);
            this.UserDataTextBox.Name = "UserDataTextBox";
            this.UserDataTextBox.Size = new System.Drawing.Size(369, 158);
            this.UserDataTextBox.TabIndex = 0;
            this.UserDataTextBox.Text = "";
            this.UserDataTextBox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.RichTextBoxImgDefender);
            // 
            // LodPage
            // 
            this.LodPage.Controls.Add(this.LodTextBox);
            this.LodPage.Controls.Add(this.label1);
            this.LodPage.Location = new System.Drawing.Point(4, 25);
            this.LodPage.Name = "LodPage";
            this.LodPage.Size = new System.Drawing.Size(369, 158);
            this.LodPage.TabIndex = 5;
            this.LodPage.Text = "Lod";
            this.LodPage.UseVisualStyleBackColor = true;
            // 
            // LodTextBox
            // 
            this.LodTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LodTextBox.Location = new System.Drawing.Point(6, 26);
            this.LodTextBox.Name = "LodTextBox";
            this.LodTextBox.Size = new System.Drawing.Size(357, 20);
            this.LodTextBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Lod path:";
            // 
            // SaveObjectDialog
            // 
            this.SaveObjectDialog.Filter = "Object file|*.object";
            // 
            // SaveOgfLodDialog
            // 
            this.SaveOgfLodDialog.Filter = "OGF file|*.ogf";
            // 
            // cToolStripMenuItem
            // 
            this.cToolStripMenuItem.Name = "cToolStripMenuItem";
            this.cToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.cToolStripMenuItem.Text = "C++";
            this.cToolStripMenuItem.Click += new System.EventHandler(this.cToolStripMenuItem_Click);
            // 
            // SaveCppDialog
            // 
            this.SaveCppDialog.Filter = "LTX file|*.ltx";
            // 
            // batchConvertToolStripMenuItem
            // 
            this.batchConvertToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fromLtxToolStripMenuItem,
            this.fromDialogToolStripMenuItem,
            this.BatchHelpToolStripMenuItem});
            this.batchConvertToolStripMenuItem.Name = "batchConvertToolStripMenuItem";
            this.batchConvertToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.batchConvertToolStripMenuItem.Text = "Batch Convert";
            // 
            // fromLtxToolStripMenuItem
            // 
            this.fromLtxToolStripMenuItem.Name = "fromLtxToolStripMenuItem";
            this.fromLtxToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.fromLtxToolStripMenuItem.Text = "From Ltx";
            this.fromLtxToolStripMenuItem.Click += new System.EventHandler(this.fromLtxToolStripMenuItem_Click);
            // 
            // fromDialogToolStripMenuItem
            // 
            this.fromDialogToolStripMenuItem.Name = "fromDialogToolStripMenuItem";
            this.fromDialogToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.fromDialogToolStripMenuItem.Text = "From Dialog";
            this.fromDialogToolStripMenuItem.Click += new System.EventHandler(this.fromDialogToolStripMenuItem_Click);
            // 
            // OpenBatchLtxDialog
            // 
            this.OpenBatchLtxDialog.Filter = "LTX file|*.ltx";
            // 
            // OpenBatchDialog
            // 
            this.OpenBatchDialog.Filter = "Object file|*.object";
            this.OpenBatchDialog.Multiselect = true;
            // 
            // BatchHelpToolStripMenuItem
            // 
            this.BatchHelpToolStripMenuItem.Name = "BatchHelpToolStripMenuItem";
            this.BatchHelpToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.BatchHelpToolStripMenuItem.Text = "Batch Help";
            this.BatchHelpToolStripMenuItem.Click += new System.EventHandler(this.ltxHelpToolStripMenuItem_Click);
            // 
            // objectInfoToolStripMenuItem
            // 
            this.objectInfoToolStripMenuItem.Name = "objectInfoToolStripMenuItem";
            this.objectInfoToolStripMenuItem.Size = new System.Drawing.Size(78, 20);
            this.objectInfoToolStripMenuItem.Text = "Object Info";
            this.objectInfoToolStripMenuItem.Click += new System.EventHandler(this.objectInfoToolStripMenuItem_Click);
            // 
            // Object_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(401, 239);
            this.Controls.Add(this.StatusPanel);
            this.Controls.Add(this.TabControl);
            this.Controls.Add(this.MenuPanel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.MenuPanel;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(417, 680);
            this.MinimumSize = new System.Drawing.Size(417, 278);
            this.Name = "Object_Editor";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Object Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ClosingForm);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.EditorKeyDown);
            this.MenuPanel.ResumeLayout(false);
            this.MenuPanel.PerformLayout();
            this.StatusPanel.ResumeLayout(false);
            this.StatusPanel.PerformLayout();
            this.FlagsPage.ResumeLayout(false);
            this.FlagsGroupBox.ResumeLayout(false);
            this.FlagsGroupBox.PerformLayout();
            this.MotionFlagsGroupBox.ResumeLayout(false);
            this.MotionFlagsGroupBox.PerformLayout();
            this.ModelFlagsGroupBox.ResumeLayout(false);
            this.ModelFlagsGroupBox.PerformLayout();
            this.TabControl.ResumeLayout(false);
            this.MotionPage.ResumeLayout(false);
            this.MotionRefsPage.ResumeLayout(false);
            this.UserDataPage.ResumeLayout(false);
            this.LodPage.ResumeLayout(false);
            this.LodPage.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog OpenObjectDialog;
        private System.Windows.Forms.OpenFileDialog OpenSklsDialog;
        private System.Windows.Forms.SaveFileDialog SaveSklsDialog;
        private System.Windows.Forms.MenuStrip MenuPanel;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem oGFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem oMFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveSklsToolStripMenuItem;
        private System.Windows.Forms.StatusStrip StatusPanel;
        private System.Windows.Forms.ToolStripStatusLabel FileLabel;
        private System.Windows.Forms.ToolStripStatusLabel StatusFile;
        private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sklSklsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem DeletesklsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem bonesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bonesToolStripMenuItem1;
        private System.Windows.Forms.OpenFileDialog OpenBonesDialog;
        private System.Windows.Forms.SaveFileDialog SaveBonesDialog;
        private System.Windows.Forms.SaveFileDialog SaveObjDialog;
        private System.Windows.Forms.SaveFileDialog SaveDmDialog;
        private System.Windows.Forms.SaveFileDialog SaveOgfDialog;
        private System.Windows.Forms.SaveFileDialog SaveOmfDialog;
        private System.Windows.Forms.OpenFileDialog OpenOgfDialog;
        private System.Windows.Forms.ToolStripMenuItem bonesPartsToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog OpenLtxDialog;
        private System.Windows.Forms.SaveFileDialog SaveLtxDialog;
        private System.Windows.Forms.ToolStripMenuItem bonesPartsToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem bonesPartsToDefaultToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showWindowToolStripMenuItem;
        private System.Windows.Forms.TabPage BonesPage;
        private System.Windows.Forms.TabPage FlagsPage;
        private System.Windows.Forms.GroupBox FlagsGroupBox;
        private System.Windows.Forms.GroupBox MotionFlagsGroupBox;
        private System.Windows.Forms.RadioButton AnimsNoCompress;
        private System.Windows.Forms.RadioButton Anims8Bit;
        private System.Windows.Forms.RadioButton Anims16Bit;
        private System.Windows.Forms.GroupBox ModelFlagsGroupBox;
        private System.Windows.Forms.RadioButton HQGeometry;
        private System.Windows.Forms.RadioButton HQGeometryPlus;
        private System.Windows.Forms.CheckBox ProgressiveMeshes;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.Label ObjectScaleLabel;
        private System.Windows.Forms.CheckBox ScaleCenterOfMassCheckBox;
        private System.Windows.Forms.TextBox ObjectScaleTextBox;
        private System.Windows.Forms.TabControl TabControl;
        private System.Windows.Forms.ToolStripMenuItem sklToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dMToolStripMenuItem;
        private System.Windows.Forms.TabPage SurfacesPage;
        private System.Windows.Forms.TabPage MotionPage;
        private System.Windows.Forms.RichTextBox MotionTextBox;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem shapeParamsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateShapesToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem typeHelperToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem allNoneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allBoxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allSphereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem allCylinderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem surfaceParamsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem enableAll2SidedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem disableAll2SidedToolStripMenuItem;
        private System.Windows.Forms.CheckBox StripifyMeshes;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog SaveObjectDialog;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateLodToolStripMenuItem;
        private System.Windows.Forms.TabPage LodPage;
        private System.Windows.Forms.SaveFileDialog SaveOgfLodDialog;
        private System.Windows.Forms.TextBox LodTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabPage UserDataPage;
        private System.Windows.Forms.RichTextBox UserDataTextBox;
        private System.Windows.Forms.TabPage MotionRefsPage;
        private System.Windows.Forms.RichTextBox MotionRefsBox;
        private System.Windows.Forms.ToolStripMenuItem cToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog SaveCppDialog;
        private System.Windows.Forms.ToolStripMenuItem batchConvertToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fromLtxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fromDialogToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog OpenBatchLtxDialog;
        private System.Windows.Forms.OpenFileDialog OpenBatchDialog;
        private System.Windows.Forms.ToolStripMenuItem BatchHelpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objectInfoToolStripMenuItem;
    }
}

