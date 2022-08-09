using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Object_tool
{
    public partial class Settings : Form
    {
        private EditorSettings pSettings = null;
        private Form EditorForm = null;
        private Object_Editor Editor = null;
        public Settings(EditorSettings settings, Form main_form, Object_Editor editor)
        {
            InitializeComponent();
            pSettings = settings;
            EditorForm = main_form;
            Editor = editor;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.FileName = "";
            ofd.Filter = "Xr file|*.xr";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                GameMtlPath.Text = ofd.FileName;
                Editor.ReloadGameMtl(GameMtlPath.Text);
            }
        }

        private void SaveParams(object sender, FormClosingEventArgs e)
        {
            pSettings.Save(NoCompress);
            pSettings.Save(Debug);
            pSettings.Save(DefaultsCoP);
            pSettings.Save(DefaultsSoC);
            pSettings.Save(GameMtlPath);
            pSettings.Save(FSLtxPath);
            pSettings.Save(TexturesPath);
            pSettings.Save(ProgressiveMeshes);
            pSettings.Save(StripifyMeshes);
            pSettings.Save(OptimizeSurfaces);
            pSettings.Save(SoCInfluence);
            pSettings.Save(SplitNormalsChbx);
            pSettings.Save(BuildInMotionsExport);
            pSettings.Save(SmoothSoC);
            pSettings.Save(SmoothCoP);
            pSettings.Save(AutoSmooth);
            pSettings.Save(Anims8Bit);
            pSettings.Save(Anims16Bit);
            pSettings.Save(AnimsNoCompress);
            pSettings.Save(NoCompressLinkLabel);
            pSettings.Save(HQGeometry);
            pSettings.Save(HQGeometryPlus);
            pSettings.Save(ScaleCenterOfMassCheckBox);
            pSettings.Save(ForceViewport);
        }

        private void Settings_Load(object sender, EventArgs e)
        {
            pSettings.Load(NoCompress);
            pSettings.Load(Debug);
            pSettings.Load(DefaultsCoP, true);
            pSettings.Load(DefaultsSoC);
            pSettings.Load(GameMtlPath);
            pSettings.Load(FSLtxPath);
            pSettings.Load(TexturesPath);
            pSettings.Load(ProgressiveMeshes);
            pSettings.Load(StripifyMeshes);
            pSettings.Load(OptimizeSurfaces);
            pSettings.Load(SoCInfluence);
            pSettings.Load(SplitNormalsChbx, true);
            pSettings.Load(BuildInMotionsExport, true);
            pSettings.Load(SmoothSoC);
            pSettings.Load(SmoothCoP);
            pSettings.Load(AutoSmooth, true);
            pSettings.Load(Anims8Bit);
            pSettings.Load(Anims16Bit, !NoCompress.Checked);
            pSettings.Load(AnimsNoCompress, NoCompress.Checked);
            pSettings.Load(NoCompressLinkLabel);
            pSettings.Load(HQGeometry);
            pSettings.Load(HQGeometryPlus, true);
            pSettings.Load(ScaleCenterOfMassCheckBox, true);
            pSettings.Load(ForceViewport, true);

            AnimsNoCompress.Enabled = NoCompress.Checked;
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            NoCompressLinkLabel.LinkVisited = true;
            System.Diagnostics.Process.Start("https://github.com/mortany/stcop-engine/commit/7d6e967ff081f8f797890f2c1954ce493c8b7084");
        }

        private void DefaultsSoC_CheckedChanged(object sender, EventArgs e)
        {
            SoCInfluence.Checked = true;
            if (!NoCompress.Checked)
                Anims8Bit.Checked = true;
        }

        private void DefaultsCoP_CheckedChanged(object sender, EventArgs e)
        {
            SoCInfluence.Checked = false;
            if (!NoCompress.Checked)
                Anims16Bit.Checked = true;
            else
                AnimsNoCompress.Checked = true;
        }

        private void NoCompress_CheckedChanged(object sender, EventArgs e)
        {
            AnimsNoCompress.Enabled = (sender as CheckBox).Checked;

            if (AnimsNoCompress.Enabled)
                AnimsNoCompress.Checked = true;
            else
            {
                if (DefaultsSoC.Checked)
                    Anims8Bit.Checked = true;
                else
                    Anims16Bit.Checked = true;
            }

            Editor.SyncCompressUI(NoCompress.Checked);
        }

        private void SyncForm(object sender, EventArgs e)
        {
            Control control = sender as Control;
            bool CheckState = (sender is CheckBox) ? (sender as CheckBox).Checked : (sender as RadioButton).Checked;

            Control[] MainControls = EditorForm.Controls.Find(control.Name, true);

            for (int i = 0; i < MainControls.Length; i++)
            {
                if (MainControls[i] is CheckBox)
                    (MainControls[i] as CheckBox).Checked = CheckState;
                else
                    (MainControls[i] as RadioButton).Checked = CheckState;
            }
        }

        private void ProgressiveCheck(object sender, EventArgs e)
        {
            CheckBox chbx = sender as CheckBox;

            if (chbx.Checked)
                StripifyMeshes.Checked = false;
        }

        private void StripifyCheck(object sender, EventArgs e)
        {
            CheckBox chbx = sender as CheckBox;

            if (chbx.Checked)
                ProgressiveMeshes.Checked = false;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.FileName = "";
            ofd.Filter = "Ltx file|*.ltx";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                FSLtxPath.Text = ofd.FileName;

                string gamedata_path = ofd.FileName.Substring(0, ofd.FileName.LastIndexOf('\\')) + "\\" + GetFSPath(ofd.FileName, "$game_data$");

                if (File.Exists(gamedata_path + "gamemtl.xr"))
                {
                    GameMtlPath.Text = gamedata_path + "gamemtl.xr";
                    Editor.ReloadGameMtl(GameMtlPath.Text);
                }
                TexturesPath.Text = gamedata_path + GetFSPath(ofd.FileName, "$game_textures$");
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            FolderSelectDialog folderSelectDialog = new FolderSelectDialog();
            if (folderSelectDialog.ShowDialog())
            {
                TexturesPath.Text = folderSelectDialog.FileName + "\\";
            }
        }

        private string GetFSPath(string filename, string key)
        {
            using (StreamReader file = new StreamReader(filename))
            {
                string ln;
                string path = "";
                while ((ln = file.ReadLine()) != null)
                {
                    if (ln.Contains(key))
                    {
                        int separator_count = 0;
                        foreach (char c in ln)
                        {
                            if (separator_count == 3)
                            {
                                path += c;
                            }

                            if (c == '|')
                                separator_count++;
                        }

                        return GetCorrectString(path);
                    }
                }
                file.Close();
            }

            return "";
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
    }
}
