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
	public partial class Object_Editor : Form
	{
		// File sytem
		public string FILE_NAME = "";
		public bool dbg_window = false;

		// Input
		public bool bKeyIsDown = false;
		string number_mask = "";

		private int ExportOGF(string object_path, string ogf_path)
		{
			return RunCompiller($"0 \"{object_path}\" \"{ogf_path}\"");
		}

		private int ExportOMF(string object_path, string omf_path)
		{
			return RunCompiller($"1 \"{object_path}\" \"{omf_path}\"");
		}

		private int MotionCount(string object_path)
		{
			return RunCompiller($"2 \"{object_path}\"", true);
		}

		private int DeleteMotions(string object_path)
		{
			return RunCompiller($"3 \"{object_path}\"", true);
		}

		private int LoadMotions(string object_path, string skls_path)
		{
			return RunCompiller($"4 \"{object_path}\" \"{skls_path}\"", true);
		}

		private int SaveMotions(string object_path, string skls_path)
		{
			return RunCompiller($"5 \"{object_path}\" \"{skls_path}\"", false);
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

		public Object_Editor()
		{
			InitializeComponent();
			number_mask = @"^-[0-9.]*$";
			System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
			openFileDialog1.Filter = "Object file|*.object";
			openFileDialog2.Filter = "Skls file|*.skls";
			saveFileDialog1.Filter = "Skls file|*.skls";

			if (Environment.GetCommandLineArgs().Length > 1)
			{
				FILE_NAME = Environment.GetCommandLineArgs()[1];
				bool has_motions = MotionCount(FILE_NAME) > 0;
				DeleteMotionsButton.Enabled = has_motions;
				//ExtractMotionsButton.Enabled = has_motions;
			}
		}

        private void ExportOGF_Click(object sender, EventArgs e)
        {
			string ogf = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('.')) + ".ogf";

			if (ExportOGF(FILE_NAME, ogf) == 0)
				MessageBox.Show("Done!");
		}

        private void ExportOMF_Click(object sender, EventArgs e)
        {
			string omf = FILE_NAME.Substring(0, FILE_NAME.LastIndexOf('.')) + ".omf";

			if (ExportOMF(FILE_NAME, omf) == 0)
				MessageBox.Show("Done!");
		}

        private void LoadSkls_Click(object sender, EventArgs e)
        {
			if (openFileDialog2.ShowDialog() == DialogResult.OK)
			{
				if (LoadMotions(FILE_NAME, openFileDialog2.FileName) == 0)
				{
					MessageBox.Show("Done!");
					DeleteMotionsButton.Enabled = true;
					//ExtractMotionsButton.Enabled = true;
				}
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
				DeleteMotionsButton.Enabled = false;
				//ExtractMotionsButton.Enabled = false;
			}
		}

        private void ExtractMotionsButton_Click(object sender, EventArgs e)
        {
			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				MessageBox.Show(SaveMotions(FILE_NAME, openFileDialog2.FileName).ToString());
			}
		}
    }
}
