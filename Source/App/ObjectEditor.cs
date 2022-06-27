using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace Object_tool
{
	public struct ShapeEditType
	{
		public ushort bone_id;
		public ushort bone_type;
		public ushort bone_flags;
		public string bone_name;
	};

	public struct Surface
	{
		public uint flags;
		public string texture;
		public string shader;
	};

	public enum EditorMode
    {
		ExportOGF = 0,
		ExportOMF,
		GenerateShape,
		DeleteMotions,
		LoadMotions,
		SaveSklsMotions,
		LoadBones,
		SaveBones,
		ExportOBJ,
		ExportDM,
		SaveObject,
		LoadBoneParts,
		SaveBoneParts,
		ToDefaultBoneParts,
		SaveSklMotions,
    };

	public partial class Object_Editor : Form
	{
		// File sytem
		public string FILE_NAME = "";
		public string TEMP_FILE_NAME = "";
		public bool dbg_window = false;
		public List<ShapeEditType> shapes;
		public List<Surface> surfaces;
		public float model_scale = 1.0f;
		public bool DEVELOPER_MODE = false;
		public bool DEBUG_MODE = false;
		IniFile Settings = null;
		FolderSelectDialog SaveSklDialog = null;

		// Input
		public bool bKeyIsDown = false;

		public Object_Editor()
		{
			InitializeComponent();
			System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
			shapeParamsToolStripMenuItem.Enabled = false;
			surfaceParamsToolStripMenuItem.Enabled = false;
			saveToolStripMenuItem.Enabled = false;
			exportToolStripMenuItem.Enabled = false;
			deleteToolStripMenuItem.Enabled = false;
			sklSklsToolStripMenuItem.Enabled = false;
			bonesToolStripMenuItem.Enabled = false;

			SaveSklDialog = new FolderSelectDialog();

			string file_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\Settings.ini";
			Settings = new IniFile(file_path, "[settings]\ndeveloper=0\ndebug=0");

			DEVELOPER_MODE = Convert.ToBoolean(Convert.ToUInt16(Settings.ReadDef("developer", "settings", "0")));
			DEBUG_MODE = Convert.ToBoolean(Convert.ToUInt16(Settings.ReadDef("debug", "settings", "0")));

			debugToolStripMenuItem.Visible = DEBUG_MODE;
			radioButton3.Visible = DEVELOPER_MODE;
			radioButton3.Checked = DEVELOPER_MODE;

			if (Environment.GetCommandLineArgs().Length > 1)
			{
				bool skeleton = false;
				if (Environment.GetCommandLineArgs().Length > 2)
					skeleton = Environment.GetCommandLineArgs()[2] == "skeleton_only";

				OpenFile(Environment.GetCommandLineArgs()[1], skeleton);
			}
		}

		public void OpenFile(string filename, bool skeleton = false)
        {
			if (Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.Delete(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp", true);

			ClearUI();
			FILE_NAME = filename;

			StatusFile.Text = FILE_NAME.Substring(FILE_NAME.LastIndexOf('\\') + 1);

			if (skeleton)
				FILE_NAME = Environment.GetCommandLineArgs()[3];

			SaveOgfDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveOgfDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".ogf";

			SaveOmfDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveOmfDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".omf";

			SaveSklsDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveSklsDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".skls";

			SaveBonesDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveBonesDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".bones";

			SaveObjDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveObjDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".obj";

			SaveLtxDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveLtxDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".ltx";

			SaveSklDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));

			FILE_NAME = filename;

			if (!Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.CreateDirectory(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp");

			string TempFile = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + $"\\temp\\{StatusFile.Text}";

			File.Copy(FILE_NAME, TempFile, true);
			TEMP_FILE_NAME = TempFile;

			bool has_motions = MotionCount() > 0;
			DeletesklsToolStripMenuItem.Enabled = has_motions;
			SaveSklsToolStripMenuItem.Enabled = has_motions;
			sklToolStripMenuItem.Enabled = has_motions;
			oMFToolStripMenuItem.Enabled = has_motions;
			saveToolStripMenuItem.Enabled = !skeleton;
			exportToolStripMenuItem.Enabled = true;
			deleteToolStripMenuItem.Enabled = true;
			sklSklsToolStripMenuItem.Enabled = true;
			bonesToolStripMenuItem.Enabled = true;
			oGFToolStripMenuItem.Enabled = !skeleton;
			objToolStripMenuItem.Enabled = !skeleton;
			objectToolStripMenuItem.Enabled = !skeleton;
			groupBox2.Enabled = !skeleton;

			if (skeleton)
			{
				TabControl.Controls.Clear();
				TabControl.Controls.Add(FlagsPage);
			}

			LoadBoneData();
			LoadScale();
			LoadSurfaceData();
			ParseMotions();

			for (int i = 0; i < shapes.Count; i++)
			{
				CreateShapeGroupBox(i, shapes[i]);
			}
		}

		private void CreateShapeGroupBox(int idx, ShapeEditType shape)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 106 * idx);
			GroupBox.Size = new System.Drawing.Size(345, 100);
			GroupBox.Text = "Bone name: " + shape.bone_name;
			GroupBox.Name = "ShapeGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			CreateShapeFlags(idx, GroupBox, shape);
			ShapesPage.Controls.Add(GroupBox);
		}

		private void CreateShapeFlags(int idx, GroupBox box, ShapeEditType shape)
		{
			var NoPickableCheckBoxBox = new CheckBox();
			NoPickableCheckBoxBox.Name = "chbx_nopickable_" + idx;
			NoPickableCheckBoxBox.Text = "No Pickable";
			NoPickableCheckBoxBox.Size = new System.Drawing.Size(130, 23);
			NoPickableCheckBoxBox.Location = new System.Drawing.Point(6, 15);
			NoPickableCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoPickableCheckBoxBox.Checked = (shape.bone_flags & (1 << 0)) == (1 << 0);

			var NoPhysicsCheckBoxBox = new CheckBox();
			NoPhysicsCheckBoxBox.Name = "chbx_nophysics_" + idx;
			NoPhysicsCheckBoxBox.Text = "No Physics";
			NoPhysicsCheckBoxBox.Size = new System.Drawing.Size(100, 23);
			NoPhysicsCheckBoxBox.Location = new System.Drawing.Point(6, 35);
			NoPhysicsCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoPhysicsCheckBoxBox.Checked = (shape.bone_flags & (1 << 1)) == (1 << 1);

			var RemoveAfterBreakCheckBoxBox = new CheckBox();
			RemoveAfterBreakCheckBoxBox.Name = "chbx_RemoveAfterBreakCheckBoxBox_" + idx;
			RemoveAfterBreakCheckBoxBox.Text = "Remove After Break";
			RemoveAfterBreakCheckBoxBox.Size = new System.Drawing.Size(160, 23);
			RemoveAfterBreakCheckBoxBox.Location = new System.Drawing.Point(6, 55);
			RemoveAfterBreakCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			RemoveAfterBreakCheckBoxBox.Checked = (shape.bone_flags & (1 << 2)) == (1 << 2);

			var NoFogColliderCheckBoxBox = new CheckBox();
			NoFogColliderCheckBoxBox.Name = "chbx_NoFogColliderCheckBox_" + idx;
			NoFogColliderCheckBoxBox.Text = "No Fog Collider ";
			NoFogColliderCheckBoxBox.Size = new System.Drawing.Size(140, 23);
			NoFogColliderCheckBoxBox.Location = new System.Drawing.Point(6, 75);
			NoFogColliderCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoFogColliderCheckBoxBox.Checked = (shape.bone_flags & (1 << 3)) == (1 << 3);

			var TypeLabel = new Label();
			TypeLabel.Name = "TypeLbl_" + idx;
			TypeLabel.Text = "Shape type: ";
			TypeLabel.Location = new System.Drawing.Point(177, 18);

			var TypeComboBox = new ComboBox();
			TypeComboBox.Name = "cbx_type_" + idx;
			TypeComboBox.Size = new System.Drawing.Size(90, 23);
			TypeComboBox.Location = new System.Drawing.Point(245, 15);
			TypeComboBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			TypeComboBox.Items.Add("None");
			TypeComboBox.Items.Add("Box");
			TypeComboBox.Items.Add("Sphere");
			TypeComboBox.Items.Add("Cylinder");
			TypeComboBox.SelectedIndex = shape.bone_type;
			TypeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;

			box.Controls.Add(NoPickableCheckBoxBox);
			box.Controls.Add(NoPhysicsCheckBoxBox);
			box.Controls.Add(RemoveAfterBreakCheckBoxBox);
			box.Controls.Add(NoFogColliderCheckBoxBox);
			box.Controls.Add(TypeComboBox);
			box.Controls.Add(TypeLabel);
		}

		private void CreateMaterialGroupBox(int idx, string name)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 114 * idx);
			GroupBox.Size = new System.Drawing.Size(345, 108);
			GroupBox.Text = name;
			GroupBox.Name = "MaterialGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			CreateMaterialFlags(idx, GroupBox);
			MaterialsPage.Controls.Add(GroupBox);
		}

		private void CreateMaterialFlags(int idx, GroupBox box)
		{
			var TwoSidedCheckBoxBox = new CheckBox();
			TwoSidedCheckBoxBox.Name = "chbx2sided_" + idx;
			TwoSidedCheckBoxBox.Text = "2 Sided";
			TwoSidedCheckBoxBox.Size = new System.Drawing.Size(130, 23);
			TwoSidedCheckBoxBox.Location = new System.Drawing.Point(8, 82);
			TwoSidedCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			TwoSidedCheckBoxBox.Checked = (surfaces[idx].flags == 1);
			TwoSidedCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var TextureLabel = new Label();
			TextureLabel.Text = "Texture:";
			TextureLabel.Name = "TextureLabel_" + idx;
			TextureLabel.Size = new System.Drawing.Size(46, 23);
			TextureLabel.Location = new System.Drawing.Point(5, 32);
			TextureLabel.Anchor = AnchorStyles.Left | AnchorStyles.Top;

			var TextureTextBox = new TextBox();
			TextureTextBox.Name = "TextureTextBox_" + idx;
			TextureTextBox.Size = new System.Drawing.Size(282, 23);
			TextureTextBox.Location = new System.Drawing.Point(55, 30);
			TextureTextBox.Text = surfaces[idx].texture;
			TextureTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			TextureTextBox.TextChanged += new System.EventHandler(this.TextBoxFilter);

			var ShaderLabel = new Label();
			ShaderLabel.Text = "Shader:";
			ShaderLabel.Name = "ShaderLabel_" + idx;
			ShaderLabel.Size = new System.Drawing.Size(46, 23);
			ShaderLabel.Location = new System.Drawing.Point(5, 60);
			ShaderLabel.Anchor = AnchorStyles.Left | AnchorStyles.Top;

			var ShaderTextBox = new TextBox();
			ShaderTextBox.Name = "ShaderTextBox_" + idx;
			ShaderTextBox.Size = new System.Drawing.Size(282, 23);
			ShaderTextBox.Location = new System.Drawing.Point(55, 58);
			ShaderTextBox.Text = surfaces[idx].shader;
			ShaderTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			ShaderTextBox.TextChanged += new System.EventHandler(this.TextBoxFilter);

			box.Controls.Add(TwoSidedCheckBoxBox);
			box.Controls.Add(TextureLabel);
			box.Controls.Add(TextureTextBox);
			box.Controls.Add(ShaderLabel);
			box.Controls.Add(ShaderTextBox);
		}

		private int StartEditor(EditorMode mode, string object_path, string second_path = "null")
		{
			string args = $"{(int)mode} \"{object_path}\" \"{second_path}\" {GetFlags()} {model_scale} {shapes.Count} {surfaces.Count} {OpenSklsDialog.FileNames.Count()}";

			// Экспортируем шейпы
			CopyShapeParams();
			for (int i = 0; i < shapes.Count; i++)
			{
				args += $" \"{shapes[i].bone_id}-{shapes[i].bone_type}-{shapes[i].bone_flags}\"";
			}

            // Экспортируем текстуры
            for (int i = 0; i < surfaces.Count; i++)
            {
                args += $" {surfaces[i].flags}";
                args += $" \"{surfaces[i].texture}\"";
                args += $" \"{surfaces[i].shader}\"";
            }

            // Экспортируем лист анимаций на загрузку
            for (int i = 0; i < OpenSklsDialog.FileNames.Count(); i++)
            {
                args += $" \"{OpenSklsDialog.FileNames[i]}\"";
            }

            return RunCompiller(args);
		}

		private int RunCompiller(string args)
		{
			string exe_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\ActorEditor.exe";
			if (File.Exists(exe_path))
			{
				System.Diagnostics.Process proc = new System.Diagnostics.Process();
				proc.StartInfo.FileName = exe_path;
				proc.StartInfo.WorkingDirectory = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\'));
				proc.StartInfo.Arguments = args;
				proc.StartInfo.CreateNoWindow = !dbg_window;
				proc.StartInfo.UseShellExecute = dbg_window;
				proc.Start();
				proc.WaitForExit();
				return proc.ExitCode;
			}
			else
			{
				MessageBox.Show("Can't find ActorEditor.exe", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return -1;
			}
		}

		private int GetFlags()
        {
			int flags = 0;

			if (radioButton2.Checked)
				flags |= (1 << 0);

			if (radioButton3.Checked)
				flags |= (1 << 1);

			if (checkBox1.Checked)
				flags |= (1 << 2);

			if (checkBox2.Checked)
				flags |= (1 << 3);

			if (dbg_window)
				flags |= (1 << 4);

			if (ScaleCenterOfMassCheckBox.Checked)
				flags |= (1 << 5);

			if (HQGeometryPlus.Checked)
				flags |= (1 << 6);

			return flags;
        }

		private void ExportOGF_Click(object sender, EventArgs e)
		{
			if (SaveOgfDialog.ShowDialog() == DialogResult.OK)
			{
				SaveOgfDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.ExportOGF, TEMP_FILE_NAME, SaveOgfDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Model succesfully exported.", "", 1000, MessageBoxIcon.Information);
				else
                {
					if (code == 1)
						MessageBox.Show("Can't export model.\nPlease, disable HQ Geometry+ flag.", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
					else
						AutoClosingMessageBox.Show("Can't export model.", "", GetErrorTime(), MessageBoxIcon.Error);
				}
			}
		}

		private void ExportOMF_Click(object sender, EventArgs e)
		{
			if (SaveOmfDialog.ShowDialog() == DialogResult.OK)
			{
				SaveOmfDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.ExportOGF, TEMP_FILE_NAME, SaveOmfDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Motions succesfully exported.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Can't export motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void LoadSkls_Click(object sender, EventArgs e)
		{
			if (OpenSklsDialog.ShowDialog() == DialogResult.OK)
			{
				int code = StartEditor(EditorMode.LoadMotions, TEMP_FILE_NAME);
				if (code == 0)
				{
					AutoClosingMessageBox.Show("Motions succesfully loaded.", "", 1000, MessageBoxIcon.Information);
					DeletesklsToolStripMenuItem.Enabled = true;
					SaveSklsToolStripMenuItem.Enabled = true;
					sklToolStripMenuItem.Enabled = true;
					oMFToolStripMenuItem.Enabled = true;
				}
				else
					AutoClosingMessageBox.Show($"Can't load motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);

				ParseMotions();
			}
		}

		private void DeleteMotionsButton_Click(object sender, EventArgs e)
		{
			int code = StartEditor(EditorMode.DeleteMotions, TEMP_FILE_NAME);
			if (code == 0)
			{
				AutoClosingMessageBox.Show("Motions succesfully deleted.", "", 1000, MessageBoxIcon.Information);
				DeletesklsToolStripMenuItem.Enabled = false;
				SaveSklsToolStripMenuItem.Enabled = false;
				sklToolStripMenuItem.Enabled = false;
				oMFToolStripMenuItem.Enabled = false;

				MotionTextBox.Clear();
			}
			else
				AutoClosingMessageBox.Show($"Can't delete motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
		}

		private void SaveMotionsButton_Click(object sender, EventArgs e)
		{
			if (SaveSklsDialog.ShowDialog() == DialogResult.OK)
			{
				SaveSklsDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.SaveSklsMotions, TEMP_FILE_NAME, SaveSklsDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Motions succesfully saved.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void sklToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (SaveSklDialog.ShowDialog(this.Handle))
			{
				SaveSklDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.SaveSklMotions, TEMP_FILE_NAME, SaveSklDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Motions succesfully saved.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void bonesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (OpenBonesDialog.ShowDialog() == DialogResult.OK)
			{
				int code = StartEditor(EditorMode.LoadBones, TEMP_FILE_NAME, OpenBonesDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Bone data succesfully loaded.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Failed to load bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void bonesToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if (SaveBonesDialog.ShowDialog() == DialogResult.OK)
			{
				SaveBonesDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.SaveBones, TEMP_FILE_NAME, SaveBonesDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Bone data succesfully saved.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Failed to save bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void objToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (SaveObjDialog.ShowDialog() == DialogResult.OK)
			{
				SaveObjDialog.InitialDirectory = "";

				int code = StartEditor(EditorMode.ExportOBJ, TEMP_FILE_NAME, SaveObjDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Model succesfully saved.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Failed to save model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void generateShapesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			int code = StartEditor(EditorMode.GenerateShape, TEMP_FILE_NAME);
			if (code == 0)
				AutoClosingMessageBox.Show("Bone shapes succesfully generated.", "", 1000, MessageBoxIcon.Information);
			else
				AutoClosingMessageBox.Show($"Can't generate bone shapes.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
		}

		private void bonesPartsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (OpenLtxDialog.ShowDialog() == DialogResult.OK)
			{
				int code = StartEditor(EditorMode.LoadBoneParts, TEMP_FILE_NAME, OpenLtxDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Bone parts succesfully loaded.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Failed to load bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void bonesPartsToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if (SaveLtxDialog.ShowDialog() == DialogResult.OK)
			{
				int code = StartEditor(EditorMode.SaveBoneParts, TEMP_FILE_NAME, SaveLtxDialog.FileName);
				if (code == 0)
					AutoClosingMessageBox.Show("Bone parts succesfully saved.", "", 1000, MessageBoxIcon.Information);
				else
					AutoClosingMessageBox.Show($"Failed to saved bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
			}
		}

		private void bonesPartsToDefaultToolStripMenuItem_Click(object sender, EventArgs e)
		{
			int code = StartEditor(EditorMode.ToDefaultBoneParts, TEMP_FILE_NAME);
			if (code == 0)
				AutoClosingMessageBox.Show("Bone parts succesfully reseted to default.", "", 1000, MessageBoxIcon.Information);
			else
				AutoClosingMessageBox.Show($"Failed to reset bone parts to default.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
		}

		private void dMToolStripMenuItem_Click_1(object sender, EventArgs e)
		{
			if (SaveDmDialog.ShowDialog() == DialogResult.OK)
			{
				int code = StartEditor(EditorMode.ExportDM, TEMP_FILE_NAME, SaveDmDialog.FileName);
				if (code == 0)
                    AutoClosingMessageBox.Show("Model succesfully saved.", "", 1000, MessageBoxIcon.Information);
                else
                {
                    switch (code)
                    {
						case 1:
							AutoClosingMessageBox.Show($"Failed to save detail model. Object must contain 1 material.", "", GetErrorTime(), MessageBoxIcon.Error);
							break;
						case 2:
							AutoClosingMessageBox.Show($"Failed to save detail model. Object must contain 1 mesh.", "", GetErrorTime(), MessageBoxIcon.Error);
							break;
						default:
							AutoClosingMessageBox.Show($"Failed to save detail model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							break;
					}
                }
			}
		}

		private void LoadBoneData()
		{
			var xr_loader = new XRayLoader();
			shapes = new List<ShapeEditType>();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				xr_loader.ReadInt64();

				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2));
				if (B_CHUNK)
				{
					int chunk = 0;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						ShapeEditType shape = new ShapeEditType();
						shape.bone_id = (ushort)chunk;

                        if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_DEF, false, true))
                        {
                            shape.bone_name = xr_loader.read_stringZ();
                        }

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_SHAPE, false, true))
						{
							shape.bone_type = (ushort)xr_loader.ReadUInt16();
							shape.bone_flags = (ushort)xr_loader.ReadUInt16();
						}

						shapes.Add(shape);

						chunk++;
						xr_loader.SetStream(temp);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, false, true))
				{
					ShapeEditType shape = new ShapeEditType();
					shape.bone_flags = 0;
					shape.bone_type = 0;

					uint size = xr_loader.ReadUInt32();
					for (int i = 0; i < size; i++)
					{
						shape.bone_name = xr_loader.read_stringZ();
						shape.bone_id = (ushort)i;
						xr_loader.read_stringZ();
						xr_loader.read_stringZ();
						xr_loader.ReadBytes(12);
						xr_loader.ReadBytes(12);
						xr_loader.ReadFloat();

						shapes.Add(shape);
					}
				}
			}
		}

		private void LoadScale()
		{
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				xr_loader.ReadInt64();

				if (xr_loader.find_chunkSize((int)OBJECT.EOBJ_CHUNK_ACTORTRANSFORM) > 24)
				{
					xr_loader.ReadBytes(12);
					xr_loader.ReadBytes(12);
					model_scale = xr_loader.ReadFloat();
					ObjectScaleTextBox.Text = model_scale.ToString();
					uint chk = xr_loader.ReadUInt32();
					ScaleCenterOfMassCheckBox.Checked = Convert.ToBoolean(chk);
				}
			}
		}

		private uint MotionCount()
		{
			uint count = 0;
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				xr_loader.ReadInt64();

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS))
					count = xr_loader.ReadUInt32();
			}
			return count;
		}

		private void LoadSurfaceData()
		{
			var xr_loader = new XRayLoader();
			surfaces = new List<Surface>();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				xr_loader.ReadInt64();

				Surface surface = new Surface();

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3))
				{
					uint cnt = xr_loader.ReadUInt32();
					for (int i = 0; i < cnt; i++)
					{
						string name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						xr_loader.read_stringZ(); // GameMtl
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
						CreateMaterialGroupBox(i, name);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2))
				{
					uint cnt = xr_loader.ReadUInt32();
					for (int i = 0; i < cnt; i++)
					{
						string name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
						CreateMaterialGroupBox(i, name);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES))
				{
					uint cnt = xr_loader.ReadUInt32();
					for (int i = 0; i < cnt; i++)
					{
						string name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						surface.flags = xr_loader.ReadByte();     // Flags
						xr_loader.ReadUInt32();	  // FVF
						xr_loader.ReadUInt32();   // TC count
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap

						surfaces.Add(surface);
						CreateMaterialGroupBox(i, name);
					}
				}
			}
		}

		private void ParseMotions()
		{
			var xr_loader = new XRayLoader();
			MotionTextBox.Clear();
			MotionTextBox.Text = $"Motions count: 0";

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				xr_loader.ReadInt64();

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS))
				{
					uint count = xr_loader.ReadUInt32();
					MotionTextBox.Clear();
					MotionTextBox.Text = $"Motions count: {count}\n";

					for (int i = 0; i < count; i++)
					{
						string name = xr_loader.read_stringZ();
						xr_loader.ReadBytes(12);
						uint vers = xr_loader.ReadUInt16();

						MotionTextBox.Text += $"\n{i + 1}. {name}";

						switch (vers)
						{
							case 4:
								{
									xr_loader.ReadBytes(22);
									uint sz = xr_loader.ReadUInt32();
									for (int j = 0; j < sz; j++)
									{
										xr_loader.ReadBytes(4);
										for (int m = 0; m < 6; m++)
										{
											xr_loader.ReadBytes(8);
											uint sz1 = xr_loader.ReadUInt32();
											for (int p = 0; p < sz1; p++)
											{
												xr_loader.ReadBytes(40);
											}
										}
									}
								}
								break;
							case 5:
								{
									xr_loader.ReadBytes(24);
									uint sz = xr_loader.ReadUInt32();
									for (int j = 0; j < sz; j++)
									{
										xr_loader.read_stringZ();
										xr_loader.ReadBytes(4);
										for (int m = 0; m < 6; m++)
										{
											xr_loader.ReadBytes(8);
											uint sz1 = xr_loader.ReadUInt32();
											for (int p = 0; p < sz1; p++)
											{
												xr_loader.ReadBytes(40);
											}
										}
									}
								}
								break;
							default:
								{
									if (vers >= 6)
									{
										xr_loader.ReadBytes(19);
										uint sz = xr_loader.ReadUInt16();
										for (int j = 0; j < sz; j++)
										{
											xr_loader.read_stringZ();
											xr_loader.ReadBytes(1);
											for (int m = 0; m < 6; m++)
											{
												xr_loader.ReadBytes(2);
												uint sz1 = xr_loader.ReadUInt16();
												for (int p = 0; p < sz1; p++)
												{
													xr_loader.ReadBytes(8);
													byte shape = xr_loader.ReadByte();
													if (shape != 4)
													{
														xr_loader.ReadBytes(14);
													}
												}
											}
										}
									}
									else
										break;
								}
								break;
						}

						if (vers >= 7)
						{
							uint sz = xr_loader.ReadUInt32();
							if (sz > 0)
							{
								for (int j = 0; j < sz; ++j)
								{
									xr_loader.read_motion_mark_string();
									uint sz1 = xr_loader.ReadUInt32();

									for (int p = 0; p < sz1; ++p)
									{
										xr_loader.ReadBytes(8);
									}
								}
							}
						}
					}
				}
			}
		}

		private void FlagsHelpButton_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Motion export:\nДанные флаги влияют на компресиию анимаций при экспортировании в OMF.\n1. 8 bit - ТЧ Формат\n2. 16 bit - ЗП Формат\n" + (DEVELOPER_MODE ? "3. No compress - экспортирует анимации без сжатия\n\n" : "\n") +
				"Model export:\n" +
				"1. Make progressive bones - создает прогрессивные меши при экспорте OGF. Это динамическая детализация модели (lod'ы), чаще используется для мировых объектов.\n" +
				"2. Optimize surfaces - при включении объединяет меши с одинаковыми названиями текстур и шейдеров как и любой SDK. В данном эдиторе появилась возможность отключить это для последующих изменений через OGF Editor.\n" +
                "3. HQ Geometry+ - при активации компилятор постарается экспортировать модель без оптимизаций вертексов, эксперементальная функция которая иногда не работает с тяжёлыми моделями. \n\n" +
                "Object scale - изменяет размер объекта при экспорте, влияет на размер модели и размер анимаций.\nScale center of mass - при активации во время экспорта с измененным размером объекта будут пересчитаны центры массы коллизии под новый размер.", "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void ClearUI()
        {
			ShapesPage.Controls.Clear();
		}

		private void objectToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (OpenObjectDialog.ShowDialog() == DialogResult.OK)
            {
				OpenFile(OpenObjectDialog.FileName);
			}
		}

		private void IndexChanged(object sender, EventArgs e)
		{
			if (TabControl.SelectedIndex < 0) return;

			shapeParamsToolStripMenuItem.Enabled = false;
			surfaceParamsToolStripMenuItem.Enabled = false;

			switch (TabControl.Controls[TabControl.SelectedIndex].Name)
			{
				case "ShapesPage":
					shapeParamsToolStripMenuItem.Enabled = shapes.Count > 0;
					break;
				case "MaterialPage":
					surfaceParamsToolStripMenuItem.Enabled = true;
					break;
			}
		}

		private void allNoneToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SwitchShapeType(0);
		}

		private void allBoxToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SwitchShapeType(1);
		}

		private void allSphereToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SwitchShapeType(2);
		}

		private void allCylinderToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SwitchShapeType(3);
		}

		private void SwitchShapeType(ushort type)
		{
			for (int i = 0; i < shapes.Count; i++)
			{
				ShapeEditType shape = new ShapeEditType();
				shape.bone_id = shapes[i].bone_id;
				shape.bone_flags = shapes[i].bone_flags;
				shape.bone_name = shapes[i].bone_name;
				shape.bone_type = type;
				shapes[i] = shape;

				ComboBox ShapeType = ShapesPage.Controls[i].Controls[4] as ComboBox;
				ShapeType.SelectedIndex = type;
			}
		}

		private void CopyShapeParams()
        {
			for (int i = 0; i < shapes.Count; i++)
			{
				ShapeEditType shape = new ShapeEditType();

				ushort flags = 0;
				CheckBox NoPickable = ShapesPage.Controls[i].Controls[0] as CheckBox;
				CheckBox NoPhysics = ShapesPage.Controls[i].Controls[1] as CheckBox;
				CheckBox RemoveAfterBreak = ShapesPage.Controls[i].Controls[2] as CheckBox;
				CheckBox NoFogCollider = ShapesPage.Controls[i].Controls[3] as CheckBox;
				ComboBox ShapeType = ShapesPage.Controls[i].Controls[4] as ComboBox;

				if (NoPickable.Checked)
					flags |= (1 << 0);

				if (NoPhysics.Checked)
					flags |= (1 << 1);

				if (RemoveAfterBreak.Checked)
					flags |= (1 << 2);

				if (NoFogCollider.Checked)
					flags |= (1 << 3);

				shape.bone_id = shapes[i].bone_id;
				shape.bone_flags = flags;
				shape.bone_name = shapes[i].bone_name;
				shape.bone_type = (ushort)ShapeType.SelectedIndex;
				shapes[i] = shape;
			}
		}

		private void objectToolStripMenuItem1_Click(object sender, EventArgs e)
        {
			StartEditor(EditorMode.SaveObject, TEMP_FILE_NAME);
			File.Copy(TEMP_FILE_NAME, FILE_NAME, true);
			AutoClosingMessageBox.Show("Object succesfully saved.", "", 1000, MessageBoxIcon.Information);
		}

        private void ClosingForm(object sender, FormClosingEventArgs e)
        {
			if (Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.Delete(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp", true);
		}

        private void ScaleKeyPress(object sender, KeyPressEventArgs e)
        {
			if (!(Char.IsDigit(e.KeyChar)) && !((e.KeyChar == '.') && (ObjectScaleTextBox.Text.IndexOf(".") == -1) &&(ObjectScaleTextBox.Text.Length != 0)))
			{
				if (e.KeyChar != (char)Keys.Back) e.Handled = true;
			}
		}

		private void ScaleTextChanged(object sender, EventArgs e)
		{
			if (bKeyIsDown)
			{
				TextBox curBox = sender as TextBox;

				if (curBox.Text.Length == 0)
					return;

				string number_mask = @"^-[0-9.]*$";
				int temp = curBox.SelectionStart;
				string mask = number_mask;
				Regex.Match(curBox.Text, mask);

				try
				{
					Convert.ToSingle(curBox.Text);
					model_scale = Convert.ToSingle(curBox.Text);
				}
				catch (Exception)
				{
					curBox.Text = model_scale.ToString();

					if (curBox.SelectionStart < 1)
						curBox.SelectionStart = curBox.Text.Length;

					curBox.SelectionStart = temp - 1;
				}
				bKeyIsDown = false;
			}
		}

        private void ScaleKeyDown(object sender, KeyEventArgs e)
        {
			bKeyIsDown = true;
		}

        private void openLogToolStripMenuItem_Click(object sender, EventArgs e)
        {
			string log = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\engine.log";
			if (File.Exists(log))
			{
				System.Diagnostics.Process proc = System.Diagnostics.Process.Start("notepad.exe", log);
				proc.WaitForExit();
				proc.Close();
			}
			else
				AutoClosingMessageBox.Show("Can't find log.", "", 1000, MessageBoxIcon.Error);
		}

        private void showWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
			dbg_window = !dbg_window;
			if (dbg_window)
				showWindowToolStripMenuItem.Text = "Hide debug window";
			else
				showWindowToolStripMenuItem.Text = "Show debug window";
		}

		private string GetRetCode(int code)
		{
			string ret = "";
			if (DEBUG_MODE)
				ret += "\nExit code: " + code.ToString();
			return ret;
		}

		private int GetErrorTime()
		{
			if (DEBUG_MODE)
				return 15000;
			return 1000;
		}

		private void TextBoxFilter(object sender, EventArgs e)
		{
			TextBox curBox = sender as TextBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			Surface surface = new Surface();
			switch (currentField)
			{
				case "TextureTextBox":
					surface.flags = surfaces[idx].flags;
					surface.texture = curBox.Text;
					surface.shader = surfaces[idx].shader;
					surfaces[idx] = surface;
					break;
				case "ShaderTextBox":
					surface.flags = surfaces[idx].flags;
					surface.texture = surfaces[idx].texture;
					surface.shader = curBox.Text;
					surfaces[idx] = surface;
					break;
			}
		}

		private void CheckBoxCheckedChanged(object sender, EventArgs e)
		{
			CheckBox curBox = sender as CheckBox;
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			Surface surface = new Surface();
			surface.flags = (uint)(curBox.Checked ? 1 : 0);
			surface.texture = surfaces[idx].texture;
			surface.shader = surfaces[idx].shader;
			surfaces[idx] = surface;
		}

        private void enableAll2SidedToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Button CurButton = sender as Button;
			for (int i = 0; i < surfaces.Count; i++)
			{
				Surface surface = new Surface();
				surface.flags = 1;
				surface.texture = surfaces[i].texture;
				surface.shader = surfaces[i].shader;
				surfaces[i] = surface;

				CheckBox TwoSided = MaterialsPage.Controls[i].Controls[0] as CheckBox;
				TwoSided.Checked = (surfaces[i].flags == 1);
			}
		}

        private void disableAll2SidedToolStripMenuItem_Click(object sender, EventArgs e)
        {
			Button CurButton = sender as Button;
			for (int i = 0; i < surfaces.Count; i++)
			{
				Surface surface = new Surface();
				surface.flags = 0;
				surface.texture = surfaces[i].texture;
				surface.shader = surfaces[i].shader;
				surfaces[i] = surface;

				CheckBox TwoSided = MaterialsPage.Controls[i].Controls[0] as CheckBox;
				TwoSided.Checked = (surfaces[i].flags == 1);
			}
		}
    }
}
