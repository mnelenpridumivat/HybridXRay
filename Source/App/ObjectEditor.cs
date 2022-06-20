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
	}

	public partial class Object_Editor : Form
	{
		// File sytem
		public string FILE_NAME = "";
		public bool dbg_window = true;
		public List<ShapeEditType> model_shapes;

		// Input
		public bool bKeyIsDown = false;

		public Object_Editor()
		{
			InitializeComponent();
			System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
			openFileDialog1.Filter = "Object file|*.object";
			openFileDialog2.Filter = "Skls file|*.skls";
			saveFileDialog1.Filter = "Skls file|*.skls";
			shapeParamsToolStripMenuItem.Enabled = false;

			if (Environment.GetCommandLineArgs().Length > 1)
			{
				FILE_NAME = Environment.GetCommandLineArgs()[1];
				StatusFile.Text = FILE_NAME.Substring(FILE_NAME.LastIndexOf('\\') + 1);
				bool has_motions = MotionCount() > 0;
				DeletesklsToolStripMenuItem.Enabled = has_motions;
				SaveSklsToolStripMenuItem.Enabled = has_motions;
				LoadBoneData();

				for (int i = 0; i < model_shapes.Count; i++)
				{
					CreateShapeGroupBox(i, model_shapes[i]);
				}
			}
		}

		private void CreateShapeGroupBox(int idx, ShapeEditType shape)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 106 * idx);
			GroupBox.Size = new System.Drawing.Size(333, 100);
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
			TypeLabel.Location = new System.Drawing.Point(163, 18);

			var TypeComboBox = new ComboBox();
			TypeComboBox.Name = "cbx_type_" + idx;
			TypeComboBox.Size = new System.Drawing.Size(90, 23);
			TypeComboBox.Location = new System.Drawing.Point(230, 15);
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

		private int ExportOGF(string object_path, string ogf_path)
		{
			return RunCompiller($"0 \"{object_path}\" \"{ogf_path}\" {GetFlags()}");
		}

		private int ExportOMF(string object_path, string omf_path)
		{
			return RunCompiller($"1 \"{object_path}\" \"{omf_path}\" {GetFlags()}");
		}
		private int GenerateShapes(string object_path, List<ShapeEditType> shapes)
		{
			CopyShapeParams();
			string args = $"2 \"{object_path}\" temp {GetFlags()}";
			for (int i = 0; i < shapes.Count; i++)
			{
				args += $" \"{shapes[i].bone_id}-{shapes[i].bone_type}-{shapes[i].bone_flags}\"";
			}
			return RunCompiller(args, false);
		}

		private int DeleteMotions(string object_path)
		{
			return RunCompiller($"3 \"{object_path}\" temp {GetFlags()}", true);
		}

		private int LoadMotions(string object_path, string skls_path)
		{
			return RunCompiller($"4 \"{object_path}\" \"{skls_path}\" {GetFlags()}", true);
		}

		private int SaveMotions(string object_path, string skls_path)
		{
			return RunCompiller($"5 \"{object_path}\" \"{skls_path}\" {GetFlags()}", false);
		}

		private int RunCompiller(string args, bool force_disable_window = false)
		{
			string exe_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\ActorEditor.exe";
			if (File.Exists(exe_path))
			{
				System.Diagnostics.Process proc = new System.Diagnostics.Process();
				proc.StartInfo.FileName = exe_path;
				proc.StartInfo.WorkingDirectory = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\'));
				proc.StartInfo.Arguments = args;
				if (!force_disable_window)
				{
					proc.StartInfo.CreateNoWindow = !dbg_window;
					proc.StartInfo.UseShellExecute = dbg_window;
				}
				else
				{
					proc.StartInfo.CreateNoWindow = true;
					proc.StartInfo.UseShellExecute = false;
				}
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
			else if (radioButton3.Checked)
				flags |= (1 << 1);

			if (checkBox1.Checked)
				flags |= (1 << 2);

			if (checkBox2.Checked)
				flags |= (1 << 3);

			if (dbg_window)
				flags |= (1 << 4);

			return flags;
        }

		private void ExportOGF_Click(object sender, EventArgs e)
		{
			string ogf = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('.')) + ".ogf";

			if (ExportOGF(FILE_NAME, ogf) == 0)
				MessageBox.Show("Done!");
			else
				MessageBox.Show("Error!");
		}

		private void ExportOMF_Click(object sender, EventArgs e)
		{
			string omf = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('.')) + ".omf";

			if (ExportOMF(FILE_NAME, omf) == 0)
				MessageBox.Show("Done!");
			else
				MessageBox.Show("Error!");
		}

		private void LoadSkls_Click(object sender, EventArgs e)
		{
			if (openFileDialog2.ShowDialog() == DialogResult.OK)
			{
				if (LoadMotions(FILE_NAME, openFileDialog2.FileName) == 0)
				{
					MessageBox.Show("Done!");
					DeletesklsToolStripMenuItem.Enabled = true;
					SaveSklsToolStripMenuItem.Enabled = true;
				}
				else
					MessageBox.Show("Error!");
			}
		}

		private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
		{
			FILE_NAME = openFileDialog2.FileName;
		}

		private void DeleteMotionsButton_Click(object sender, EventArgs e)
		{
			if (DeleteMotions(FILE_NAME) == 0)
			{
				MessageBox.Show("Done!");
				DeletesklsToolStripMenuItem.Enabled = false;
				SaveSklsToolStripMenuItem.Enabled = false;
			}
			else
				MessageBox.Show("Error!");
		}

		private void SaveMotionsButton_Click(object sender, EventArgs e)
		{
			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				if (SaveMotions(FILE_NAME, saveFileDialog1.FileName) == 0)
					MessageBox.Show("Done!");
				else
					MessageBox.Show("Error!");
			}
		}

		private void LoadBoneData()
		{
			var xr_loader = new XRayLoader();
			model_shapes = new List<ShapeEditType>();

			using (var r = new BinaryReader(new FileStream(FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, false, true));
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

						model_shapes.Add(shape);

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

						model_shapes.Add(shape);
					}
				}
			}
		}

		private uint MotionCount()
		{
			uint count = 0;
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS, false, true))
				{
					count = xr_loader.ReadUInt32();
				}
			}
			return count;
		}

		private void FlagsHelpButton_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Motion export:\nДанные флаги влияют на компресиию анимаций при экспортировании в OMF.\n1. 8 bit - ТЧ Формат\n2. 16 bit - ЗП Формат\n3. Without compress - сохраняет без сжатия, кастомный формат из STCoP\n\n" +
				"Model export:\n" +
				"1. Make progressive bones - Создает прогрессивные меши при экспорте OGF. Это динамическая детализация модели (lod'ы), чаще используется для мировых объектов.\n" +
				"2. Optimize surfaces - при включении объединяет меши с одинаковыми названиями текстур и шейдеров как и любой SDK. В данном эдиторе появилась возможность отключить это для последующих изменений через OGF Editor", "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void objectToolStripMenuItem_Click(object sender, EventArgs e)
		{

		}

		private void IndexChanged(object sender, EventArgs e)
		{
			if (TabControl.SelectedIndex < 0) return;

			shapeParamsToolStripMenuItem.Enabled = false;

			switch (TabControl.Controls[TabControl.SelectedIndex].Name)
			{
				case "ShapesPage":
					{
						shapeParamsToolStripMenuItem.Enabled = true;
						break;
					}
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
			for (int i = 0; i < model_shapes.Count; i++)
			{
				ShapeEditType shape = new ShapeEditType();
				shape.bone_id = model_shapes[i].bone_id;
				shape.bone_flags = model_shapes[i].bone_flags;
				shape.bone_name = model_shapes[i].bone_name;
				shape.bone_type = type;
				model_shapes[i] = shape;

				ComboBox ShapeType = ShapesPage.Controls[i].Controls[4] as ComboBox;
				ShapeType.SelectedIndex = type;
			}
		}

		private void generateShapesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (GenerateShapes(FILE_NAME, model_shapes) == 0)
				MessageBox.Show("Done!");
			else
				MessageBox.Show("Error!");
		}

		private void CopyShapeParams()
        {
			for (int i = 0; i < model_shapes.Count; i++)
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

				shape.bone_id = model_shapes[i].bone_id;
				shape.bone_flags = flags;
				shape.bone_name = model_shapes[i].bone_name;
				shape.bone_type = (ushort)ShapeType.SelectedIndex;
				model_shapes[i] = shape;
			}
		}
	}
}
