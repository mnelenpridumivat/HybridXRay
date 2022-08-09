using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.IO;

namespace Object_tool
{
	public partial class Object_Editor
	{
		public void InitSettings()
		{
			string file_path = AppPath() + "\\Settings.ini";
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
			pSettings.Load(SmoothCoP);
			pSettings.Load(AutoSmooth, true);
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
		}

		public void InitScripts()
		{
			SCRIPT_FOLDER = AppPath() + "\\scripts\\";

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
		}

		public void InitArgs()
		{
			if (Environment.GetCommandLineArgs().Length > 1)
			{
				string filename = Environment.GetCommandLineArgs()[1];
				IsOgfMode = false;

				if (Environment.GetCommandLineArgs().Length > 2)
				{
					string mode = Environment.GetCommandLineArgs()[2];
					if (mode == "model_viewer")
					{
						m_Object = new Object();
						m_Object.Init(filename, TempFolder());
						m_Object.LoadData();

						SmoothSoC.Checked = true;

						ViewtoolStripMenuItem_Click(null, null); // Start Viewer, need inited object with loaded data

						if (Directory.Exists(TempFolder()))
							Directory.Delete(TempFolder(), true);

						Close();
						return;
					}
					else if (mode == "skeleton_only")
						IsOgfMode = true;
				}

				OpenFile(filename);
			}
		}
		private void InitDialogs()
		{
			if (m_Object != null)
			{
				string old_filename = m_Object.FILE_NAME;
				if (IsOgfMode)
					m_Object.FILE_NAME = Environment.GetCommandLineArgs()[3];

				SaveOgfDialog.InitialDirectory = FileFolder();
				SaveOgfDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".ogf";

				SaveOmfDialog.InitialDirectory = FileFolder();
				SaveOmfDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".omf";

				SaveSklsDialog.InitialDirectory = FileFolder();
				SaveSklsDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".skls";

				SaveBonesDialog.InitialDirectory = FileFolder();
				SaveBonesDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".bones";

				SaveObjDialog.InitialDirectory = FileFolder();
				SaveObjDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".obj";

				SaveBonePartsDialog.InitialDirectory = FileFolder();
				SaveBonePartsDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + "_boneparts.ltx";

				SaveCppDialog.InitialDirectory = FileFolder();
				SaveCppDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + "_cpp.ltx";

				SaveMotionRefsDialog.InitialDirectory = FileFolder();
				SaveMotionRefsDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + "_refs.ltx";

				SaveUserDataDialog.InitialDirectory = FileFolder();
				SaveUserDataDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + "_userdata.ltx";

				SaveSklDialog.InitialDirectory = FileFolder();

				SaveDmDialog.InitialDirectory = FileFolder();
				SaveDmDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".dm";

				SaveObjectDialog.InitialDirectory = FileFolder();
				SaveObjectDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + ".object";

				SaveOgfLodDialog.InitialDirectory = FileFolder();
				SaveOgfLodDialog.FileName = m_Object.NAME.Substring(0, m_Object.NAME.LastIndexOf('.')) + "_lod.ogf";

				m_Object.FILE_NAME = old_filename;
			}
			else
				Msg("InitDialogs: Error!\nObject is null. Please report this bug for developer.");
		}

		private void InitBoneUI()
		{
			if (m_Object != null)
			{
				if (m_Object.bones != null)
				{
					if (USE_OLD_BONES)
					{
						for (int i = m_Object.bones.Count - 1; i >= 0; i--)
							CreateBoneGroupBox(i, m_Object.bones[i]);
					}
					else
					{
						for (int i = 0; i < m_Object.bones.Count; i++)
							BonesList.Items.Add(m_Object.bones[i].bone_name);
						BoneMaterial.Items.AddRange(game_materials);
						if (BonesList.Items.Count > 0)
							BonesList.SelectedIndex = 0;
					}
				}
			}
			else
				Msg("InitBoneUI: Error!\nObject is null. Please report this bug for developer.");
		}

		private void InitSurfaceUI()
		{
			if (m_Object != null)
			{
				if (m_Object.surfaces != null)
				{
					for (int i = 0; i < m_Object.surfaces.Count; i++)
						CreateSurfaceGroupBox(i, m_Object.surfaces[i]);
				}
			}
			else
				Msg("InitSurfaceUI: Error!\nObject is null. Please report this bug for developer.");
		}

		private void InitUI()
		{
			CurrentSize = this.Size;
			BoneSize    = new Size(713, 790);

			shapeParamsToolStripMenuItem.Enabled        = false;
			surfaceParamsToolStripMenuItem.Enabled      = false;
			saveToolStripMenuItem.Enabled               = false;
			exportToolStripMenuItem.Enabled             = false;
			deleteToolStripMenuItem.Enabled             = false;
			sklSklsToolStripMenuItem.Enabled            = false;
			bonesToolStripMenuItem.Enabled              = false;
			dMToolStripMenuItem.Enabled                 = false;
			bonesPartsToolStripMenuItem.Enabled         = false;
			FlagsGroupBox.Enabled                       = false;
			MotionRefsBox.Enabled                       = false;
			UserDataTextBox.Enabled                     = false;
			LodTextBox.Enabled                          = false;
			ScaleGroupBox.Enabled                       = false;
			motionRefsToolStripMenuItem.Enabled         = false;
			userDataToolStripMenuItem.Enabled           = false;
			motionRefsToolStripMenuItem1.Enabled        = false;
			userDataToolStripMenuItem1.Enabled          = false;
			generateLodToolStripMenuItem.Enabled        = false;
			objectInfoToolStripMenuItem.Enabled         = false;
			UseSplitNormals.Enabled                     = false;
			normalsToolStripMenuItem.Enabled            = false;
			importObjectParamsToolStripMenuItem.Enabled = false;
			saveToolStripMenuItem1.Enabled              = false;
			generateShapesToolStripMenuItem.Enabled     = false;
			viewPortToolStripMenuItem.Enabled           = false;

			if (USE_OLD_BONES)
				BonesPage.Controls.Clear();
		}

		private void UpdateUI()
		{
			if (m_Object != null)
			{
				bool has_motions = m_Object.motions.Count > 0;
				bool has_bones = m_Object.bones.Count > 0;

				StatusFile.Text                                 = m_Object.NAME;
				DeletesklsToolStripMenuItem.Enabled             = has_motions;
				SaveSklsToolStripMenuItem.Enabled               = has_motions;
				sklToolStripMenuItem.Enabled                    = has_motions;
				oMFToolStripMenuItem.Enabled                    = has_motions;
				saveToolStripMenuItem.Enabled                   = !IsOgfMode;
				exportToolStripMenuItem.Enabled                 = true;
				deleteToolStripMenuItem.Enabled                 = true;
				sklSklsToolStripMenuItem.Enabled                = has_bones;
				bonesToolStripMenuItem.Enabled                  = has_bones;
				oGFToolStripMenuItem.Enabled                    = !IsOgfMode;
				objToolStripMenuItem.Enabled                    = !IsOgfMode;
				objectToolStripMenuItem.Enabled                 = !IsOgfMode;
				dMToolStripMenuItem.Enabled                     = !IsOgfMode;
				bonesPartsToolStripMenuItem.Enabled             = has_bones;
				cToolStripMenuItem.Enabled                      = !IsOgfMode;
				bonesToolStripMenuItem1.Enabled                 = has_bones;
				bonesPartsToolStripMenuItem1.Enabled            = has_bones;
				bonesPartsToDefaultToolStripMenuItem.Enabled    = has_bones;
				ScaleGroupBox.Enabled                           = !IsOgfMode;
				MotionRefsBox.Enabled                           = has_bones;
				UserDataTextBox.Enabled                         = has_bones;
				LodTextBox.Enabled                              = has_bones;
				motionRefsToolStripMenuItem.Enabled             = has_bones && !IsOgfMode;
				userDataToolStripMenuItem.Enabled               = has_bones && !IsOgfMode;
				ModelFlagsGroupBox.Enabled                      = !IsOgfMode;
				FlagsGroupBox.Enabled                           = true;
				generateLodToolStripMenuItem.Enabled            = !IsOgfMode;
				objectInfoToolStripMenuItem.Enabled             = !IsOgfMode;
				importObjectParamsToolStripMenuItem.Enabled     = !IsOgfMode;
				saveToolStripMenuItem1.Enabled                  = !IsOgfMode;
				shapeParamsToolStripMenuItem.Enabled            = !IsOgfMode && has_bones;
				generateShapesToolStripMenuItem.Enabled         = !IsOgfMode && has_bones;
				surfaceParamsToolStripMenuItem.Enabled          = !IsOgfMode;
				viewPortToolStripMenuItem.Enabled               = true;

				SurfacesPage.Controls.Clear();

				if (USE_OLD_BONES)
					BonesPage.Controls.Clear();

				if (IsOgfMode)
				{
					TabControl.Controls.Clear();
					TabControl.Controls.Add(FlagsPage);
					TabControl.Controls.Add(MotionPage);
				}
			}
			else
				Msg("UpdateUI: Error!\nObject is null. Please report this bug for developer.");
		}
	}
}
