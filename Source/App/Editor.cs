using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using System.Threading;

namespace Object_tool
{
    public partial class Object_Editor
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

		private int StartEditor(bool async, EditorMode mode, string object_path, string second_path = "null", int flags = -1, float scale = 1.0f, string[] temp_arr = null)
		{
			if (m_Object == null)
			{
				Msg("StartEditor: Error!\nObject is null. Please report this bug for developer.");
				return -1;
			}

			if (flags == -1)
			{
				flags = GetFlags();
				scale = m_Object.scale;
			}

			string args = $"{(int)mode} \"{object_path}\" \"{second_path}\" {flags} {scale} {m_Object.bones.Count} {m_Object.surfaces.Count} {OpenSklsDialog.FileNames.Count()}";

			// Экспортируем кости
			for (int i = 0; i < m_Object.bones.Count; i++)
			{
				args += $" \"{m_Object.bones[i].bone_id}-{m_Object.bones[i].shape_type}-{m_Object.bones[i].shape_flags}\"";
				args += $" \"{m_Object.bones[i].material}\"";
				args += $" {m_Object.bones[i].mass}";
			}

			// Экспортируем текстуры
			for (int i = 0; i < m_Object.surfaces.Count; i++)
			{
				args += $" {m_Object.surfaces[i].flags}";
				args += $" \"{m_Object.surfaces[i].texture}\"";
				args += $" \"{m_Object.surfaces[i].shader}\"";
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
			args += $" \"{m_Object.FILE_NAME}\"";

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
			args += $" \"{TempFolder()}\"";

			int exit_code = RunCompiller(args, async);

			if (File.Exists(object_path + "_temp.userdata"))
				File.Delete(object_path + "_temp.userdata");
			return exit_code;
		}

		private int RunCompiller(string args, bool async)
		{
			string exe_path = AppPath() + "\\Object Editor.exe";
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
				EditorProcess.StartInfo.WorkingDirectory = AppPath();
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

				dLastCopmileTime = Math.Round((EditorProcess.ExitTime - EditorProcess.StartTime).TotalSeconds, 3);

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
								int code = StartEditor(true, EditorMode.ExportOGF, m_Object.TEMP_FILE_NAME, SaveOgfDialog.FileName);
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
								int code = StartEditor(true, EditorMode.ExportOMF, m_Object.TEMP_FILE_NAME, SaveOmfDialog.FileName);
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
								int code = StartEditor(true, EditorMode.LoadMotions, m_Object.TEMP_FILE_NAME);
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

									m_Object.LoadMotions();
									AfterLoadMotions();
								}
							});
							SdkThread.Start();
						}
					}
					break;
				case "DeleteSkls":
					{
						SdkThread = new Thread(() => {
							int code = StartEditor(true, EditorMode.DeleteMotions, m_Object.TEMP_FILE_NAME);
							if (!EditorKilled)
							{
								if (code == 0)
								{
									AutoClosingMessageBox.Show($"Motions successfully deleted. {GetTime()}", "", 1000, MessageBoxIcon.Information);
									DeletesklsToolStripMenuItem.Enabled = false;
									SaveSklsToolStripMenuItem.Enabled = false;
									sklToolStripMenuItem.Enabled = false;
									oMFToolStripMenuItem.Enabled = false;

									m_Object.LoadMotions();
									AfterLoadMotions();
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
								int code = StartEditor(true, EditorMode.SaveSklsMotions, m_Object.TEMP_FILE_NAME, SaveSklsDialog.FileName);
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
								int code = StartEditor(true, EditorMode.SaveSklMotions, m_Object.TEMP_FILE_NAME, SaveSklDialog.FileName);
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
								int code = StartEditor(true, EditorMode.LoadBones, m_Object.TEMP_FILE_NAME, OpenBonesDialog.FileName);
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
								int code = StartEditor(true, EditorMode.SaveBones, m_Object.TEMP_FILE_NAME, SaveBonesDialog.FileName);
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
								int code = StartEditor(true, EditorMode.ExportOBJ, m_Object.TEMP_FILE_NAME, SaveObjDialog.FileName);
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
							int code = StartEditor(true, EditorMode.GenerateShape, m_Object.TEMP_FILE_NAME);
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
								int code = StartEditor(true, EditorMode.LoadBoneParts, m_Object.TEMP_FILE_NAME, OpenLtxDialog.FileName);
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
								int code = StartEditor(true, EditorMode.SaveBoneParts, m_Object.TEMP_FILE_NAME, SaveBonePartsDialog.FileName);
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
							int code = StartEditor(true, EditorMode.ToDefaultBoneParts, m_Object.TEMP_FILE_NAME);
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
								int code = StartEditor(true, EditorMode.ExportDM, m_Object.TEMP_FILE_NAME, SaveDmDialog.FileName);
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
									int code = StartEditor(true, EditorMode.GenerateLod, m_Object.TEMP_FILE_NAME, SaveOgfLodDialog.FileName);
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
								int code = StartEditor(true, EditorMode.SaveCpp, m_Object.TEMP_FILE_NAME, SaveCppDialog.FileName);
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
								int code = StartEditor(true, EditorMode.BatchLtx, m_Object.TEMP_FILE_NAME, OpenBatchLtxDialog.FileName, batch_flags.GetFlags(dbg_window), batch_flags.scale);
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
									int code = StartEditor(true, EditorMode.BatchDialogOGF, m_Object.TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
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
									int code = StartEditor(true, EditorMode.BatchDialogOMF, m_Object.TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
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
									int code = StartEditor(true, EditorMode.BatchDialogOGF, m_Object.TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
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
									int code = StartEditor(true, EditorMode.BatchDialogOMF, m_Object.TEMP_FILE_NAME, "null", batch_flags.GetFlags(dbg_window), batch_flags.scale);
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

			if (AutoSmooth.Checked)
				flags |= (1 << 12);

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
	}
}
