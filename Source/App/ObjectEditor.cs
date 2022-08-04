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
using System.Threading;
using System.Diagnostics;

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
		ExportOBJOptimized,
	};

	public partial class Object_Editor : Form
	{
		public class Bone
		{
			public ushort bone_id;
			public ushort shape_type;
			public ushort shape_flags;
			public string bone_name;
			public string material;
			public float  mass;
			public ushort joint_type;
			public Fvector position;
			public Fvector rotation;
			public Fvector center_of_mass;
			public bool breakable;
			public float spring;
			public float damping;
			public float friction;

			public Bone()
            {
				bone_id = 0;
				shape_type = 0;
				shape_flags = 0;
				joint_type = 0;
				bone_name = "Error! Not loaded!";
				material = "Error! Not loaded!";
				mass = 10.0f;
				position = new Fvector();
				rotation = new Fvector();
				center_of_mass = new Fvector();
				breakable = false;
				spring = 0.0f;
				damping = 0.0f;
				friction = 0.0f;
			}
		};

		public class Surface
		{
			public uint flags;
			public string texture;
			public string shader;
			public string name;

			public Surface()
            {
				flags = 0;
				texture = "Error! Not loaded!";
				shader = "Error! Not loaded!";
				name = "Error! Not loaded!";
			}
		};

		// File sytem
		public string FILE_NAME = "";
		public string TEMP_FILE_NAME = "";
		public bool dbg_window = false;
		public List<Bone> bones;
		public List<Surface> surfaces;
		public float model_scale = 1.0f;
		public float lod_quality = 0.5f;
		public int lod_flags = 0;
		EditorSettings pSettings = null;
		FolderSelectDialog SaveSklDialog = null;
		FolderSelectDialog OpenBatchOutDialog = null;
        List<string[]> batch_files = null;
		List<string> batch_source = null;
		public bool IsOgfMode = false;
		public string script = "null";
		public string SCRIPT_FOLDER = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\scripts\\";

		// Info
		public uint vertex_count = 0;
		public uint face_count = 0;
		public uint surface_count = 0;
		public uint joints_count = 0;

		// Input
		public bool bKeyIsDown = false;
		Size CurrentSize = new Size();
		Size BoneSize = new Size();

		// Other
		public int cpp_mode = 0;
		public string[] game_materials = { };
		public Thread SdkThread = null;
		public bool NORMALS_DEFAULT = true;
		System.Diagnostics.Process EditorProcess = null;
		public bool EditorWorking = false;
		public bool EditorKilled = false;
		public double dLastTime = 0.0;

		// Settings
		public bool USE_OLD_BONES = true;

		public Object_Editor()
		{
			InitializeComponent();

			InitUI();

			EditorProcess = new System.Diagnostics.Process();
			EditorProcess.OutputDataReceived += SortOutputHandler;
			EditorProcess.StartInfo.UseShellExecute = false;

			Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");

			SaveSklDialog = new FolderSelectDialog();
			OpenBatchOutDialog = new FolderSelectDialog();
			batch_files = new List<string[]>();
			batch_source = new List<string>();

			string file_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\Settings.ini";
			pSettings = new EditorSettings(file_path);

			bool NoCompress = false;
			bool Debug = false;
			string gamemtl = "";
			pSettings.LoadText("GameMtlPath", ref gamemtl);
			pSettings.LoadState("NoCompress", ref NoCompress);
			pSettings.LoadState("Debug", ref Debug);
			pSettings.Load(ProgressiveMeshes);
			pSettings.Load(StripifyMeshes);
			pSettings.Load(OptimizeSurfaces);
			pSettings.Load(SoCInfluence);
			pSettings.LoadState("SplitNormalsChbx", ref NORMALS_DEFAULT, true);
			pSettings.Load(BuildInMotionsExport, true);
			pSettings.Load(SmoothSoC);
			pSettings.Load(SmoothCoP, true);
			pSettings.Load(Anims8Bit);
			pSettings.Load(Anims16Bit, !NoCompress);
			pSettings.Load(AnimsNoCompress, NoCompress);
			pSettings.Load(HQGeometry);
			pSettings.Load(HQGeometryPlus, true);
			pSettings.Load(ScaleCenterOfMassCheckBox, true);

			if (File.Exists(gamemtl))
				game_materials = GameMtlParser(gamemtl);

#if DEBUG
			Debug = true;
			NoCompress = true;
			dbg_window = true;
			showWindowToolStripMenuItem.Enabled = false;
#endif

			if (System.Diagnostics.Debugger.IsAttached)
            {
				dbg_window = true;
				showWindowToolStripMenuItem.Enabled = false;
			}

			debugToolStripMenuItem.Visible = Debug;

			if (!NoCompress)
            {
				AnimsNoCompress.Visible = false;
				BuildInMotionsExport.Location = AnimsNoCompress.Location;
				MotionFlagsGroupBox.Size = new Size(MotionFlagsGroupBox.Size.Width, MotionFlagsGroupBox.Size.Height - 22);
				ScaleGroupBox.Location = new Point(ScaleGroupBox.Location.X, ScaleGroupBox.Location.Y - 22);
				ScaleGroupBox.Size = new Size(ScaleGroupBox.Size.Width, ScaleGroupBox.Size.Height + 22);
			}

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

			FILE_NAME = filename;
			StatusFile.Text = FILE_NAME.Substring(FILE_NAME.LastIndexOf('\\') + 1);

			string TempFile = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + $"\\temp\\{StatusFile.Text}";
			File.Copy(FILE_NAME, TempFile, true);
			TEMP_FILE_NAME = TempFile;

			InitDialogs();
			UpdateUI();
			LoadData();
			ParseMotions();

            InitSurfaceUI();
			InitBoneUI();

			IndexChanged(null, null);

			if (!IsOgfMode)
			{
				MotionRefsTextChanged(MotionRefsBox, null);
				UserDataTextChanged(UserDataTextBox, null);
			}
		}

		private int StartEditor(bool async, EditorMode mode, string object_path, string second_path = "null", int flags = -1, float scale = 1.0f, string[] temp_arr = null)
		{
			if (flags == -1)
            {
				flags = GetFlags();
				scale = model_scale;
			}

			int shapes_count = (bones != null ? bones.Count : 0);
			int surfaces_count = (surfaces != null ? surfaces.Count : 0);
			string args = $"{(int)mode} \"{object_path}\" \"{second_path}\" {flags} {scale} {shapes_count} {surfaces_count} {OpenSklsDialog.FileNames.Count()}";

			// Экспортируем шейпы
			if (shapes_count > 0)
			{
				for (int i = 0; i < bones.Count; i++)
				{
					args += $" \"{bones[i].bone_id}-{bones[i].shape_type}-{bones[i].shape_flags}\"";
					args += $" \"{bones[i].material}\"";
					args += $" {bones[i].mass}";
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
					args += $" {batch_files[i].Count()}";
					for (int cnt = 0; cnt < batch_files[i].Count(); cnt++)
					{
						args += $" \"{batch_files[i][cnt]}\"";
					}
				}
			}
            args += $" \"{OpenBatchOutDialog.FileName}\"";

            batch_files.Clear();
			batch_source.Clear();

			// Экспорт режима экспорта c++
			args += $" {cpp_mode}";

			// Экспорт скрипта
			args += $" \"{script}\"";

			// Экспорт названия ориг модели
			args += $" \"{FILE_NAME}\"";

			// Лист текстур для просмотра моделей
			args += temp_arr == null ? " 0" : $" {temp_arr.Count() / 2}";
			if (temp_arr != null)
            {
				for (int i = 0; i < temp_arr.Count(); i++)
				{
					args += $" \"{temp_arr[i]}\"";
				}
			}

			// Временная папка со всеми данными программы
			args += $" \"{Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\'))}\\temp\"";

			int exit_code = RunCompiller(args, async);

			if (File.Exists(object_path + "_temp.userdata"))
				File.Delete(object_path + "_temp.userdata");
			return exit_code;
		}

		private int RunCompiller(string args, bool async)
		{
			string exe_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\Object Editor.exe";
			if (File.Exists(exe_path))
			{
				if (async)
				{
					this.Invoke((MethodInvoker)delegate ()
					{
						LogTextBox.Clear();
						LogTextBox.SelectionStart = LogTextBox.Text.Length;
						LogTextBox.ScrollToCaret();
					});
				}
                CheckTempFileExist();
				EditorProcess.StartInfo.FileName = exe_path;
				EditorProcess.StartInfo.WorkingDirectory = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\'));
				EditorProcess.StartInfo.Arguments = args;
				EditorProcess.StartInfo.CreateNoWindow = !dbg_window;
				EditorProcess.StartInfo.RedirectStandardOutput = !dbg_window;
				EditorProcess.StartInfo.RedirectStandardError = !dbg_window;
				EditorProcess.Start();
				EditorWorking = true;

				if (async && !dbg_window)
					EditorProcess.BeginOutputReadLine();

				EditorProcess.WaitForExit();
				EditorWorking = false;

				if (async && !dbg_window)
					EditorProcess.CancelOutputRead();

				dLastTime = Math.Round((EditorProcess.ExitTime - EditorProcess.StartTime).TotalSeconds, 3);

				if (async)
				{
                    this.Invoke((MethodInvoker)delegate ()
                    {
                        LogTextBox.AppendText("\n" + GetTime());
                        LogTextBox.SelectionStart = LogTextBox.Text.Length;
                        LogTextBox.ScrollToCaret();
                    });
                }

				int code = EditorProcess.ExitCode;
                EditorProcess.Close();
                return code;
			}
			else
			{
				MessageBox.Show("Can't find Object Editor.exe", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return -1;
			}
		}

		private void SortOutputHandler(object sendingProcess, DataReceivedEventArgs outLine)
		{
			if (outLine.Data != null)
			{
				this.Invoke((MethodInvoker)delegate () {
					LogTextBox.AppendText(outLine.Data + "\n");
					LogTextBox.SelectionStart = LogTextBox.Text.Length;
					LogTextBox.ScrollToCaret();
				});
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

			if (OptimizeSurfaces.Checked)
				flags |= (1 << 3);

			if (dbg_window)
				flags |= (1 << 4);

			if (ScaleCenterOfMassCheckBox.Checked)
				flags |= (1 << 5);

			if (HQGeometryPlus.Checked)
				flags |= (1 << 6);

			if (StripifyMeshes.Checked)
				flags |= (1 << 7);

			if (UseSplitNormals.Checked)
				flags |= (1 << 8);

			if (BuildInMotionsExport.Checked)
				flags |= (1 << 9);

			if (SmoothSoC.Checked)
				flags |= (1 << 10);

			if (SoCInfluence.Checked)
				flags |= (1 << 11);

			return flags;
        }

		private bool CheckThread()
        {
			if (SdkThread != null && SdkThread.ThreadState != System.Threading.ThreadState.Stopped)
			{
				AutoClosingMessageBox.Show("Wait for another process to complete.", "", 800, MessageBoxIcon.Information);
				return false;
			}
			return true;
		}

		private void RunSDK_Click(object sender, EventArgs e)
		{
			if (!CheckThread()) return;

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

							SdkThread = new Thread(() => { FastSaveObject(SaveObjectDialog.FileName); });
							SdkThread.Start();
						}
					}
					break;
				case "ExportOGF":
                    {
						if (SaveOgfDialog.ShowDialog() == DialogResult.OK)
						{
							SaveOgfDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.ExportOGF, TEMP_FILE_NAME, SaveOgfDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Model successfully exported. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
									{
										if (code == 1)
											MessageBox.Show("Can't export model.\nPlease, disable HQ Geometry+ flag.", "", MessageBoxButtons.OK, MessageBoxIcon.Error);
										else
											AutoClosingMessageBox.Show($"Can't export model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "ExportOMF":
                    {
						if (SaveOmfDialog.ShowDialog() == DialogResult.OK)
						{
							SaveOmfDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.ExportOMF, TEMP_FILE_NAME, SaveOmfDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Motions successfully exported. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Can't export motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "LoadSkls":
                    {
						if (OpenSklsDialog.ShowDialog() == DialogResult.OK)
						{
							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.LoadMotions, TEMP_FILE_NAME);
								if (!EditorKilled)
								{
									if (code == 0)
									{
										AutoClosingMessageBox.Show($"Motions successfully loaded. {GetTime()}", "", 1000, MessageBoxIcon.Information);
										DeletesklsToolStripMenuItem.Enabled = true;
										SaveSklsToolStripMenuItem.Enabled = true;
										sklToolStripMenuItem.Enabled = true;
										oMFToolStripMenuItem.Enabled = true;
									}
									else
										AutoClosingMessageBox.Show($"Can't load motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);

									ParseMotions();
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "DeleteSkls":
                    {
						SdkThread = new Thread(() => {
							int code = StartEditor(true, EditorMode.DeleteMotions, TEMP_FILE_NAME);
							if (!EditorKilled)
							{
								if (code == 0)
								{
									AutoClosingMessageBox.Show($"Motions successfully deleted. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									DeletesklsToolStripMenuItem.Enabled = false;
									SaveSklsToolStripMenuItem.Enabled = false;
									sklToolStripMenuItem.Enabled = false;
									oMFToolStripMenuItem.Enabled = false;

									ParseMotions();
								}
								else
									AutoClosingMessageBox.Show($"Can't delete motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						});
						SdkThread.Start();
					}
					break;
				case "SaveSkls":
                    {
						if (SaveSklsDialog.ShowDialog() == DialogResult.OK)
						{
							SaveSklsDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.SaveSklsMotions, TEMP_FILE_NAME, SaveSklsDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Motions successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "SaveSkl":
                    {
						if (SaveSklDialog.ShowDialog(this.Handle))
						{
							SaveSklDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.SaveSklMotions, TEMP_FILE_NAME, SaveSklDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Motions successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Can't save motions.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "LoadBones":
					{
						if (OpenBonesDialog.ShowDialog() == DialogResult.OK)
						{
							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.LoadBones, TEMP_FILE_NAME, OpenBonesDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Bone data successfully loaded. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to load bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "SaveBones":
                    {
						if (SaveBonesDialog.ShowDialog() == DialogResult.OK)
						{
							SaveBonesDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.SaveBones, TEMP_FILE_NAME, SaveBonesDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Bone data successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to save bone data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "ExportOBJ":
					{
						if (SaveObjDialog.ShowDialog() == DialogResult.OK)
						{
							SaveObjDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.ExportOBJ, TEMP_FILE_NAME, SaveObjDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Model successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to save model.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "GenerateShapes":
					{
						SdkThread = new Thread(() => {
							int code = StartEditor(true, EditorMode.GenerateShape, TEMP_FILE_NAME);
							if (!EditorKilled)
							{
								if (code == 0)
									AutoClosingMessageBox.Show($"Bone shapes successfully generated. {GetTime()}", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Can't generate bone shapes.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						});
						SdkThread.Start();
					}
					break;
				case "LoadBoneParts":
					{
						if (OpenLtxDialog.ShowDialog() == DialogResult.OK)
						{
							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.LoadBoneParts, TEMP_FILE_NAME, OpenLtxDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Bone parts successfully loaded. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to load bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "SaveBoneParts":
					{
						if (SaveBonePartsDialog.ShowDialog() == DialogResult.OK)
						{
							SaveBonePartsDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.SaveBoneParts, TEMP_FILE_NAME, SaveBonePartsDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Bone parts successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to saved bone parts.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "ResetBoneParts":
					{
						SdkThread = new Thread(() => {
							int code = StartEditor(true, EditorMode.ToDefaultBoneParts, TEMP_FILE_NAME);
							if (!EditorKilled)
							{
								if (code == 0)
									AutoClosingMessageBox.Show($"Bone parts successfully reseted to default. {GetTime()}", "", 1000, MessageBoxIcon.Information);
								else
									AutoClosingMessageBox.Show($"Failed to reset bone parts to default.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
							}
						});
						SdkThread.Start();
					}
					break;
				case "ExportDM":
					{
						if (SaveDmDialog.ShowDialog() == DialogResult.OK)
						{
							SaveDmDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								int code = StartEditor(true, EditorMode.ExportDM, TEMP_FILE_NAME, SaveDmDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Model successfully saved. {GetTime()}", "", 1000, MessageBoxIcon.Information);
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
							});
							SdkThread.Start();
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

								SdkThread = new Thread(() => {
									int code = StartEditor(true, EditorMode.GenerateLod, TEMP_FILE_NAME, SaveOgfLodDialog.FileName);
									if (!EditorKilled)
									{
										if (code == 0)
										{
											AutoClosingMessageBox.Show($"Lod successfully generated. {GetTime()}", "", 1000, MessageBoxIcon.Information);

											if (SaveOgfLodDialog.FileName.Contains("meshes") && LodTextBox.Enabled)
											{
												string lod_path = SaveOgfLodDialog.FileName.Substring(SaveOgfLodDialog.FileName.LastIndexOf("meshes") + 7);
												LodTextBox.Text = lod_path;
											}
										}
										else
											AutoClosingMessageBox.Show($"Failed to generate lod.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								});
								SdkThread.Start();
							}
						}
					}
					break;
				case "ExportCpp":
					{
						if (SaveCppDialog.ShowDialog() == DialogResult.OK)
						{
							SaveCppDialog.InitialDirectory = "";

							SdkThread = new Thread(() => {
								cpp_mode = idx;
								int code = StartEditor(true, EditorMode.SaveCpp, TEMP_FILE_NAME, SaveCppDialog.FileName);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Model data successfully exported. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Failed to export model data.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							});
							SdkThread.Start();
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
							BatchFlags batch_flags = new BatchFlags(pSettings);
							batch_flags.ShowDialog();

							if (batch_flags.res)
							{
								int code = StartEditor(true, EditorMode.BatchLtx, TEMP_FILE_NAME, OpenBatchLtxDialog.FileName, batch_flags.GetFlags(dbg_window), batch_flags.scale);
								if (!EditorKilled)
								{
									if (code == 0)
										AutoClosingMessageBox.Show($"Batch convert successful. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									else
										AutoClosingMessageBox.Show($"Batch convert failed.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
								}
							}
						}
					}
					break;
				case "BatchFilesToOGF":
					{
						if (OpenBatchDialog.ShowDialog() == DialogResult.OK && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(pSettings);
							batch_flags.ShowDialog();

							batch_files.Add(OpenBatchDialog.FileNames);

							if (batch_flags.res)
							{
								SdkThread = new Thread(() => {
									int code = StartEditor(true, EditorMode.BatchDialogOGF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
									if (!EditorKilled)
									{
										if (code == 0)
											AutoClosingMessageBox.Show($"Batch convert successful. {GetTime()}", "", 1000, MessageBoxIcon.Information);
										else
											AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								});
								SdkThread.Start();
							}
						}
					}
					break;
				case "BatchFilesToOMF":
					{
						if (OpenBatchDialog.ShowDialog() == DialogResult.OK && OpenBatchOutDialog.ShowDialog())
						{
							BatchFlags batch_flags = new BatchFlags(pSettings);
							batch_flags.ShowDialog();

							batch_files.Add(OpenBatchDialog.FileNames);

							if (batch_flags.res)
							{
								SdkThread = new Thread(() => {
									int code = StartEditor(true, EditorMode.BatchDialogOMF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
									if (!EditorKilled)
									{
										if (code == 0)
											AutoClosingMessageBox.Show($"Batch convert successful. {GetTime()}", "", 1000, MessageBoxIcon.Information);
										else
											AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								});
								SdkThread.Start();
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
							BatchFlags batch_flags = new BatchFlags(pSettings);
							batch_flags.ShowDialog();

							for (int i = 0; i < OpenBatchFoldersDialog.FileNames.Count(); i++)
							{
								string source_folder = OpenBatchFoldersDialog.FileNames[i];
								source_folder = source_folder.Substring(0, source_folder.LastIndexOf('\\'));
								batch_source.Add(source_folder);
								string[] files = DirSearch(OpenBatchFoldersDialog.FileNames[i]);
								files = SortFormat(files, "object");
								batch_files.Add(files);
							}

							if (batch_flags.res)
							{
								SdkThread = new Thread(() => {
									int code = StartEditor(true, EditorMode.BatchDialogOGF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
									if (!EditorKilled)
									{
										if (code == 0)
											AutoClosingMessageBox.Show($"Batch convert successful. {GetTime()}", "", 1000, MessageBoxIcon.Information);
										else
											AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								});
								SdkThread.Start();
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
							BatchFlags batch_flags = new BatchFlags(pSettings);
							batch_flags.ShowDialog();

							for (int i = 0; i < OpenBatchFoldersDialog.FileNames.Count(); i++)
							{
								string source_folder = OpenBatchFoldersDialog.FileNames[i];
								source_folder = source_folder.Substring(0, source_folder.LastIndexOf('\\'));
								batch_source.Add(source_folder);
								string[] files = DirSearch(OpenBatchFoldersDialog.FileNames[i]);
								files = SortFormat(files, "object");
								batch_files.Add(files);
							}

							if (batch_flags.res)
							{
								SdkThread = new Thread(() => {
									int code = StartEditor(true, EditorMode.BatchDialogOMF, TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
									if (!EditorKilled)
									{
										if (code == 0)
											AutoClosingMessageBox.Show($"Batch convert successful. {GetTime()}", "", 1000, MessageBoxIcon.Information);
										else
											AutoClosingMessageBox.Show($"Batch convert completed with errors.{GetRetCode(code)}", "", GetErrorTime(), MessageBoxIcon.Error);
									}
								});
								SdkThread.Start();
							}
						}
					}
					break;
			}
			EditorKilled = false;
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

							(SurfacesPage.Controls[i].Controls[0] as CheckBox).Checked = (surfaces[i].flags == 1);
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

							(SurfacesPage.Controls[i].Controls[0] as CheckBox).Checked = (surfaces[i].flags == 1);
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

							(SurfacesPage.Controls[i].Controls[0] as CheckBox).Checked = (surfaces[i].flags == 1);
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
					uint size;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_SHAPE, false, true);
						if (size == 0) break;
						bones[chunk].shape_type = (ushort)xr_loader.ReadUInt16();
						bones[chunk].shape_flags = (ushort)xr_loader.ReadUInt16();

						(BonesPage.Controls[chunk].Controls[4] as ComboBox).SelectedIndex = bones[chunk].shape_type;

						(BonesPage.Controls[chunk].Controls[0] as CheckBox).Checked = (bones[chunk].shape_flags & (1 << 0)) == (1 << 0);
						(BonesPage.Controls[chunk].Controls[1] as CheckBox).Checked = (bones[chunk].shape_flags & (1 << 1)) == (1 << 1);
						(BonesPage.Controls[chunk].Controls[2] as CheckBox).Checked = (bones[chunk].shape_flags & (1 << 2)) == (1 << 2);
						(BonesPage.Controls[chunk].Controls[3] as CheckBox).Checked = (bones[chunk].shape_flags & (1 << 3)) == (1 << 3);

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MATERIAL, false, true);
						if (size == 0) break;
						BonesPage.Controls[chunk].Controls[7].Text = bones[chunk].material = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MASS, false, true);
						if (size == 0) break;
						bones[chunk].mass = xr_loader.ReadFloat();
						BonesPage.Controls[chunk].Controls[9].Text = bones[chunk].mass.ToString();

						chunk++;
						xr_loader.SetStream(temp);

						if (bones.Count <= chunk) break;
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();

					if (size == bones.Count)
					{
						for (int i = 0; i < size; i++)
						{
							bones[i].shape_flags = 0;
							bones[i].shape_type = 0;
							bones[i].bone_name = xr_loader.read_stringZ();
							bones[i].bone_id = (ushort)i;
							bones[i].material = "default_object";
							bones[i].mass = 10.0f;
							xr_loader.read_stringZ();
							xr_loader.read_stringZ();
							xr_loader.ReadBytes(12);
							xr_loader.ReadBytes(12);
							xr_loader.ReadFloat();

							(BonesPage.Controls[i].Controls[4] as ComboBox).SelectedIndex = bones[i].shape_type;

							(BonesPage.Controls[i].Controls[0] as CheckBox).Checked = (bones[i].shape_flags & (1 << 0)) == (1 << 0);
							(BonesPage.Controls[i].Controls[1] as CheckBox).Checked = (bones[i].shape_flags & (1 << 1)) == (1 << 1);
							(BonesPage.Controls[i].Controls[2] as CheckBox).Checked = (bones[i].shape_flags & (1 << 2)) == (1 << 2);
							(BonesPage.Controls[i].Controls[3] as CheckBox).Checked = (bones[i].shape_flags & (1 << 3)) == (1 << 3);

							BonesPage.Controls[i].Controls[7].Text = bones[i].material = xr_loader.read_stringZ();
							BonesPage.Controls[i].Controls[9].Text = bones[i].mass.ToString();
						}
					}
				}
			}
		}

		private void LoadData()
		{
			var xr_loader = new XRayLoader();
			bool SmoothChanged = false;

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
					SmoothCoP.Checked = true;
					SmoothChanged = true;
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
					SmoothSoC.Checked = true;
					SmoothChanged = true;
				}

				UseSplitNormals.Enabled = false;
				UseSplitNormals.Checked = false;
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

						if (!UseSplitNormals.Enabled && xr_loader.find_chunk((int)MESH.EMESH_CHUNK_NORMALS, false, true))
						{
							UseSplitNormals.Enabled = true;
							UseSplitNormals.Checked = NORMALS_DEFAULT;
							normalsToolStripMenuItem.Enabled = true;
							SmoothChanged = NORMALS_DEFAULT;
						}

						id++;
						xr_loader.SetStream(temp);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				surfaces = new List<Surface>();

				if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES3, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						xr_loader.read_stringZ(); // GameMtl
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES2, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						xr_loader.read_stringZ(); // Shader XRLC
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap
						surface.flags = xr_loader.ReadUInt32();   // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count

						surfaces.Add(surface);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_SURFACES, !FindBody, true))
				{
					surface_count = xr_loader.ReadUInt32();
					for (int i = 0; i < surface_count; i++)
					{
						Surface surface = new Surface();
						surface.name = xr_loader.read_stringZ(); // Name
						surface.shader = xr_loader.read_stringZ(); // Shader
						surface.flags = xr_loader.ReadByte();     // Flags
						xr_loader.ReadUInt32();   // FVF
						xr_loader.ReadUInt32();   // TC count
						surface.texture = xr_loader.read_stringZ(); // Texture
						xr_loader.read_stringZ(); // VMap

						surfaces.Add(surface);
					}
				}

				xr_loader.SetStream(r.BaseStream);
				FindBody = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_OBJECT_BODY, false, true));

				bones = new List<Bone>();
				bool B_CHUNK = xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)OBJECT.EOBJ_CHUNK_BONES2, !FindBody, true));
				if (B_CHUNK)
				{
					int chunk = 0;
					uint size;

					while (true)
					{
						Stream temp = xr_loader.reader.BaseStream;

						if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(chunk, false, true))) break;

						Bone bone = new Bone();
						bone.bone_id = (ushort)chunk;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_DEF, false, true);
						if (size == 0) break;
						bone.bone_name = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_MATERIAL, false, true);
						if (size == 0) break;
						bone.material = xr_loader.read_stringZ();

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_SHAPE, false, true);
						if (size == 0) break;
						bone.shape_type = (ushort)xr_loader.ReadUInt16();
						bone.shape_flags = (ushort)xr_loader.ReadUInt16();

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_MASS, false, true))
						{
							bone.mass = xr_loader.ReadFloat();
							bone.center_of_mass.set(xr_loader.ReadVector());
						}

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_BIND_POSE, false, true);
						if (size == 0) break;
						bone.position.set(xr_loader.ReadVector());
						bone.rotation.set(xr_loader.ReadVector());

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_FLAGS, false, true))
							bone.breakable = xr_loader.ReadUInt32() == 1;

						size = xr_loader.find_chunkSize((int)BONE.BONE_CHUNK_IK_JOINT, false, true);
						if (size == 0) break;
						bone.joint_type = (ushort)xr_loader.ReadUInt32();
						xr_loader.ReadBytes(16*3); // Лимиты, добавлю позже
						bone.spring = xr_loader.ReadFloat();
						bone.damping = xr_loader.ReadFloat();

						if (xr_loader.find_chunk((int)BONE.BONE_CHUNK_IK_JOINT_FRICTION, false, true))
							bone.friction = xr_loader.ReadFloat();

						bones.Add(bone);

						chunk++;
						xr_loader.SetStream(temp);
					}
				}
				else if (xr_loader.find_chunk((int)OBJECT.EOBJ_CHUNK_BONES, !FindBody, true))
				{
					uint size = xr_loader.ReadUInt32();
					for (int i = 0; i < size; i++)
					{
						Bone bone = new Bone();
						bone.shape_flags = 0;
						bone.shape_type = 0;
						bone.bone_name = xr_loader.read_stringZ();
						bone.bone_id = (ushort)i;
						bone.material = "default_object";
						bone.mass = 10.0f;
						xr_loader.read_stringZ();
						xr_loader.read_stringZ();
						xr_loader.ReadBytes(12);
						xr_loader.ReadBytes(12);
						xr_loader.ReadFloat();

						bones.Add(bone);
					}
				}

				joints_count = (uint)bones.Count;

				if (!SmoothChanged)
                {
					pSettings.Load(SmoothSoC);
					pSettings.Load(SmoothCoP, true);
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
				ScaleGroupBox.Enabled = has_bones && hasmot;

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

		private string[] GameMtlParser(string filename)
        {
			List<string> materials = new List<string>();

			var xr_loader = new XRayLoader();

			using (var r = new BinaryReader(new FileStream(filename, FileMode.Open)))
			{
				xr_loader.SetStream(r.BaseStream);
				xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk((int)MTL.GAMEMTLS_CHUNK_MTLS, false, true));

				int id = 0;
				uint size;

				while (true)
				{
					if (!xr_loader.find_chunk(id)) break;

					Stream temp = xr_loader.reader.BaseStream;

					if (!xr_loader.SetData(xr_loader.find_and_return_chunk_in_chunk(id, false, true))) break;

					size = xr_loader.find_chunkSize((int)MTL.GAMEMTL_CHUNK_MAIN);
					if (size == 0) break;
					xr_loader.ReadBytes(4);
					materials.Add(xr_loader.read_stringZ());

					id++;
					xr_loader.SetStream(temp);
				}
			}
			string[] ret = materials.ToArray();
			Array.Sort(ret);
			return ret;
        }

		public void ReloadGameMtl(string filename)
		{
			 game_materials = GameMtlParser(filename);
			 
			 if (bones != null)
			 {
			 	 BonesPage.Controls.Clear();
			 	 for (int i = 0; i < bones.Count; i++)
			 		 CreateShapeGroupBox(i, bones[i]);
			 }
		}

		private void objectInfoToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show($"Vertex count: {vertex_count}\nFace count: {face_count}\nSurface count: {surface_count}\nJoints count: {joints_count}", "Info", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void IndexChanged(object sender, EventArgs e)
		{
			if (TabControl.SelectedIndex < 0 || !FlagsGroupBox.Enabled) return;

			switch (TabControl.Controls[TabControl.SelectedIndex].Name)
			{
				case "BonesPage":
					if (!USE_OLD_BONES)
						this.Size = BoneSize;
					break;
				case "SurfacesPage":
					if (!USE_OLD_BONES)
						this.Size = CurrentSize;
					break;
				default:
					if (!USE_OLD_BONES)
						this.Size = CurrentSize;
					break;
			}
		}

		private void SwitchShapeType(object sender, EventArgs e)
		{
			ToolStripItem Item = sender as ToolStripItem;
			int type = Convert.ToInt32(Item.Tag.ToString().Split('_')[1]);

			int shapes_count = (bones != null ? bones.Count : 0);
			for (int i = 0; i < shapes_count; i++)
			{
				bones[i].shape_type = (ushort)type;
				(BonesPage.Controls[i].Controls[4] as ComboBox).SelectedIndex = type;
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
			if (TEMP_FILE_NAME != "")
			{
				StartEditor(true, EditorMode.SaveObject, TEMP_FILE_NAME);
				if (!EditorKilled)
				{
					File.Copy(TEMP_FILE_NAME, filename, true);
					AutoClosingMessageBox.Show($"Object successfully saved.{GetTime()}", "", 1000, MessageBoxIcon.Information);
				}
			}
		}

        private void ClosingForm(object sender, FormClosingEventArgs e)
        {
			if (Directory.Exists(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp"))
				Directory.Delete(Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp", true);

			if (EditorWorking)
			{
				EditorKilled = true;
				EditorProcess.Kill();
			}
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

		private void FloatTextChanged(object sender, EventArgs e, ref float val)
		{
			if (bKeyIsDown)
			{
				TextBox curBox = sender as TextBox;

				if (curBox.Text.Length == 0)
					return;

				int len = curBox.Text.Contains(".") ? 9 : 8;

				while (curBox.Text.Length >= len)
				{
					if (curBox.SelectionStart < 1)
						curBox.SelectionStart = curBox.Text.Length;

					int tmp = curBox.SelectionStart;
					curBox.Text = curBox.Text.Remove(curBox.Text.Length - 1, 1);
					curBox.SelectionStart = tmp;
				}

				string number_mask = @"^-[0-9.]*$";
				int temp = curBox.SelectionStart;
				string mask = number_mask;
				Regex.Match(curBox.Text, mask);

				try
				{
					Convert.ToSingle(curBox.Text);
					val = Convert.ToSingle(curBox.Text);
				}
				catch (Exception)
				{
					curBox.Text = val.ToString();

					if (curBox.SelectionStart < 1)
						curBox.SelectionStart = curBox.Text.Length;

					curBox.SelectionStart = temp - 1;
				}
				bKeyIsDown = false;
			}
		}

		private void ScaleTextChanged(object sender, EventArgs e)
		{
			FloatTextChanged(sender, e, ref model_scale);
		}

		private void MassTextChanged(object sender, EventArgs e)
		{
			TextBox curBox = sender as TextBox;
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);
			FloatTextChanged(sender, e, ref bones[idx].mass);
		}

		private void MaterialTextChanged(object sender, EventArgs e)
		{
			Control curBox = sender as Control;
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);
			bones[idx].material = GetCorrectString(curBox.Text);
		}

		private void TextBoxKeyDown(object sender, KeyEventArgs e)
		{
			bKeyIsDown = true;
		}

		private void TextBoxKeyPress(object sender, KeyPressEventArgs e)
		{
			TextBox textBox = sender as TextBox;
			if (!(Char.IsDigit(e.KeyChar)) && !((e.KeyChar == '.') && (textBox.Text.IndexOf(".") == -1) &&(textBox.Text.Length != 0)))
			{
				if (e.KeyChar != (char)Keys.Back) e.Handled = true;
			}
		}

		private void openLogToolStripMenuItem_Click(object sender, EventArgs e)
        {
			string log = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\engine.log";
			if (File.Exists(log))
				System.Diagnostics.Process.Start("notepad.exe", log);
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
				surfaces[i].flags = 1;
				(SurfacesPage.Controls[i].Controls[0] as CheckBox).Checked = true;
			}
		}

		private void disableAll2SidedToolStripMenuItem_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < surfaces.Count; i++)
			{
				surfaces[i].flags = 0;
				(SurfacesPage.Controls[i].Controls[0] as CheckBox).Checked = true;
			}
		}

		private string GetRetCode(int code)
		{
			string ret = "";
			if (debugToolStripMenuItem.Visible)
				ret += "\nExit code: " + code.ToString();
			return ret;
		}

		private string GetTime()
		{
			int minutes = 0;
			int hours = 0;

			while (dLastTime > 60.0)
            {
				minutes++;
				dLastTime -= 60.0;
			}

			while (minutes > 60)
			{
				hours++;
				minutes -= 60;
			}

			if (minutes == 0)
				return $"Time: {dLastTime} sec.";
			else if (hours == 0)
				return $"Time: {minutes} min {dLastTime} sec.";
			else
				return $"Time: {hours} hour {minutes} min {dLastTime} sec.";
		}

		private int GetErrorTime()
		{
			if (debugToolStripMenuItem.Visible)
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

		private string[] SortFormat(string[] source, string format)
		{
			List<string> files = new List<string>();

			foreach (string s in source)
            {
				if (Path.GetExtension(s) == "." + format)
					files.Add(s);
			}

			return files.ToArray();
		}

		private void TextBoxFilter(object sender, EventArgs e)
		{
			TextBox curBox = sender as TextBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			switch (currentField)
			{
				case "TextureTextBox":
					surfaces[idx].texture = curBox.Text;
					break;
				case "ShaderTextBox":
					surfaces[idx].shader = curBox.Text;
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

			switch (currentField)
			{
				case "chbx2sided":
					surfaces[idx].flags = (uint)(curBox.Checked ? 1 : 0);
					break;
				case "chbxNoPickable":
					bones[idx].shape_flags = (ushort)BitSet(bones[idx].shape_flags, (1 << 0), curBox.Checked);
					break;
				case "chbxNoPhysics":
					bones[idx].shape_flags = (ushort)BitSet(bones[idx].shape_flags, (1 << 1), curBox.Checked);
					break;
				case "chbxRemoveAfterBreakCheckBoxBox":
					bones[idx].shape_flags = (ushort)BitSet(bones[idx].shape_flags, (1 << 2), curBox.Checked);
					break;
				case "chbxNoFogColliderCheckBox":
					bones[idx].shape_flags = (ushort)BitSet(bones[idx].shape_flags, (1 << 3), curBox.Checked);
					break;
			}
		}

		private void ComboBoxIndexChanged(object sender, EventArgs e)
		{
			ComboBox curBox = sender as ComboBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			switch (currentField)
			{
				case "cbxType":
					bones[idx].shape_type = (ushort)curBox.SelectedIndex;
					break;
			}
		}

		private void TextBoxTextChanged(object sender, EventArgs e)
		{
			ComboBox curBox = sender as ComboBox;

			string currentField = curBox.Name.ToString().Split('_')[0];
			int idx = Convert.ToInt32(curBox.Name.ToString().Split('_')[1]);

			switch (currentField)
			{
				case "cbxType":
					bones[idx].shape_type = (ushort)curBox.SelectedIndex;
					break;
			}
		}

		private void EditorKeyDown(object sender, KeyEventArgs e)
		{
			if (e.Control && e.KeyCode == Keys.Delete && EditorWorking)
			{
				try
				{
					EditorKilled = true;
					EditorProcess.Kill();
					EditorWorking = false;
					AutoClosingMessageBox.Show("Process Closed!", "", 1000, MessageBoxIcon.Information);
				}
				catch (Exception)
                {
					AutoClosingMessageBox.Show("Process not valid!", "", 1000, MessageBoxIcon.Information);
				}
			}

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
				case Keys.F5:
					if (!CheckThread()) return;

					SdkThread = new Thread(() => { FastSaveObject(FILE_NAME); });
					SdkThread.Start();
					break;
				case Keys.F6:
					fileToolStripMenuItem.ShowDropDown();
					saveToolStripMenuItem.ShowDropDown();
					break;
			}
		}

		private void saveToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			if (!CheckThread()) return;

			SdkThread = new Thread(() => { FastSaveObject(FILE_NAME); });
			SdkThread.Start();
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

		private void SplitNormalsChbx_CheckedChanged(object sender, EventArgs e)
		{
			CheckBox chbx = sender as CheckBox;
			SmoothTypeGroupBox.Enabled = !chbx.Checked;
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

		private void InitBoneUI()
		{
			if (bones != null)
			{
				if (USE_OLD_BONES)
				{
					for (int i = 0; i < bones.Count; i++)
						CreateShapeGroupBox(i, bones[i]);
				}
				else
				{
					for (int i = 0; i < bones.Count; i++)
						BonesList.Items.Add(bones[i].bone_name);
					BoneMaterial.Items.AddRange(game_materials);
					if (BonesList.Items.Count > 0)
						BonesList.SelectedIndex = 0;
				}
			}
		}

		private void InitSurfaceUI()
		{
			if (surfaces != null)
			{
				for (int i = 0; i < surfaces.Count; i++)
					CreateMaterialGroupBox(i);
			}
		}

		private void InitUI()
        {
			CurrentSize = this.Size;
			BoneSize	= new Size(713, 790);

			shapeParamsToolStripMenuItem.Enabled		= false;
			surfaceParamsToolStripMenuItem.Enabled		= false;
			saveToolStripMenuItem.Enabled				= false;
			exportToolStripMenuItem.Enabled				= false;
			deleteToolStripMenuItem.Enabled				= false;
			sklSklsToolStripMenuItem.Enabled			= false;
			bonesToolStripMenuItem.Enabled				= false;
			dMToolStripMenuItem.Enabled					= false;
			bonesPartsToolStripMenuItem.Enabled			= false;
			FlagsGroupBox.Enabled						= false;
			MotionRefsBox.Enabled						= false;
			UserDataTextBox.Enabled						= false;
			LodTextBox.Enabled							= false;
			ScaleGroupBox.Enabled						= false;
			motionRefsToolStripMenuItem.Enabled			= false;
			userDataToolStripMenuItem.Enabled			= false;
			motionRefsToolStripMenuItem1.Enabled		= false;
			userDataToolStripMenuItem1.Enabled			= false;
			generateLodToolStripMenuItem.Enabled		= false;
			objectInfoToolStripMenuItem.Enabled			= false;
			UseSplitNormals.Enabled						= false;
			normalsToolStripMenuItem.Enabled			= false;
			importObjectParamsToolStripMenuItem.Enabled = false;
			saveToolStripMenuItem1.Enabled				= false;
			generateShapesToolStripMenuItem.Enabled		= false;
			ViewtoolStripMenuItem.Enabled				= false;

			if (USE_OLD_BONES)
				BonesPage.Controls.Clear();
		}

		private void UpdateUI()
        {
			bool has_motions = MotionCount() > 0;
			bool has_bones = HasBones();

			DeletesklsToolStripMenuItem.Enabled				= has_motions;
			SaveSklsToolStripMenuItem.Enabled				= has_motions;
			sklToolStripMenuItem.Enabled					= has_motions;
			oMFToolStripMenuItem.Enabled					= has_motions;
			saveToolStripMenuItem.Enabled					= !IsOgfMode;
			exportToolStripMenuItem.Enabled					= true;
			deleteToolStripMenuItem.Enabled					= true;
			sklSklsToolStripMenuItem.Enabled				= has_bones;
			bonesToolStripMenuItem.Enabled					= has_bones;
			oGFToolStripMenuItem.Enabled					= !IsOgfMode;
			objToolStripMenuItem.Enabled					= !IsOgfMode;
			objectToolStripMenuItem.Enabled					= !IsOgfMode;
			dMToolStripMenuItem.Enabled						= !IsOgfMode;
			bonesPartsToolStripMenuItem.Enabled				= has_bones;
			cToolStripMenuItem.Enabled						= !IsOgfMode;
			bonesToolStripMenuItem1.Enabled					= has_bones;
			bonesPartsToolStripMenuItem1.Enabled			= has_bones;
			bonesPartsToDefaultToolStripMenuItem.Enabled	= has_bones;
			ScaleGroupBox.Enabled							= !IsOgfMode;
			MotionRefsBox.Enabled							= has_bones;
			UserDataTextBox.Enabled							= has_bones;
			LodTextBox.Enabled								= has_bones;
			motionRefsToolStripMenuItem.Enabled				= has_bones && !IsOgfMode;
			userDataToolStripMenuItem.Enabled				= has_bones && !IsOgfMode;
			ModelFlagsGroupBox.Enabled						= !IsOgfMode;
			FlagsGroupBox.Enabled							= true;
			generateLodToolStripMenuItem.Enabled			= !IsOgfMode;
			objectInfoToolStripMenuItem.Enabled				= !IsOgfMode;
			importObjectParamsToolStripMenuItem.Enabled		= !IsOgfMode;
			saveToolStripMenuItem1.Enabled					= !IsOgfMode;
			shapeParamsToolStripMenuItem.Enabled			= !IsOgfMode && has_bones;
			generateShapesToolStripMenuItem.Enabled			= !IsOgfMode && has_bones;
			surfaceParamsToolStripMenuItem.Enabled			= !IsOgfMode;
			ViewtoolStripMenuItem.Enabled					= true;

			vertex_count	= 0;
			face_count		= 0;
			surface_count	= 0;
			joints_count	= 0;

			SurfacesPage.Controls.Clear();
			MotionRefsBox.Clear();
			UserDataTextBox.Clear();
			LodTextBox.Clear();
			if (USE_OLD_BONES)
				BonesPage.Controls.Clear();

			if (IsOgfMode)
			{
				TabControl.Controls.Clear();
				TabControl.Controls.Add(FlagsPage);
				TabControl.Controls.Add(MotionPage);
			}
		}

		private void CreateShapeGroupBox(int idx, Bone bone)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 106 * idx);
			GroupBox.Size = new System.Drawing.Size(362, 100);
			GroupBox.Text = "Bone name: " + bone.bone_name;
			GroupBox.Name = "ShapeGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			GroupBox.Dock = DockStyle.Top;
			CreateShapeFlags(idx, GroupBox, bone);
			BonesPage.Controls.Add(GroupBox);
		}

		private void CreateShapeFlags(int idx, GroupBox box, Bone bone)
		{
			var NoPickableCheckBoxBox = new CheckBox();
			NoPickableCheckBoxBox.Name = "chbxNoPickable_" + idx;
			NoPickableCheckBoxBox.Text = "No Pickable";
			NoPickableCheckBoxBox.Size = new System.Drawing.Size(130, 23);
			NoPickableCheckBoxBox.Location = new System.Drawing.Point(6, 15);
			NoPickableCheckBoxBox.Anchor = AnchorStyles.Left;
			NoPickableCheckBoxBox.Checked = (bone.shape_flags & (1 << 0)) == (1 << 0);
			NoPickableCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var NoPhysicsCheckBoxBox = new CheckBox();
			NoPhysicsCheckBoxBox.Name = "chbxNoPhysics_" + idx;
			NoPhysicsCheckBoxBox.Text = "No Physics";
			NoPhysicsCheckBoxBox.Size = new System.Drawing.Size(100, 23);
			NoPhysicsCheckBoxBox.Location = new System.Drawing.Point(6, 35);
			NoPhysicsCheckBoxBox.Anchor = AnchorStyles.Left;
			NoPhysicsCheckBoxBox.Checked = (bone.shape_flags & (1 << 1)) == (1 << 1);
			NoPhysicsCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var RemoveAfterBreakCheckBoxBox = new CheckBox();
			RemoveAfterBreakCheckBoxBox.Name = "chbxRemoveAfterBreakCheckBoxBox_" + idx;
			RemoveAfterBreakCheckBoxBox.Text = "Remove After Break";
			RemoveAfterBreakCheckBoxBox.Size = new System.Drawing.Size(132, 23);
			RemoveAfterBreakCheckBoxBox.Location = new System.Drawing.Point(6, 55);
			RemoveAfterBreakCheckBoxBox.Anchor = AnchorStyles.Left;
			RemoveAfterBreakCheckBoxBox.Checked = (bone.shape_flags & (1 << 2)) == (1 << 2);
			RemoveAfterBreakCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var NoFogColliderCheckBoxBox = new CheckBox();
			NoFogColliderCheckBoxBox.Name = "chbxNoFogColliderCheckBox_" + idx;
			NoFogColliderCheckBoxBox.Text = "No Fog Collider";
			NoFogColliderCheckBoxBox.Size = new System.Drawing.Size(120, 23);
			NoFogColliderCheckBoxBox.Location = new System.Drawing.Point(6, 75);
			NoFogColliderCheckBoxBox.Anchor = AnchorStyles.Left;
			NoFogColliderCheckBoxBox.Checked = (bone.shape_flags & (1 << 3)) == (1 << 3);
			NoFogColliderCheckBoxBox.CheckedChanged += new System.EventHandler(this.CheckBoxCheckedChanged);

			var TypeLabel = new Label();
			TypeLabel.Name = "TypeLbl_" + idx;
			TypeLabel.Text = "Shape type:";
			TypeLabel.Size = new System.Drawing.Size(70, 17);
			TypeLabel.Location = new System.Drawing.Point(135, 18);
			TypeLabel.Anchor = AnchorStyles.Left;

			var TypeComboBox = new ComboBox();
			TypeComboBox.Name = "cbxType_" + idx;
			TypeComboBox.Size = new System.Drawing.Size(155, 23);
			TypeComboBox.Location = new System.Drawing.Point(200, 15);
			TypeComboBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			TypeComboBox.Items.Add("None");
			TypeComboBox.Items.Add("Box");
			TypeComboBox.Items.Add("Sphere");
			TypeComboBox.Items.Add("Cylinder");
			TypeComboBox.SelectedIndex = bone.shape_type;
			TypeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
			TypeComboBox.SelectedIndexChanged += new System.EventHandler(this.ComboBoxIndexChanged);

			var MaterialLabel = new Label();
			MaterialLabel.Name = "MaterialLbl_" + idx;
			MaterialLabel.Text = "Material:";
			MaterialLabel.Size = new System.Drawing.Size(50, 17);
			MaterialLabel.Location = new System.Drawing.Point(135, 45);
			MaterialLabel.Anchor = AnchorStyles.Left;

			var MateriaBox = new Control();
			if (game_materials.Count() == 0)
			{
				var MateriaTextBox = new TextBox();
				MateriaTextBox.Name = "MaterialTextBox_" + idx;
				MateriaTextBox.Text = bone.material;
				MateriaTextBox.Size = new System.Drawing.Size(155, 17);
				MateriaTextBox.Location = new System.Drawing.Point(200, 43);
				MateriaTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
				MateriaTextBox.TextChanged += new EventHandler(this.MaterialTextChanged);

				MateriaBox = MateriaTextBox;
			}
			else
            {
				var MateriaTextBox = new ComboBox();
				MateriaTextBox.Name = "MaterialTextBox_" + idx;
				MateriaTextBox.Text = bone.material;
				MateriaTextBox.Size = new System.Drawing.Size(155, 17);
				MateriaTextBox.Location = new System.Drawing.Point(200, 43);
				MateriaTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
				MateriaTextBox.Items.AddRange(game_materials);
				MateriaTextBox.DropDownStyle = ComboBoxStyle.DropDownList;
				MateriaTextBox.SelectedIndexChanged += new System.EventHandler(this.MaterialTextChanged);
				if (MateriaTextBox.Items.Contains(bone.material))
					MateriaTextBox.SelectedIndex = MateriaTextBox.Items.IndexOf(bone.material);
				else
					MateriaTextBox.Text = bone.material;

				MateriaBox = MateriaTextBox;
			}

			var MassLabel = new Label();
			MassLabel.Name = "MassLbl_" + idx;
			MassLabel.Text = "Mass:";
			MassLabel.Size = new System.Drawing.Size(50, 17);
			MassLabel.Location = new System.Drawing.Point(135, 72);
			MassLabel.Anchor = AnchorStyles.Left;

			var MassTextBox = new TextBox();
			MassTextBox.Name = "MassTextBox_" + idx;
			MassTextBox.Text = bone.mass.ToString();
			MassTextBox.Size = new System.Drawing.Size(155, 17);
			MassTextBox.Location = new System.Drawing.Point(200, 70);
			MassTextBox.Anchor = AnchorStyles.Left | AnchorStyles.Right;
			MassTextBox.KeyDown += new KeyEventHandler(this.TextBoxKeyDown);
			MassTextBox.KeyPress += new KeyPressEventHandler(this.TextBoxKeyPress);
			MassTextBox.TextChanged += new EventHandler(this.MassTextChanged);

			box.Controls.Add(NoPickableCheckBoxBox);
			box.Controls.Add(NoPhysicsCheckBoxBox);
			box.Controls.Add(RemoveAfterBreakCheckBoxBox);
			box.Controls.Add(NoFogColliderCheckBoxBox);
			box.Controls.Add(TypeComboBox);
			box.Controls.Add(TypeLabel);
			box.Controls.Add(MaterialLabel);
			box.Controls.Add(MateriaBox);
			box.Controls.Add(MassLabel);
			box.Controls.Add(MassTextBox);
		}

		private void CreateMaterialGroupBox(int idx)
		{
			var GroupBox = new GroupBox();
			GroupBox.Location = new System.Drawing.Point(3, 3 + 114 * idx);
			GroupBox.Size = new System.Drawing.Size(362, 108);
			GroupBox.Text = surfaces[idx].name;
			GroupBox.Name = "MaterialGrpBox_" + idx;
			GroupBox.Anchor = AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
			GroupBox.Dock = DockStyle.Top;
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
			TextureLabel.Anchor = AnchorStyles.Left;

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
				if (format.Length == 3) // Хз нужно ли или нет, но у основных форматов длинна названия в 3 символа, пусть будет проверкой
					texture_path = surfaces[idx].texture.Substring(0, surfaces[idx].texture.LastIndexOf('.'));
			}
			TextureTextBox.Text = texture_path;

			var ShaderLabel = new Label();
			ShaderLabel.Text = "Shader:";
			ShaderLabel.Name = "ShaderLabel_" + idx;
			ShaderLabel.Size = new System.Drawing.Size(46, 23);
			ShaderLabel.Location = new System.Drawing.Point(5, 60);
			ShaderLabel.Anchor = AnchorStyles.Left;

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

		public void Msg(string text)
		{
			MessageBox.Show(text);
		}

		private void PageResize(object sender, EventArgs e)
		{
			switch (TabControl.Controls[TabControl.SelectedIndex].Name)
			{
				case "BonesPage":
					BoneSize = this.Size;
					break;
				default:
					CurrentSize = this.Size;
					break;
			}
		}

		private void settingsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Settings ProgramSettings = new Settings(this.pSettings, this, this);
			ProgramSettings.ShowDialog();

			string game_mtl = "";
			pSettings.Load("GameMtlPath", ref game_mtl);
			pSettings.LoadState("SplitNormalsChbx", ref NORMALS_DEFAULT, true);
			if (File.Exists(game_mtl))
				game_materials = GameMtlParser(game_mtl);

			bool Debug = false;
			pSettings.LoadState("Debug", ref Debug);
			debugToolStripMenuItem.Visible = Debug;
		}

		public void SyncCompressUI(bool visible)
        {
			if (AnimsNoCompress.Visible && !visible) // disable
			{
				AnimsNoCompress.Visible = false;
				BuildInMotionsExport.Location = AnimsNoCompress.Location;
				MotionFlagsGroupBox.Size = new Size(MotionFlagsGroupBox.Size.Width, MotionFlagsGroupBox.Size.Height - 22);
				ScaleGroupBox.Location = new Point(ScaleGroupBox.Location.X, ScaleGroupBox.Location.Y - 22);
				ScaleGroupBox.Size = new Size(ScaleGroupBox.Size.Width, ScaleGroupBox.Size.Height + 22);
			}
			else if (!AnimsNoCompress.Visible && visible) // enable
			{
				AnimsNoCompress.Visible = true;
				BuildInMotionsExport.Location = new Point(6, 88);
				MotionFlagsGroupBox.Size = new Size(MotionFlagsGroupBox.Size.Width, MotionFlagsGroupBox.Size.Height + 22);
				ScaleGroupBox.Location = new Point(ScaleGroupBox.Location.X, ScaleGroupBox.Location.Y + 22);
				ScaleGroupBox.Size = new Size(ScaleGroupBox.Size.Width, ScaleGroupBox.Size.Height - 22);
			}
		}

        private void modelExportToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"1. Make progressive meshes - создает прогрессивные меши при экспорте OGF. Это динамическая детализация модели (lod'ы), используется для оптимизации мировых объектов.\n" +
			"2. Make stripify meshes - оптимизация vertex'ов и face'ов у мешей которая портила сетку полигонов, раньше стояла по дефолту в SDK и использовалась для оптимизации мешей под старый DirectX и видеокарты. Можно включать для оптимизации мировых моделей.\n" +
			"3. Optimize surfaces - объединяет меши с одинаковыми текстурами и шейдерами в один.\n" +
			"4. HQ Geometry+ - компилятор не будет удалять похожие vertex'ы и face'ы, поддержка более плотной сетки полигонов.\n" +
			"5. SoC bone export - при экспорте динамического OGF, на полигон будут влиять максимум 2 кости. При отключении будет включено CoP влияние в 4 кости (не поддерживается в SoC).\n" +
			"6. Smooth Type определяет тип сглаживания при экспорте моделей.\n1) SoC: #1\n2) CS\\CoP: #2\n3) Normals: использует оригинальные Split нормали, новый формат."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void motionExportToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"1. 8 bit - SoC Формат, плохое качество.\n2. 16 bit - CoP Формат, хорошее качество.\n" + (AnimsNoCompress.Visible ? "3. No compress - Новый формат без сжатия, лучшее качество.\n" : "") +
			(AnimsNoCompress.Visible ? "4." : "3.") + " Use build-in motions - При активации программа будет использовать загруженные анимации вместо моушн референсов, если анимаций нет то будут использованы референсы, если таковы есть. При деактивации загруженные анимации будут игнорированы. Влияет на всё кроме сохранения object."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void scaleToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"1. Object scale - изменяет размер экспортированных OGF, OMF и Obj.\n" +
			"2. Scale center of mass - при активации центр массы кости будет пересчитан под новый размер."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void bonesToolStripMenuItem2_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"1. No Pickable - Ray Query лучи, хит wallmark'и будут пропускать данный элемент.\n" +
			"2. Remove After Break - при активации у всех костей, после спавна объекта начнется таймер \"remove_time\" из конфига, при истечении которого объект удалится.\n" +
			"3. No Physics - движок игнорирует физику шейпа.\n" +
			"4. No Fog Collider - Volumetric fog будет игнорировать данный элемент.\n" +
			"5. Material - определяет материал кости при ударах\\столкновениях итд. который влияет на звук и партиклы.\n" +
			"6. Mass - масса кости."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void surfacesToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"1. Surface->2 Sided - после экспорта OGF, Obj модель будет отрисовываться с наружной и внутренней стороны, в 2 раза увеличивает колличество полигонов у модели."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void otherToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"Для создания lod модели нужно нажать Tools->Generate lod, появится окно с настройкой детализации лода, после нажатия кнопки Append сгенерируется lod модель. Если она была сохранена в геймдату игры, то референс лода автоматически пропишестя в текщую модель, иначе его нужно будет прописывать вручную."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void hotkeysToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"F3 - Экспорт\nF4 - Загрузка\nF5, Ctrl+S - Быстрое сохранение .object\nF6 - Сохранение\nCtrl+Del - Закрытие текущего процесса"
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void ltxToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show("Batch Converter создан для массового экспорта моделей и анимаций.\nВ данной программе имеется 3 режима: From Ltx, From File Dialog и From Folder Dialog.\n\n" +
			"From Ltx - Пример ltx конфига:\n\n" +
			"[ogf] ; секция из которой будут экспортироватсья ogf модели\n" +
			"test.object = test.ogf ; test.object из папки где находится ltx будет экспортирован в test.ogf\n" +
			"test2 = test3 ; можно указывать без форматов файлов, все равно будет работать\n" +
			"test\\test3 = test\\test3 ; так же можно прописывать папки\n\n" +
			"[omf] ; секция из которой будут экспортироваться omf анимации\n" +
			"test.object = test.omf ; встроенные анимации из test.object будут экспортированны в test.omf\n" +
			"test\\test = test\\test ; все так же можно прописывать без указания формата и в папках\n\n" +
			"[skls_skl] ; новая секция которая есть только в Object Editor, подгружает анимации в модели перед экспортом\n" +
			"test.object = test1.skl, test\\test2.skls, test3.skl ; все анимации из списка будут загружены в test.object перед экспортом в ogf и omf\n" +
			"test = test1, test\\test2, test3 ; указание без форматов и расположение в папках так же работает, программа будет искать анимации в skls и skl формате"
		, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void dialogsToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show("Batch Converter создан для массового экспорта моделей и анимаций.\nВ данной программе имеется 3 режима: From Ltx, From File Dialog и From Folder Dialog.\n\n" +
			"From Dialog - принцип работы:\n" +
			"Object будет экспортирован в выбранный вами формат.\nПри экспорте программа будет искать skls анимации лежащие рядом с ним с таким же названием что и у object, и при нахождении загрузит их в модель перед экспортом.\n\n" +
			"При выборе From File Dialog откроется окно выбора файлов, после выбора необходимых моделей откроется окно выбора папки куда будут сохранены все выбранные модели.\n" +
			"При выборе From Folder Dialog откроется окно выбора папок, после выбора папок с моделями откроется окно выбора папки куда будут сохранены все выбранные папки с моделями внутри."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void settingsToolStripMenuItem1_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"В меню настроек можно установить дефолтные значения параметров при первом запуске программы, а так же активировать дополнительные функции.\n\n" +
            "Presets - выставит вам параметры для комфортной работы с выбранной игрой из трилогии.\n" +
            "Use split normals - будет выставлять Smooth Type на Normals при наличии этих нормалей. При деактивации активировать нормали нужно будет вручную.\n\n" +
            "Настройки программы:\n" +
			"1. Use No Compress motions - активирует новый параметр компрессии анимаций, выбрав который анимации будут экспортированы без сжатия. Требует наличие коммита из STCoP WP\n" +
			"2. Program debugging - активирует вкладку с кнопками для отладки.\n" +
			"3. Game Mtl path - при выборе gamemtl.xr во вкладке Bones можно будет выбрать и применить материал из gamemtl.\n\n" +
            "Остальные параметры вы могли встречать в первых 3х пунктах в Help."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

        private void shapesGenerateToolStripMenuItem_Click(object sender, EventArgs e)
        {
			MessageBox.Show(
			"Для создания коллизии с нуля нужно настроить Shape type параметры у каждой кости если таковы были не настроены (можно воспользоваться Tools->Shape Params) и далее нажать Tools->Generate Shapes.\nЕсли коллизия уже была сгенерирована, то Shape type можно менять без повторной генерации коллизии."
			, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void viewToolStripMenuItem1_Click(object sender, EventArgs e)
		{
			MessageBox.Show(
				"Примечания:\n1. Model Viewer будет отображать выбранное вами сглаживание во вкладке Flags, благодаря этому можно будет узнать какое сглаживание нужно использовать.\nHQ Geometry и HQ Geometry+ тоже влияют на модель при просмотре.\n2. Model Viewer реагирует на хоткеи только английской раскладки.\n3. Для удобного использования программы нужно ознакомиться с возможностями через клавишу F1.\n4. Если указать путь к текстурам в настройках программы (или указать путь к fs.ltx), то Model Viewer будет отображать текстуры."
				, "Help", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void ViewtoolStripMenuItem_Click(object sender, EventArgs e)
		{
			string exe_path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\OBJ Viewer.exe";
			if (File.Exists(exe_path))
			{
				string Textures = "";
				pSettings.LoadText("TexturesPath", ref Textures);

				List<string> pTextures = new List<string>();

				int surfaces_count = (surfaces != null ? surfaces.Count : 0);
				if (surfaces_count > 0 && Textures != "")
				{
					for (int i = 0; i < surfaces.Count; i++)
					{
						string texture_main = Textures + surfaces[i].texture + ".dds";
						string texture_temp = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\')) + "\\temp\\" + Path.GetFileName(surfaces[i].texture + ".tga");

						if (File.Exists(texture_main)) // Create tga
						{
							pTextures.Add(texture_main);
							pTextures.Add(texture_temp);
						}
					}
				}

				string ObjName = Path.ChangeExtension(TEMP_FILE_NAME, ".obj");
				string MtlName = TEMP_FILE_NAME.Substring(0, TEMP_FILE_NAME.LastIndexOf('\\')) + "\\" + Path.ChangeExtension(GetCorrectString(Path.GetFileName(TEMP_FILE_NAME)), ".mtl");

				StartEditor(false, EditorMode.ExportOBJOptimized, TEMP_FILE_NAME, ObjName, -1, 1.0f, pTextures.ToArray());

				bool ModelExist = File.Exists(ObjName);

				System.Diagnostics.Process Viewer = new System.Diagnostics.Process();
				if (ModelExist)
				{
					Viewer.StartInfo.FileName = exe_path;
					Viewer.StartInfo.UseShellExecute = false;
					Viewer.StartInfo.Arguments = $"\"{ObjName}\"";
					Viewer.Start();
					Viewer.WaitForExit();
				}
				else
					AutoClosingMessageBox.Show("Failed to compile model.", "", GetErrorTime(), MessageBoxIcon.Error);

				if (File.Exists(ObjName))
					File.Delete(ObjName);
				if (File.Exists(MtlName))
					File.Delete(MtlName);

				string[] _files = Directory.GetFiles(TEMP_FILE_NAME.Substring(0, TEMP_FILE_NAME.LastIndexOf('\\')), "*.tga");
				foreach (string fl in _files)
					File.Delete(fl);

				Viewer.Close();
			}
			else
				MessageBox.Show("Can't find OBJ Viewer.exe", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		private void BoneListIndexChanged(object sender, EventArgs e)
        {
			if (BonesList.SelectedIndex == -1) return;

			Bone cur_bone = bones[BonesList.SelectedIndex];

			NoPickable.Checked = (cur_bone.shape_flags & (1 << 0)) == (1 << 0);
			NoPhysics.Checked = (cur_bone.shape_flags & (1 << 1)) == (1 << 1);
			RemoveAfterBreak.Checked = (cur_bone.shape_flags & (1 << 2)) == (1 << 2);
			NoFogCollider.Checked = (cur_bone.shape_flags & (1 << 3)) == (1 << 3);
			ShapeType.SelectedIndex = cur_bone.shape_type;
			BoneName.Text = cur_bone.bone_name;
			BoneMaterial.Text = cur_bone.material;
			BoneMass.Text = ((decimal)cur_bone.mass).ToString();
			Breakable.Checked = cur_bone.breakable;
			JointType.SelectedIndex = cur_bone.joint_type;
			JointFriction.Text = (cur_bone.friction).ToString();
			JointSpring.Text = ((decimal)cur_bone.spring).ToString();
			JointDamping.Text = ((decimal)cur_bone.damping).ToString();
			PosX.Text = ((decimal)cur_bone.position.x).ToString();
			PosY.Text = ((decimal)cur_bone.position.y).ToString();
			PosZ.Text = ((decimal)cur_bone.position.z).ToString();
			RotX.Text = ((decimal)cur_bone.rotation.x).ToString();
			RotY.Text = ((decimal)cur_bone.rotation.y).ToString();
			RotZ.Text = ((decimal)cur_bone.rotation.z).ToString();
			MassX.Text = ((decimal)cur_bone.center_of_mass.x).ToString();
			MassY.Text = ((decimal)cur_bone.center_of_mass.y).ToString();
			MassZ.Text = ((decimal)cur_bone.center_of_mass.z).ToString();
		}

        private void JointType_SelectedIndexChanged(object sender, EventArgs e)
        {
			if (JointType.SelectedIndex == -1) return;

			switch (JointType.SelectedIndex)
            {
				case 0: // Rigid
					LimitsBox.Enabled = false;
					AxisX.Enabled = true;
					AxisX.Text = "Axis X";
					AxisY.Enabled = true;
					AxisY.Text = "Axis Y";
					AxisZ.Enabled = true;
					Steer.Enabled = true;
					JointFriction.Enabled = false;
					JointFrictionLabel.Enabled = false;
					JointSpring.Enabled = false;
					JointSpringLabel.Enabled = false;
					JointDamping.Enabled = false;
					JointDampingLabel.Enabled = false;
					break;
				case 1: // Cloth
					LimitsBox.Enabled = false;
					AxisX.Enabled = true;
					AxisX.Text = "Axis X";
					AxisY.Enabled = true;
					AxisY.Text = "Axis Y";
					AxisZ.Enabled = true;
					Steer.Enabled = true;
					JointFriction.Enabled = true;
					JointFrictionLabel.Enabled = true;
					JointSpring.Enabled = true;
					JointSpringLabel.Enabled = true;
					JointDamping.Enabled = true;
					JointDampingLabel.Enabled = true;
					break;
				case 2: // Joint
					LimitsBox.Enabled = true;
					AxisX.Enabled = true;
					AxisX.Text = "Axis X";
					AxisY.Enabled = true;
					AxisY.Text = "Axis Y";
					AxisZ.Enabled = true;
					Steer.Enabled = false;
					JointFriction.Enabled = true;
					JointFrictionLabel.Enabled = true;
					JointSpring.Enabled = true;
					JointSpringLabel.Enabled = true;
					JointDamping.Enabled = true;
					JointDampingLabel.Enabled = true;
					break;
				case 3: // Wheel
					LimitsBox.Enabled = true;
					AxisX.Enabled = true;
					AxisX.Text = "Axis X";
					AxisY.Enabled = false;
					AxisY.Text = "Axis Y";
					AxisZ.Enabled = false;
					Steer.Enabled = false;
					JointFriction.Enabled = true;
					JointFrictionLabel.Enabled = true;
					JointSpring.Enabled = true;
					JointSpringLabel.Enabled = true;
					JointDamping.Enabled = true;
					JointDampingLabel.Enabled = true;
					break;
				case 4: // Slider
					LimitsBox.Enabled = true;
					AxisX.Enabled = true;
					AxisX.Text = "Slide Z";
					AxisY.Enabled = true;
					AxisY.Text = "Rotate Z";
					AxisZ.Enabled = false;
					Steer.Enabled = false;
					JointFriction.Enabled = true;
					JointFrictionLabel.Enabled = true;
					JointSpring.Enabled = true;
					JointSpringLabel.Enabled = true;
					JointDamping.Enabled = true;
					JointDampingLabel.Enabled = true;
					break;
			}
		}
    }
}
