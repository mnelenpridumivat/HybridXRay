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
		GenerateLod,
		SaveCpp,
		BatchLtx,
		BatchDialogOGF,
		BatchDialogOMF,
	};

	public partial class Object_Editor : Form
	{
		public class ShapeEditType
		{
			public ushort bone_id;
			public ushort bone_type;
			public ushort bone_flags;
			public string bone_name;
		};

		public class Surface
		{
			public uint flags;
			public string texture;
			public string shader;
		};

		// File sytem
		public string FILE_NAME = "";
		public string TEMP_FILE_NAME = "";
		public bool dbg_window = false;
		public List<ShapeEditType> shapes;
		public List<Surface> surfaces;
		public float model_scale = 1.0f;
		public float lod_quality = 0.5f;
		public int lod_flags = 0;
		public bool DEVELOPER_MODE = false;
		public bool DEBUG_MODE = false;
		IniFile Settings = null;
		FolderSelectDialog SaveSklDialog = null;
		FolderSelectDialog OpenBatchOutDialog = null;
        List<string[]> batch_files = null;
		List<string> batch_source = null;
		public bool IsOgfMode = false;
		public string script = "null";
		public string SCRIPT_FOLDER = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\scripts\\";

		public int cpp_mode = 0;

		// Info
		public uint vertex_count = 0;
		public uint face_count = 0;
		public uint surface_count = 0;
		public uint joints_count = 0;

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
			dMToolStripMenuItem.Enabled = false;
			bonesPartsToolStripMenuItem.Enabled = false;
			FlagsGroupBox.Enabled = false;
			MotionRefsBox.Enabled = false;
			UserDataTextBox.Enabled = false;
			LodTextBox.Enabled = false;
			ObjectScaleTextBox.Enabled = false;
			ScaleCenterOfMassCheckBox.Enabled = false;
			ObjectScaleLabel.Enabled = false;
			motionRefsToolStripMenuItem.Enabled = false;
			userDataToolStripMenuItem.Enabled = false;
			motionRefsToolStripMenuItem1.Enabled = false;
			userDataToolStripMenuItem1.Enabled = false;
			generateLodToolStripMenuItem.Enabled = false;
			objectInfoToolStripMenuItem.Enabled = false;
			SplitNormalsChbx.Enabled = false;
			normalsToolStripMenuItem.Enabled = false;
			importObjectParamsToolStripMenuItem.Enabled = false;

			SaveSklDialog = new FolderSelectDialog();
			OpenBatchOutDialog = new FolderSelectDialog();
			batch_files = new List<string[]>();
			batch_source = new List<string>();

			string file_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\Settings.ini";
			Settings = new IniFile(file_path, "[settings]\ndeveloper=0\ndebug=0");

			DEVELOPER_MODE = Convert.ToBoolean(Convert.ToUInt16(Settings.ReadDef("developer", "settings", "0")));
			DEBUG_MODE = Convert.ToBoolean(Convert.ToUInt16(Settings.ReadDef("debug", "settings", "0")));

#if DEBUG
			DEVELOPER_MODE = true;
			DEBUG_MODE = true;
			dbg_window = true;
			showWindowToolStripMenuItem.Enabled = false;
#endif

			if (System.Diagnostics.Debugger.IsAttached)
            {
				DEVELOPER_MODE = true;
				DEBUG_MODE = true;
				dbg_window = true;
				showWindowToolStripMenuItem.Enabled = false;
			}

			debugToolStripMenuItem.Visible = DEBUG_MODE;
			AnimsNoCompress.Visible = DEVELOPER_MODE;
			AnimsNoCompress.Checked = DEVELOPER_MODE;

			// Init scripts
			if (File.Exists(SCRIPT_FOLDER + "main.script"))
            {
				IniFile Script = new IniFile(SCRIPT_FOLDER + "main.script");

				for (int i = 0; i < 50; i++)
                {
					string script_param = "script_" + i;
					string script_file = Script.Read(script_param, "main");
					if (script_file == "") break;

					IniFile NewScript = new IniFile(SCRIPT_FOLDER + script_file + ".script");
					string script_name = NewScript.Read("name", "options");

					ToolStripMenuItem item = new ToolStripMenuItem();
					item.Text = script_name;
					item.Tag = SCRIPT_FOLDER + script_file + ".script";
					item.Click += new System.EventHandler(this.ScriptClicked);

					loadScriptToolStripMenuItem.DropDownItems.Add(item);
				}
			}

			loadScriptToolStripMenuItem.Visible = loadScriptToolStripMenuItem.DropDownItems.Count > 0;

			if (Environment.GetCommandLineArgs().Length > 1)
			{
				IsOgfMode = false;
				if (Environment.GetCommandLineArgs().Length > 2)
					IsOgfMode = Environment.GetCommandLineArgs()[2] == "skeleton_only";

				OpenFile(Environment.GetCommandLineArgs()[1]);
			}
		}

		public void OpenFile(string filename)
        {
			if (Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.Delete(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp", true);

			if (!Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.CreateDirectory(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp");

			BonesPage.Controls.Clear();
			FILE_NAME = filename;
			StatusFile.Text = FILE_NAME.Substring(FILE_NAME.LastIndexOf('\\') + 1);

			string TempFile = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + $"\\temp\\{StatusFile.Text}";
			File.Copy(FILE_NAME, TempFile, true);
			TEMP_FILE_NAME = TempFile;

			InitDialogs();
			InitUI();

			ParseMotions();
			LoadData();

			if (shapes != null)
			{
				for (int i = 0; i < shapes.Count; i++)
				{
					CreateShapeGroupBox(i, shapes[i]);
				}
			}

			IndexChanged(null, null);

			if (!IsOgfMode)
			{
				MotionRefsTextChanged(MotionRefsBox, null);
				UserDataTextChanged(UserDataTextBox, null);
			}
		}

		private int StartEditor(EditorMode mode, string object_path, string second_path = "null", int flags = -1, float scale = 1.0f)
		{
			if (flags == -1)
            {
				flags = GetFlags();
				scale = model_scale;
			}

			int shapes_count = (shapes != null ? shapes.Count : 0);
			int surfaces_count = (surfaces != null ? surfaces.Count : 0);
			string args = $"{(int)mode} \"{object_path}\" \"{second_path}\" {flags} {scale} {shapes_count} {surfaces_count} {OpenSklsDialog.FileNames.Count()}";

			// Экспортируем шейпы
			if (shapes_count > 0)
			{
				for (int i = 0; i < shapes.Count; i++)
				{
					args += $" \"{shapes[i].bone_id}-{shapes[i].bone_type}-{shapes[i].bone_flags}\"";
				}
			}

			// Экспортируем текстуры
			if (surfaces_count > 0)
			{
				for (int i = 0; i < surfaces.Count; i++)
				{
					args += $" {surfaces[i].flags}";
					args += $" \"{surfaces[i].texture}\"";
					args += $" \"{surfaces[i].shader}\"";
				}
			}

            // Экспортируем лист анимаций на загрузку
            for (int i = 0; i < OpenSklsDialog.FileNames.Count(); i++)
            {
                args += $" \"{OpenSklsDialog.FileNames[i]}\"";
            }

			// Экспортируем качество и флаги лода
			args += $" {lod_quality} {lod_flags}";

			// Экспортируем путь к огф лоду
			args += $" \"{LodTextBox.Text}\"";

			// Экспортируем юзердату
			if (IsTextCorrect(UserDataTextBox.Text))
			{
				string userdata = "";
				for (int i = 0; i < UserDataTextBox.Lines.Count(); i++)
				{
					string ext = i == UserDataTextBox.Lines.Count() - 1 ? "" : "\r\n";
					userdata += UserDataTextBox.Lines[i] + ext;
				}
				File.WriteAllText(object_path + "_temp.userdata", userdata);
			}

			// Экспортируем моушн рефы
			List<string> motion_refs = new List<string>();
			if (IsTextCorrect(MotionRefsBox.Text))
			{
				for (int i = 0; i < MotionRefsBox.Lines.Count(); i++)
				{
					if (IsTextCorrect(MotionRefsBox.Lines[i]))
						motion_refs.Add(GetCorrectString(MotionRefsBox.Lines[i]));
				}
			}

			args += $" {motion_refs.Count}";
			for (int i = 0; i < motion_refs.Count; i++)
			{
				args += $" \"{motion_refs[i]}\"";
			}

            // Экспортируем файлы батч конвертера
            args += $" {(batch_source.Count == 0 ? -1 : batch_source.Count)}";

			if (batch_source.Count > 0)
			{
				for (int i = 0; i < batch_source.Count; i++)
				{
					args += $" \"{batch_source[i]}\"";
					args += $" {batch_files[i].Count()}";
					for (int cnt = 0; cnt < batch_files[i].Count(); cnt++)
					{
						args += $" \"{batch_files[i][cnt]}\"";
					}
				}
			}
			else
            {
				args += $" {batch_files.Count}";
				for (int i = 0; i < batch_files.Count; i++)
				{
					args += $" \"{batch_files[i]}\"";
				}
			}
            args += $" \"{OpenBatchOutDialog.FileName}\"";

            batch_files.Clear();
			batch_source.Clear();

			// Экспорт режима экспорта c++
			args += $" {cpp_mode}";

			// Экспорт скрипта
			args += $" \"{script}\"";

			int exit_code = RunCompiller(args);
			if (File.Exists(object_path + "_temp.userdata"))
				File.Delete(object_path + "_temp.userdata");
			return exit_code;
		}

		private int RunCompiller(string args)
		{
			string exe_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\ActorEditor.exe";
			if (File.Exists(exe_path))
			{
				CheckTempFileExist();
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

			if (Anims16Bit.Checked)
				flags |= (1 << 0);

			if (AnimsNoCompress.Checked)
				flags |= (1 << 1);

			if (ProgressiveMeshes.Checked)
				flags |= (1 << 2);

			if (checkBox2.Checked)
				flags |= (1 << 3);

			if (dbg_window)
				flags |= (1 << 4);

			if (ScaleCenterOfMassCheckBox.Checked)
				flags |= (1 << 5);

			if (HQGeometryPlus.Checked)
				flags |= (1 << 6);

			if (StripifyMeshes.Checked)
				flags |= (1 << 7);

			if (SplitNormalsChbx.Checked)
				flags |= (1 << 8);

			if (BuildInMotionsExport.Checked)
				flags |= (1 << 9);

			return flags;
        }

		private void RunSDK_Click(object sender, EventArgs e)
		{
			ToolStripItem Item = sender as ToolStripItem;
			string currentField = Item.Tag.ToString();
			int idx = 0;
			if (currentField.Contains("_"))
			{
				currentField = currentField.Split('_')[0];
				idx = Convert.ToInt32(Item.Tag.ToString().Split('_')[1]);
			}

			switch (currentField)
            {
				case "LoadObject":
                    {
						if (OpenObjectDialog.ShowDialog() == DialogResult.OK)
							OpenFile(OpenObjectDialog.FileName);
					}
					break;
				case "SaveObject":
					{
						if (SaveObjectDialog.ShowDialog() == DialogResult.OK)
						{
							SaveObjectDialog.InitialDirectory = "";
							FastSaveObject(SaveObjectDialog.FileName);
						}
					}
					break;
				case "ExportOGF":
                    {
						if (SaveOgfDialog.ShowDialog() == DialogResult.OK)
						{
							SaveOgfDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.ExportOGF, TEMP_FILE_NAME, SaveOgfDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Model successfully exported.", "", 1000, MessageBoxIcon.Information);
							else
							{
								if (code == 1)
									MessageBox.Show("Can't export model.\nPlease, disable HQ Geometry+ flag.", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
								else
									AutoClosingMessageBox.Show($"Can't export model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "ExportOMF":
                    {
						if (SaveOmfDialog.ShowDialog() == DialogResult.OK)
						{
							SaveOmfDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.ExportOMF, TEMP_FILE_NAME, SaveOmfDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Motions successfully exported.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Can't export motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "LoadSkls":
                    {
						if (OpenSklsDialog.ShowDialog() == DialogResult.OK)
						{
							int code = StartEditor(EditorMode.LoadMotions, TEMP_FILE_NAME);
							if (code == 0)
							{
								AutoClosingMessageBox.Show("Motions successfully loaded.", "", 1000, MessageBoxIcon.Information);
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
					break;
				case "DeleteSkls":
                    {
						int code = StartEditor(EditorMode.DeleteMotions, TEMP_FILE_NAME);
						if (code == 0)
						{
							AutoClosingMessageBox.Show("Motions successfully deleted.", "", 1000, MessageBoxIcon.Information);
							DeletesklsToolStripMenuItem.Enabled = false;
							SaveSklsToolStripMenuItem.Enabled = false;
							sklToolStripMenuItem.Enabled = false;
							oMFToolStripMenuItem.Enabled = false;

							ParseMotions();
						}
						else
							AutoClosingMessageBox.Show($"Can't delete motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
					}
					break;
				case "SaveSkls":
                    {
						if (SaveSklsDialog.ShowDialog() == DialogResult.OK)
						{
							SaveSklsDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.SaveSklsMotions, TEMP_FILE_NAME, SaveSklsDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Motions successfully saved.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "SaveSkl":
                    {
						if (SaveSklDialog.ShowDialog(this.Handle))
						{
							SaveSklDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.SaveSklMotions, TEMP_FILE_NAME, SaveSklDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Motions successfully saved.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "LoadBones":
					{
						if (OpenBonesDialog.ShowDialog() == DialogResult.OK)
						{
							int code = StartEditor(EditorMode.LoadBones, TEMP_FILE_NAME, OpenBonesDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Bone data successfully loaded.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to load bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "SaveBones":
                    {
						if (SaveBonesDialog.ShowDialog() == DialogResult.OK)
						{
							SaveBonesDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.SaveBones, TEMP_FILE_NAME, SaveBonesDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Bone data successfully saved.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to save bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "SaveObj":
					{
						if (SaveObjDialog.ShowDialog() == DialogResult.OK)
						{
							SaveObjDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.ExportOBJ, TEMP_FILE_NAME, SaveObjDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Model successfully saved.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to save model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "GenerateShapes":
					{
						int code = StartEditor(EditorMode.GenerateShape, TEMP_FILE_NAME);
						if (code == 0)
							AutoClosingMessageBox.Show("Bone shapes successfully generated.", "", 1000, MessageBoxIcon.Information);
						else
							AutoClosingMessageBox.Show($"Can't generate bone shapes.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
					}
					break;
				case "LoadBoneParts":
					{
						if (OpenLtxDialog.ShowDialog() == DialogResult.OK)
						{
							int code = StartEditor(EditorMode.LoadBoneParts, TEMP_FILE_NAME, OpenLtxDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Bone parts successfully loaded.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to load bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "SaveBoneParts":
					{
						if (SaveBonePartsDialog.ShowDialog() == DialogResult.OK)
						{
							SaveBonePartsDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.SaveBoneParts, TEMP_FILE_NAME, SaveBonePartsDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Bone parts successfully saved.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to saved bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "ResetBoneParts":
					{
						int code = StartEditor(EditorMode.ToDefaultBoneParts, TEMP_FILE_NAME);
						if (code == 0)
							AutoClosingMessageBox.Show("Bone parts successfully reseted to default.", "", 1000, MessageBoxIcon.Information);
						else
							AutoClosingMessageBox.Show($"Failed to reset bone parts to default.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
					}
					break;
				case "ExportDM":
					{
						if (SaveDmDialog.ShowDialog() == DialogResult.OK)
						{
							SaveDmDialog.InitialDirectory = "";

							int code = StartEditor(EditorMode.ExportDM, TEMP_FILE_NAME, SaveDmDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Model successfully saved.", "", 1000, MessageBoxIcon.Information);
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
					break;
				case "GenerateLod":
					{
						GenerateLod Params = new GenerateLod();
						Params.ShowDialog();

						if (Params.res)
						{
							lod_quality = Params.lod_quality;
							lod_flags = 0;
							if (Params.progressive)
								lod_flags |= (1<<2);

							if (SaveOgfLodDialog.ShowDialog() == DialogResult.OK)
							{
								SaveOgfLodDialog.InitialDirectory = "";

								int code = StartEditor(EditorMode.GenerateLod, TEMP_FILE_NAME, SaveOgfLodDialog.FileName);
								if (code == 0)
								{
									AutoClosingMessageBox.Show("Lod successfully generated.", "", 1000, MessageBoxIcon.Information);

									if (SaveOgfLodDialog.FileName.Contains("meshes") && LodTextBox.Enabled)
									{
										string lod_path = SaveOgfLodDialog.FileName.Substring(SaveOgfLodDialog.FileName.LastIndexOf("meshes") + 7);
										LodTextBox.Text = lod_path;
									}
								}
								else
									AutoClosingMessageBox.Show($"Failed to generate lod.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "ExportCpp":
					{
						if (SaveCppDialog.ShowDialog() == DialogResult.OK)
						{
							SaveCppDialog.InitialDirectory = "";

							cpp_mode = idx;
							int code = StartEditor(EditorMode.SaveCpp, TEMP_FILE_NAME, SaveCppDialog.FileName);
							if (code == 0)
								AutoClosingMessageBox.Show("Model data successfully exported.", "", 1000, MessageBoxIcon.Information);
							else
								AutoClosingMessageBox.Show($"Failed to export model data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
						}
					}
					break;
				case "LoadUserData":
                    {
						if (OpenUserDataDialog.ShowDialog() == DialogResult.OK)
						{
							UserDataTextBox.Clear();

							StreamReader file = new StreamReader(OpenUserDataDialog.FileName);
							string line = file.ReadLine();
							UserDataTextBox.Text += line;
							while (line != null)
							{
								line = file.ReadLine();
								UserDataTextBox.Text += "\n" + line;
							}
							file.Close();
						}
					}
					break;
				case "ExportUserData":
					{
						if (SaveUserDataDialog.ShowDialog() == DialogResult.OK)
						{
							SaveUserDataDialog.InitialDirectory = "";
							File.WriteAllText(SaveUserDataDialog.FileName, UserDataTextBox.Text);
						}
					}
					break;
				case "LoadMotionRefs":
					{
						if (OpenMotionRefsDialog.ShowDialog() == DialogResult.OK)
						{
							MotionRefsBox.Clear();

							IniFile MotionRefs = new IniFile(OpenMotionRefsDialog.FileName);

							string Ref = MotionRefs.Read("000000", "refs");
							MotionRefsBox.Text += Ref;

							int counter = 0;
							while (Ref != "")
							{
								counter++;
								string param = string.Format("{0:000000}.", counter).Substring(0, 6);
								Ref = MotionRefs.Read(param, "refs");

								if (Ref == "")
									break;

								MotionRefsBox.Text += "\n" + Ref;
							}
						}
					}
					break;
				case "ExportMotionRefs":
					{
						if (SaveMotionRefsDialog.ShowDialog() == DialogResult.OK)
						{
							SaveMotionRefsDialog.InitialDirectory = "";

							string ExportRefs = "[refs]";

							List<string> motion_refs = new List<string>(); // корректные моушн рефы
							if (IsTextCorrect(MotionRefsBox.Text))
							{
								for (int i = 0; i < MotionRefsBox.Lines.Count(); i++)
								{
									if (IsTextCorrect(MotionRefsBox.Lines[i]))
										motion_refs.Add(GetCorrectString(MotionRefsBox.Lines[i]));
								}
							}

							for (int i = 0; i < motion_refs.Count; i++)
							{
								string val = string.Format("\n        {0:000000}                           = ", i);
								ExportRefs += $"{val}{motion_refs[i]}";
							}

							File.WriteAllText(SaveMotionRefsDialog.FileName, ExportRefs);
						}
					}
					break;
				case "BatchLtx":
					{
						if (OpenBatchLtxDialog.ShowDialog() == DialogResult.OK)
						{
							BatchFlags batch_flags = new BatchFlags(DEVELOPER_MODE);
							batch_flags.ShowDialog();

							if (batch_flags.res)
							{
								int code = StartEditor(EditorMode.BatchLtx, TEMP_FILE_NAME, OpenBatchLtxDialog.FileName, batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (code == 0)
									AutoClosingMessageBox.Show("Batch convert successful.", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Batch convert failed.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "BatchFilesToOGF":
					{
						if (OpenBatchDialog.ShowDialog() == DialogResult.OK && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(DEVELOPER_MODE);
							batch_flags.ShowDialog();

							batch_files.Add(OpenBatchDialog.FileNames);

							if (batch_flags.res)
							{
								int code = StartEditor(EditorMode.BatchDialogOGF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (code == 0)
									AutoClosingMessageBox.Show("Batch convert successful.", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "BatchFilesToOMF":
					{
						if (OpenBatchDialog.ShowDialog() == DialogResult.OK && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(DEVELOPER_MODE);
							batch_flags.ShowDialog();

							batch_files.Add(OpenBatchDialog.FileNames);

							if (batch_flags.res)
							{
								int code = StartEditor(EditorMode.BatchDialogOMF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (code == 0)
									AutoClosingMessageBox.Show("Batch convert successful.", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "BatchFoldersToOGF":
					{
						FolderSelectDialog OpenBatchFoldersDialog = new FolderSelectDialog();
						OpenBatchFoldersDialog.Multiselect = true;

						if (OpenBatchFoldersDialog.ShowDialog() && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(DEVELOPER_MODE);
							batch_flags.ShowDialog();

							for (int i = 0; i < OpenBatchFoldersDialog.FileNames.Count(); i++)
							{
								string source_folder = OpenBatchFoldersDialog.FileNames[i];
								source_folder = source_folder.Substring(0, source_folder.LastIndexOf('\\'));
								batch_source.Add(source_folder);
								string[] files = DirSearch(OpenBatchFoldersDialog.FileNames[i]);
								batch_files.Add(files);
							}

							if (batch_flags.res)
							{
								int code = StartEditor(EditorMode.BatchDialogOGF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (code == 0)
									AutoClosingMessageBox.Show("Batch convert successful.", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
				case "BatchFoldersToOMF":
					{
						FolderSelectDialog OpenBatchFoldersDialog = new FolderSelectDialog();
						OpenBatchFoldersDialog.Multiselect = true;

						if (OpenBatchFoldersDialog.ShowDialog() && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(DEVELOPER_MODE);
							batch_flags.ShowDialog();

							for (int i = 0; i < OpenBatchFoldersDialog.FileNames.Count(); i++)
							{
								string source_folder = OpenBatchFoldersDialog.FileNames[i];
								source_folder = source_folder.Substring(0, source_folder.LastIndexOf('\\'));
								batch_source.Add(source_folder);
								string[] files = DirSearch(OpenBatchFoldersDialog.FileNames[i]);
								batch_files.Add(files);
							}

							if (batch_flags.res)
							{
								int code = StartEditor(EditorMode.BatchDialogOMF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (code == 0)
									AutoClosingMessageBox.Show("Batch convert successful.", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						}
					}
					break;
			}
		}

		private void CopyObjectParams(string FileName)
		{
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(FileName, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_LODS, !FindBody, true)) // Импортируем LOD
				{
					LodTextBox.Text = xr_loader.read_stringData();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_CLASSSCRIPT, !FindBody, true)) // Импортируем UserData
				{
					UserDataTextBox.Text = xr_loader.read_stringZ();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS3, !FindBody, true)) // Импортируем Моушн рефы
				{
					List<string> refs = new List<string>();
					uint count = xr_loader.ReadUInt32();
					for (int i = 0; i < count; i++)
					{
						refs.Add(xr_loader.read_stringZ());
					}
					MotionRefsBox.Lines = refs.ToArray();
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS2, !FindBody, true))
				{
					string motions = xr_loader.read_stringZ();

					string motion = "";
					List<string> refs = new List<string>();

					for (int i = 0; i < motions.Length; i++)
					{
						if (motions[i] != ',')
							motion += motions[i];
						else
						{
							refs.Add(motion);
							motion = "";
						}

					}

					if (motion != "")
						refs.Add(motion);

					MotionRefsBox.Lines = refs.ToArray();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surface_count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surface_count; i++)
						{
							string name = xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							xr_loader.read_stringZ(); // Shader XRLC
							xr_loader.read_stringZ(); // GameMtl
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap
							surfaces[i].flags = xr_loader.ReadUInt32();   // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count

							CheckBox chbx = SurfacesPage.Controls[i].Controls[0] as CheckBox;
							chbx.Checked = (surfaces[i].flags == 1);
							SurfacesPage.Controls[i].Controls[2].Text = surfaces[i].texture;
							SurfacesPage.Controls[i].Controls[4].Text = surfaces[i].shader;
						}
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surface_count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surface_count; i++)
						{
							string name = xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							xr_loader.read_stringZ(); // Shader XRLC
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap
							surfaces[i].flags = xr_loader.ReadUInt32();   // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count

							CheckBox chbx = SurfacesPage.Controls[i].Controls[0] as CheckBox;
							chbx.Checked = (surfaces[i].flags == 1);
							SurfacesPage.Controls[i].Controls[2].Text = surfaces[i].texture;
							SurfacesPage.Controls[i].Controls[4].Text = surfaces[i].shader;
						}
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES, !FindBody, true))
				{
					uint surf_count = xr_loader.ReadUInt32();
					if (surface_count == surf_count && MessageBox.Show("Import textures and shaders path?\nThey may have different positions", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
					{
						for (int i = 0; i < surface_count; i++)
						{
							string name = xr_loader.read_stringZ(); // Name
							surfaces[i].shader = xr_loader.read_stringZ(); // Shader
							surfaces[i].flags = xr_loader.ReadByte();     // Flags
							xr_loader.ReadUInt32();   // FVF
							xr_loader.ReadUInt32();   // TC count
							surfaces[i].texture = xr_loader.read_stringZ(); // Texture
							xr_loader.read_stringZ(); // VMap

							CheckBox chbx = SurfacesPage.Controls[i].Controls[0] as CheckBox;
							chbx.Checked = (surfaces[i].flags == 1);
							SurfacesPage.Controls[i].Controls[2].Text = surfaces[i].texture;
							SurfacesPage.Controls[i].Controls[4].Text = surfaces[i].shader;
						}
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true));
				if (B_CHUNK)
				{
					int chunk = 0;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_SHAPE, false, true))
						{
							shapes[chunk].bone_type = (ushort)xr_loader.ReadUInt16();
							shapes[chunk].bone_flags = (ushort)xr_loader.ReadUInt16();

							ComboBox cb = BonesPage.Controls[chunk].Controls[4] as ComboBox;
							cb.SelectedIndex = shapes[chunk].bone_type;

							CheckBox chbx1 = BonesPage.Controls[chunk].Controls[0] as CheckBox;
							chbx1.Checked = (shapes[chunk].bone_flags & (1 << 0)) == (1 << 0);
							CheckBox chbx2 = BonesPage.Controls[chunk].Controls[1] as CheckBox;
							chbx2.Checked = (shapes[chunk].bone_flags & (1 << 1)) == (1 << 1);
							CheckBox chbx3 = BonesPage.Controls[chunk].Controls[2] as CheckBox;
							chbx3.Checked = (shapes[chunk].bone_flags & (1 << 2)) == (1 << 2);
							CheckBox chbx4 = BonesPage.Controls[chunk].Controls[3] as CheckBox;
							chbx4.Checked = (shapes[chunk].bone_flags & (1 << 3)) == (1 << 3);
						}

						chunk++;
						xr_loader.SetStream(temp);

						if (shapes.Count <= chunk) break;
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();

					if (size == shapes.Count)
					{
						for (int i = 0; i < size; i++)
						{
							shapes[i].bone_flags = 0;
							shapes[i].bone_type = 0;
							shapes[i].bone_name = xr_loader.read_stringZ();
							shapes[i].bone_id = (ushort)i;
							xr_loader.read_stringZ();
							xr_loader.read_stringZ();
							xr_loader.ReadBytes(12);
							xr_loader.ReadBytes(12);
							xr_loader.ReadFloat();

							ComboBox cb = BonesPage.Controls[i].Controls[4] as ComboBox;
							cb.SelectedIndex = shapes[i].bone_type;

							CheckBox chbx1 = BonesPage.Controls[i].Controls[0] as CheckBox;
							chbx1.Checked = (shapes[i].bone_flags & (1 << 0)) == (1 << 0);
							CheckBox chbx2 = BonesPage.Controls[i].Controls[1] as CheckBox;
							chbx2.Checked = (shapes[i].bone_flags & (1 << 1)) == (1 << 1);
							CheckBox chbx3 = BonesPage.Controls[i].Controls[2] as CheckBox;
							chbx3.Checked = (shapes[i].bone_flags & (1 << 2)) == (1 << 2);
							CheckBox chbx4 = BonesPage.Controls[i].Controls[3] as CheckBox;
							chbx4.Checked = (shapes[i].bone_flags & (1 << 3)) == (1 << 3);
						}
					}
				}
			}
		}

		private void LoadData()
		{
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_LODS, !FindBody, true))
				{
					LodTextBox.Text = xr_loader.read_stringData();
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_CLASSSCRIPT, !FindBody, true))
				{
					UserDataTextBox.Text = xr_loader.read_stringZ();
				}

				if (xr_loader.find_chunkSize((int)OBJECT.EOBJ_CHUNK_ACTORTRANSFORM, !FindBody, true) > 24)
				{
					xr_loader.ReadBytes(12);
					xr_loader.ReadBytes(12);
					model_scale = xr_loader.ReadFloat();
					ObjectScaleTextBox.Text = model_scale.ToString();
					uint chk = xr_loader.ReadUInt32();
					ScaleCenterOfMassCheckBox.Checked = Convert.ToBoolean(chk);
				}

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS3, !FindBody, true))
				{
					List<string> refs = new List<string>();
					uint count = xr_loader.ReadUInt32();
					for (int i = 0; i < count; i++)
                    {
						refs.Add(xr_loader.read_stringZ());
					}
					MotionRefsBox.Lines = refs.ToArray();
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS2, !FindBody, true))
				{
					string motions = xr_loader.read_stringZ();

					string motion = "";
					List<string> refs = new List<string>();

					for (int i = 0; i < motions.Length; i++)
                    {
						if (motions[i] != ',')
							motion += motions[i];
						else
						{
							refs.Add(motion);
							motion = "";
						}

					}

					if (motion != "")
						refs.Add(motion);

					MotionRefsBox.Lines = refs.ToArray();
				}

				SplitNormalsChbx.Enabled = false;
				SplitNormalsChbx.Checked = false;
				normalsToolStripMenuItem.Enabled = false;

				if (xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_EDITMESHES, !FindBody, true)))
				{
					int id = 0;
					uint size;

					while (true)
					{
						if (!xr_loader.find_chunk(id)) break;

						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(id, false, true))) break;

						size = xr_loader.find_chunkSize((int)MESH.EMESH_CHUNK_VERTS);
						if (size == 0) break;
						vertex_count += xr_loader.ReadUInt32();

						size = xr_loader.find_chunkSize((int)MESH.EMESH_CHUNK_FACES);
						if (size == 0) break;
						face_count += xr_loader.ReadUInt32();

						if (!SplitNormalsChbx.Enabled && xr_loader.find_chunk((int)MESH.EMESH_CHUNK_NORMALS, false, true))
						{
							SplitNormalsChbx.Enabled = true;
							SplitNormalsChbx.Checked = true;
							normalsToolStripMenuItem.Enabled = true;
						}

						id++;
						xr_loader.SetStream(temp);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				surfaces = new List<Surface>();
				Surface surface = new Surface();

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
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
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
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
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						string name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						surface.flags = xr_loader.ReadByte();     // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap

						surfaces.Add(surface);
						CreateMaterialGroupBox(i, name);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				shapes = new List<ShapeEditType>();
				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true));
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
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();
					for (int i = 0; i < size; i++)
					{
						ShapeEditType shape = new ShapeEditType();
						shape.bone_flags = 0;
						shape.bone_type = 0;
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

				joints_count = (uint)shapes.Count;
			}
		}

		private uint MotionCount()
		{
			uint count = 0;
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS, !FindBody, true))
					count = xr_loader.ReadUInt32();
			}
			return count;
		}

		private void ParseMotions()
		{
			var xr_loader = new XRayLoader();
			MotionTextBox.Clear();
			MotionTextBox.Text = $"Motions count: 0";
			bool hasmot = MotionCount() > 0;
			bool has_bones = HasBones();
			MotionFlagsGroupBox.Enabled = hasmot && has_bones;
			MotionRefsTextChanged(MotionRefsBox, null);

			if (IsOgfMode)
			{
				ObjectScaleTextBox.Enabled = has_bones && hasmot;
				ScaleCenterOfMassCheckBox.Enabled = has_bones && hasmot;
				ObjectScaleLabel.Enabled = has_bones && hasmot;
			}

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SMOTIONS, !FindBody, true))
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

		private bool HasBones()
		{
			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(TEMP_FILE_NAME, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				bool FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true) || xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
					return true;
			}
			return false;
		}

		private void FlagsHelpButton_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Motion export:\nДанные флаги влияют на компресиию анимаций при экспортировании в OMF.\n1. 8 bit - ТЧ Формат\n2. 16 bit - ЗП Формат\n" + (DEVELOPER_MODE ? "3. No compress - экспортирует анимации без сжатия\n" : "") +
				(DEVELOPER_MODE ? "4." : "3.") + " Use build-in motions - при активации, анимации загруженные в object будут использоваться вместо моушн референсов для всевозможных экспортов. При деактивации встроенные анимации будут игнорироваться и будут использованы моушн референсы.\n\n" +
				"Model export:\n" +
				"1. Make progressive meshes - создает прогрессивные меши при экспорте OGF. Это динамическая детализация модели (lod'ы), чаще используется для мировых объектов.\n" +
				"2. Make stripify meshes - оптимизация vertex'ов и face'ов у мешей которая портила сетку, раньше стояла по дефолту в SDK и использовалась для оптимизации мешей под старый DirectX и видеокарты. Сейчас же надобности в данном флаге нет.\n" +
				"3. Optimize surfaces - при включении объединяет меши с одинаковыми названиями текстур и шейдеров как и любой SDK. В данном эдиторе появилась возможность отключить это для последующих изменений через OGF Editor.\n" +
                "4. Use split normals - активируется только при наличии оригинальных нормалей у модели, которые экспортируют специальные плагины. При включении модель будет экспортированна с оригинальными нормалями модели, иначе со стандартными Smooth Groups\n" +
                "5. HQ Geometry+ - при активации компилятор будет экспортировать модель без оптимизаций vertex'ов и face'ов. \n\n" +
				"Object scale - изменяет размер объекта при экспорте, влияет на размер модели и размер анимаций.\nScale center of mass - при активации во время экспорта с измененным размером объекта будут пересчитаны центры массы коллизии под новый размер.\n\n" +
				"Surface params:\n" +
				"1. Surface->2 Sided - после экспорта OGF модель будет отрисовываться с наружной и внутренней стороны, в 2 раза увеличивает колличество полигонов у модели.\n\n" +
				"Shape params (Bones):\n" +
				"1. No Pickable - при активации Ray Query лучи, хит wallmark'и будут пропускать данный элемент.\n" +
				"2. Remove After Break - при активации у всех костей, после спавна объекта начнется таймер \"remove_time\" из конфига, при истечении которого объект удалится.\n" +
				"3. No Physics - при активации движок игнорирует физику шейпа.\n" +
				"4. No Fog Collider - при активации Volumetric fog будет игнорировать данный элемент.\n\n" +
				"Для создания коллизии с нуля нужно настроить Shape type параметры у каждой кости (можно воспользоваться Tools->Shape Params->Type helper) и далее нажать Tools->Shape Params->Generate Shapes.\nЕсли коллизия уже была сгенерирована, то Shape type можно менять без повторной генерации коллизии.\n\n" +
				"Tools->Surface Params и Tools->Shape Params активируются при выборе соответствующей вкладки в программе.\n\n" + 
				"Для создания lod модели нужно нажать Tools->Generate lod, появится окно с настройкой детализации лода, после нажатия кнопки Append сгенерируется lod модель. Если она была сохранена в геймдату игры, то референс лода автоматически пропишестя в текщую модель, иначе его нужно будет прописывать вручную.\n\n" +
				"Хоткеи:\nF3 - Экспорт\nF4 - Загрузка\nF5, Ctrl+S - Быстрое сохранение .object\nF6 - Сохранение"
				, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void ltxHelpToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show("Batch Converter создан для массового экспорта моделей и анимаций.\nВ данной программе имеется 2 режима: From Ltx и From Dialog.\n\n" +
				"From Ltx - Пример ltx конфига:\n\n" +
				"[ogf] ; секция из которой будут экспортироватсья ogf модели\n" +
                "test.object = test.ogf ; test.object из папки с ltx будет экспортирован в test.ogf\n" +
				"test2 = test3 ; можно указывать без форматов файлов, все равно будет работать\n" +
                "test\\test3 = test\\test3 ; так же можно прописывать папки\n\n" +
                "[omf] ; секция из которой будут экспортироваться omf анимации\n" +
                "test.object = test.omf ; встроенные анимации из test.object будут экспортированны в test.omf\n" +
				"test\\test = test\\test ; все так же можно прописывать без указания формата и в папках\n\n" +
                "[skls_skl] ; новая секция которая есть только в Object Editor, подгружает анимации в модели перед экспортом\n" +
				"test.object = test1.skl, test\\test2.skls, test3.skl ; все анимации из списка будут загружены в test.object перед экспортом в ogf и omf\n" +
				"test = test1, test\\test2, test3 ; указание без форматов и расположение в папках так же работает, программа будет искать анимации в skls и skl формате\n\n" +
                "From Dialog - принцип работы:\n" +
				"Object будет экспортирован в выбранный вами формат во вкладке From Dialog\nПри экспорте программа будет искать skls анимации лежащие рядом с ним с таким же названием что и у object, и при нахождении загрузит их в модель перед экспортом.\n\n" +
                "При выборе From File Dialog откроется окно выбора файлов, после выбора необходимых моделей откроется окно выбора папки куда будут сохранены все выбранные модели.\n" +
				"При выборе From Folder Dialog откроется окно выбора папок, после выбора папок с моделями откроется окно выбора папки куда будут сохранены все выбранные папки с моделями внутри."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void objectInfoToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show($"Vertex count: {vertex_count}\nFace count: {face_count}\nSurface count: {surface_count}\nJoints count: {joints_count}", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void IndexChanged(object sender, EventArgs e)
		{
			if (TabControl.SelectedIndex < 0 || !FlagsGroupBox.Enabled) return;

			shapeParamsToolStripMenuItem.Enabled = false;
			surfaceParamsToolStripMenuItem.Enabled = false;

			switch (TabControl.Controls[TabControl.SelectedIndex].Name)
			{
				case "BonesPage":
					int shapes_count = (shapes != null ? shapes.Count : 0);
					shapeParamsToolStripMenuItem.Enabled = shapes_count > 0;
					break;
				case "SurfacesPage":
					surfaceParamsToolStripMenuItem.Enabled = true;
					break;
			}
		}

		private void SwitchShapeType(object sender, EventArgs e)
		{
			ToolStripItem Item = sender as ToolStripItem;
			int type = Convert.ToInt32(Item.Tag.ToString().Split('_')[1]);

			int shapes_count = (shapes != null ? shapes.Count : 0);
			for (int i = 0; i < shapes_count; i++)
			{
				ShapeEditType shape = new ShapeEditType();
				shape.bone_id = shapes[i].bone_id;
				shape.bone_flags = shapes[i].bone_flags;
				shape.bone_name = shapes[i].bone_name;
				shape.bone_type = (ushort)type;
				shapes[i] = shape;

				ComboBox ShapeType = BonesPage.Controls[i].Controls[4] as ComboBox;
				ShapeType.SelectedIndex = type;
			}
		}

		private void importObjectParamsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog Dialog = new OpenFileDialog();
			Dialog.Filter = "Object file|*.object";

			if (Dialog.ShowDialog() == DialogResult.OK)
            {
				CopyObjectParams(Dialog.FileName);
            }
		}

		private void FastSaveObject(string filename)
        {
			StartEditor(EditorMode.SaveObject, TEMP_FILE_NAME);
			File.Copy(TEMP_FILE_NAME, filename, true);
			AutoClosingMessageBox.Show("Object successfully saved.", "", 1000, MessageBoxIcon.Information);
		}

        private void ClosingForm(object sender, FormClosingEventArgs e)
        {
			if (Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.Delete(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp", true);
		}

		private void CheckTempFileExist()
        {
			if (!File.Exists(TEMP_FILE_NAME) && File.Exists(FILE_NAME))
            {
				if (!Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
					Directory.CreateDirectory(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp");

				File.Copy(FILE_NAME, Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + $"\\temp\\{StatusFile.Text}", true);
			}
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

		private void enableAll2SidedToolStripMenuItem_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < surfaces.Count; i++)
			{
				Surface surface = new Surface();
				surface.flags = 1;
				surface.texture = surfaces[i].texture;
				surface.shader = surfaces[i].shader;
				surfaces[i] = surface;

				CheckBox TwoSided = SurfacesPage.Controls[i].Controls[0] as CheckBox;
				TwoSided.Checked = (surfaces[i].flags == 1);
			}
		}

		private void disableAll2SidedToolStripMenuItem_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < surfaces.Count; i++)
			{
				Surface surface = new Surface();
				surface.flags = 0;
				surface.texture = surfaces[i].texture;
				surface.shader = surfaces[i].shader;
				surfaces[i] = surface;

				CheckBox TwoSided = SurfacesPage.Controls[i].Controls[0] as CheckBox;
				TwoSided.Checked = (surfaces[i].flags == 1);
			}
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

		private string[] DirSearch(string sDir)
		{
			List<string> files = new List<string>();
			try
			{
				foreach (string f in Directory.GetFiles(sDir))
				{
					files.Add(f);
				}
				foreach (string d in Directory.GetDirectories(sDir))
				{
					files.AddRange(DirSearch(d));
				}
			}
			catch (System.Exception excpt)
			{
				MessageBox.Show(excpt.Message);
			}

			return files.ToArray();
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

		private void ScriptClicked(object sender, EventArgs e)
		{
			ToolStripMenuItem item = sender as ToolStripMenuItem;
			script = item.Tag.ToString();
			loadScriptToolStripMenuItem.Text = "Delete Script";

			for (int i = 0; i < loadScriptToolStripMenuItem.DropDownItems.Count; i++)
				loadScriptToolStripMenuItem.DropDownItems[i].Visible = false;
		}

		private void LoadScriptClicked(object sender, EventArgs e)
		{
			if (script != "null")
			{
				script = "null";
				loadScriptToolStripMenuItem.Text = "Load Script";

				for (int i = 0; i < loadScriptToolStripMenuItem.DropDownItems.Count; i++)
					loadScriptToolStripMenuItem.DropDownItems[i].Visible = true;
			}
		}

		private void CheckBoxCheckedChanged(object sender, EventArgs e)
		{
			CheckBox curBox = sender as CheckBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			Surface surface = new Surface();
			ShapeEditType shape = new ShapeEditType();

			switch (currentField)
			{
				case "chbx2sided":
					surface.flags = (uint)(curBox.Checked ? 1 : 0);
					surface.texture = surfaces[idx].texture;
					surface.shader = surfaces[idx].shader;
					surfaces[idx] = surface;
					break;
				case "chbxNoPickable":
					shape.bone_id = shapes[idx].bone_id;
					shape.bone_type = shapes[idx].bone_type;
					shape.bone_name = shapes[idx].bone_name;
					shape.bone_flags = (ushort)BitSet(shapes[idx].bone_flags, (1 << 0), curBox.Checked);
					shapes[idx] = shape;
					break;
				case "chbxNoPhysics":
					shape.bone_id = shapes[idx].bone_id;
					shape.bone_type = shapes[idx].bone_type;
					shape.bone_name = shapes[idx].bone_name;
					shape.bone_flags = (ushort)BitSet(shapes[idx].bone_flags, (1 << 1), curBox.Checked);
					shapes[idx] = shape;
					break;
				case "chbxRemoveAfterBreakCheckBoxBox":
					shape.bone_id = shapes[idx].bone_id;
					shape.bone_type = shapes[idx].bone_type;
					shape.bone_name = shapes[idx].bone_name;
					shape.bone_flags = (ushort)BitSet(shapes[idx].bone_flags, (1 << 2), curBox.Checked);
					shapes[idx] = shape;
					break;
				case "chbxNoFogColliderCheckBox":
					shape.bone_id = shapes[idx].bone_id;
					shape.bone_type = shapes[idx].bone_type;
					shape.bone_name = shapes[idx].bone_name;
					shape.bone_flags = (ushort)BitSet(shapes[idx].bone_flags, (1 << 3), curBox.Checked);
					shapes[idx] = shape;
					break;
			}
		}

		private void ComboBoxIndexChanged(object sender, EventArgs e)
		{
			ComboBox curBox = sender as ComboBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			ShapeEditType shape = new ShapeEditType();

			switch (currentField)
			{
				case "cbxType":
					shape.bone_id = shapes[idx].bone_id;
					shape.bone_type = (ushort)curBox.SelectedIndex;
					shape.bone_name = shapes[idx].bone_name;
					shape.bone_flags = shapes[idx].bone_flags;
					shapes[idx] = shape;
					break;
			}
		}

		private void EditorKeyDown(object sender, KeyEventArgs e)
		{
			if (e.Control && e.KeyCode == Keys.S)
				FastSaveObject(FILE_NAME);

			switch (e.KeyData)
			{
				case Keys.F3:
					fileToolStripMenuItem.ShowDropDown();
					exportToolStripMenuItem.ShowDropDown();
					break;
				case Keys.F4:
					fileToolStripMenuItem.ShowDropDown();
					loadToolStripMenuItem.ShowDropDown();
					break;
				case Keys.F5: FastSaveObject(FILE_NAME); break;
				case Keys.F6:
					fileToolStripMenuItem.ShowDropDown();
					saveToolStripMenuItem.ShowDropDown();
					break;
			}
		}

		private int BitSet(int flags, int mask, bool bvalue)
		{
			if (bvalue)
				return flags |= mask;
			else
				return flags &= ~mask;
		}

		private void ProgressiveMeshes_CheckedChanged(object sender, EventArgs e)
		{
			CheckBox chbx = sender as CheckBox;

			if (chbx.Checked)
				StripifyMeshes.Checked = false;
		}

		private void StripifyMeshes_CheckedChanged(object sender, EventArgs e)
		{
			CheckBox chbx = sender as CheckBox;

			if (chbx.Checked)
				ProgressiveMeshes.Checked = false;
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (MessageBox.Show("Are you sure you want to exit?", "Object Editor", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
				Close();
		}

		private void RichTextBoxImgDefender(object sender, KeyEventArgs e)
		{
			RichTextBox TextBox = sender as RichTextBox;
			if (e.Control && e.KeyCode == Keys.V)
			{
				if (Clipboard.ContainsText())
					TextBox.Paste(DataFormats.GetFormat(DataFormats.Text));
				e.Handled = true;
			}
		}

		private bool IsTextCorrect(string text)
		{
			foreach (char ch in text)
			{
				if (ch > 0x1F && ch != 0x20)
					return true;
			}
			return false;
		}

		private string GetCorrectString(string text)
		{
			string ret_text = "", symbols = "";
			bool started = false;
			foreach (char ch in text)
			{
				if (started)
				{
					if (ch <= 0x1F || ch == 0x20)
						symbols += ch;
					else
					{
						ret_text += symbols + ch;
						symbols = "";
					}
				}
				else if (ch > 0x1F && ch != 0x20)
				{
					started = true;
					ret_text += ch;
				}
			}
			return ret_text;
		}

		private void MotionRefsTextChanged(object sender, EventArgs e)
		{
			motionRefsToolStripMenuItem1.Enabled = IsTextCorrect((sender as RichTextBox).Text) && MotionRefsBox.Enabled;
		}

		private void UserDataTextChanged(object sender, EventArgs e)
		{
			userDataToolStripMenuItem1.Enabled = IsTextCorrect((sender as RichTextBox).Text) && UserDataTextBox.Enabled;
		}

		private void InitDialogs()
        {
			string old_filename = FILE_NAME;
			if (IsOgfMode)
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

			SaveBonePartsDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveBonePartsDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + "_boneparts.ltx";

			SaveCppDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveCppDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + "_cpp.ltx";

			SaveMotionRefsDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveMotionRefsDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + "_refs.ltx";

			SaveUserDataDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveUserDataDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + "_userdata.ltx";

			SaveSklDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));

			SaveDmDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveDmDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".dm";

			SaveObjectDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveObjectDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + ".object";

			SaveOgfLodDialog.InitialDirectory = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('\\'));
			SaveOgfLodDialog.FileName = StatusFile.Text.Substring(0, StatusFile.Text.LastIndexOf('.')) + "_lod.ogf";

			FILE_NAME = old_filename;
		}

		private void InitUI()
        {
			bool has_motions = MotionCount() > 0;
			bool has_bones = HasBones();

			DeletesklsToolStripMenuItem.Enabled = has_motions;
			SaveSklsToolStripMenuItem.Enabled = has_motions;
			sklToolStripMenuItem.Enabled = has_motions;
			oMFToolStripMenuItem.Enabled = has_motions;
			saveToolStripMenuItem.Enabled = !IsOgfMode;
			exportToolStripMenuItem.Enabled = true;
			deleteToolStripMenuItem.Enabled = true;
			sklSklsToolStripMenuItem.Enabled = has_bones;
			bonesToolStripMenuItem.Enabled = has_bones;
			oGFToolStripMenuItem.Enabled = !IsOgfMode;
			objToolStripMenuItem.Enabled = !IsOgfMode;
			objectToolStripMenuItem.Enabled = !IsOgfMode;
			dMToolStripMenuItem.Enabled = !IsOgfMode;
			bonesPartsToolStripMenuItem.Enabled = has_bones;
			cToolStripMenuItem.Enabled = !IsOgfMode;
			StripifyMeshes.Enabled = has_bones;
			bonesToolStripMenuItem1.Enabled = has_bones;
			bonesPartsToolStripMenuItem1.Enabled = has_bones;
			bonesPartsToDefaultToolStripMenuItem.Enabled = has_bones;
			ObjectScaleTextBox.Enabled = has_bones && !IsOgfMode;
			ScaleCenterOfMassCheckBox.Enabled = has_bones && !IsOgfMode;
			ObjectScaleLabel.Enabled = has_bones && !IsOgfMode;
			MotionRefsBox.Enabled = has_bones;
			UserDataTextBox.Enabled = has_bones;
			LodTextBox.Enabled = has_bones;
			motionRefsToolStripMenuItem.Enabled = has_bones && !IsOgfMode;
			userDataToolStripMenuItem.Enabled = has_bones && !IsOgfMode;
			ModelFlagsGroupBox.Enabled = !IsOgfMode;
			FlagsGroupBox.Enabled = true;
			generateLodToolStripMenuItem.Enabled = !IsOgfMode;
			objectInfoToolStripMenuItem.Enabled = !IsOgfMode;
			importObjectParamsToolStripMenuItem.Enabled = !IsOgfMode;

			if (IsOgfMode)
			{
				TabControl.Controls.Clear();
				TabControl.Controls.Add(FlagsPage);
				TabControl.Controls.Add(MotionPage);
			}
		}

		private void CreateShapeGroupBox(int idx, ShapeEditType shape)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 106 * idx);
			GroupBox.Size = new System.Drawing.Size(362, 100);
			GroupBox.Text = "Bone name: " + shape.bone_name;
			GroupBox.Name = "ShapeGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			CreateShapeFlags(idx, GroupBox, shape);
			BonesPage.Controls.Add(GroupBox);
		}

		private void CreateShapeFlags(int idx, GroupBox box, ShapeEditType shape)
		{
			var NoPickableCheckBoxBox = new CheckBox();
			NoPickableCheckBoxBox.Name = "chbxNoPickable_" + idx;
			NoPickableCheckBoxBox.Text = "No Pickable";
			NoPickableCheckBoxBox.Size = new System.Drawing.Size(130, 23);
			NoPickableCheckBoxBox.Location = new System.Drawing.Point(6, 15);
			NoPickableCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoPickableCheckBoxBox.Checked = (shape.bone_flags & (1 << 0)) == (1 << 0);
			NoPickableCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var NoPhysicsCheckBoxBox = new CheckBox();
			NoPhysicsCheckBoxBox.Name = "chbxNoPhysics_" + idx;
			NoPhysicsCheckBoxBox.Text = "No Physics";
			NoPhysicsCheckBoxBox.Size = new System.Drawing.Size(100, 23);
			NoPhysicsCheckBoxBox.Location = new System.Drawing.Point(6, 35);
			NoPhysicsCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoPhysicsCheckBoxBox.Checked = (shape.bone_flags & (1 << 1)) == (1 << 1);
			NoPhysicsCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var RemoveAfterBreakCheckBoxBox = new CheckBox();
			RemoveAfterBreakCheckBoxBox.Name = "chbxRemoveAfterBreakCheckBoxBox_" + idx;
			RemoveAfterBreakCheckBoxBox.Text = "Remove After Break";
			RemoveAfterBreakCheckBoxBox.Size = new System.Drawing.Size(160, 23);
			RemoveAfterBreakCheckBoxBox.Location = new System.Drawing.Point(6, 55);
			RemoveAfterBreakCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			RemoveAfterBreakCheckBoxBox.Checked = (shape.bone_flags & (1 << 2)) == (1 << 2);
			RemoveAfterBreakCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var NoFogColliderCheckBoxBox = new CheckBox();
			NoFogColliderCheckBoxBox.Name = "chbxNoFogColliderCheckBox_" + idx;
			NoFogColliderCheckBoxBox.Text = "No Fog Collider ";
			NoFogColliderCheckBoxBox.Size = new System.Drawing.Size(140, 23);
			NoFogColliderCheckBoxBox.Location = new System.Drawing.Point(6, 75);
			NoFogColliderCheckBoxBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			NoFogColliderCheckBoxBox.Checked = (shape.bone_flags & (1 << 3)) == (1 << 3);
			NoFogColliderCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var TypeLabel = new Label();
			TypeLabel.Name = "TypeLbl_" + idx;
			TypeLabel.Text = "Shape type: ";
			TypeLabel.Location = new System.Drawing.Point(180, 18);
			TypeLabel.Anchor = AnchorStyles.Right;

			var TypeComboBox = new ComboBox();
			TypeComboBox.Name = "cbxType_" + idx;
			TypeComboBox.Size = new System.Drawing.Size(107, 23);
			TypeComboBox.Location = new System.Drawing.Point(245, 15);
			TypeComboBox.Anchor = AnchorStyles.Right;
			TypeComboBox.Items.Add("None");
			TypeComboBox.Items.Add("Box");
			TypeComboBox.Items.Add("Sphere");
			TypeComboBox.Items.Add("Cylinder");
			TypeComboBox.SelectedIndex = shape.bone_type;
			TypeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
			TypeComboBox.SelectedIndexChanged += new System.EventHandler(this.ComboBoxIndexChanged);

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
			GroupBox.Size = new System.Drawing.Size(362, 108);
			GroupBox.Text = name;
			GroupBox.Name = "MaterialGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			CreateMaterialFlags(idx, GroupBox);
			SurfacesPage.Controls.Add(GroupBox);
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
			TextureTextBox.Size = new System.Drawing.Size(299, 23);
			TextureTextBox.Location = new System.Drawing.Point(55, 30);
			TextureTextBox.TextChanged += new System.EventHandler(this.TextBoxFilter);
			TextureTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;

			string texture_path = surfaces[idx].texture;
			if (surfaces[idx].texture.LastIndexOf('.') != -1)
			{
				string format = surfaces[idx].texture.Substring(surfaces[idx].texture.LastIndexOf('.') + 1, surfaces[idx].texture.Length - surfaces[idx].texture.LastIndexOf('.') - 1);
				if (format.Length == 3) // Хз нужно ли или нет, но у основных форматов длинна названяи в 3 символа, пусть будет проверкой
					texture_path = surfaces[idx].texture.Substring(0, surfaces[idx].texture.LastIndexOf('.'));
			}
			TextureTextBox.Text = texture_path;

			var ShaderLabel = new Label();
			ShaderLabel.Text = "Shader:";
			ShaderLabel.Name = "ShaderLabel_" + idx;
			ShaderLabel.Size = new System.Drawing.Size(46, 23);
			ShaderLabel.Location = new System.Drawing.Point(5, 60);
			ShaderLabel.Anchor = AnchorStyles.Left | AnchorStyles.Top;

			var ShaderTextBox = new TextBox();
			ShaderTextBox.Name = "ShaderTextBox_" + idx;
			ShaderTextBox.Size = new System.Drawing.Size(299, 23);
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
    }
}
