using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace Object_tool
{
    public partial class BatchFlags : Form
    {
		public bool res = false;
		private bool bKeyIsDown = false;
		public float scale = 1.0f;

		public BatchFlags(bool dev_mode, bool soc)
        {
            InitializeComponent();

			if (!dev_mode)
            {
				AnimsNoCompress.Visible = false;
				Anims16Bit.Checked = true;

				BuildInMotionsExport.Location = AnimsNoCompress.Location;
				MotionFlagsGroupBox.Size = new Size(new Point(MotionFlagsGroupBox.Size.Width, MotionFlagsGroupBox.Size.Height - 22));
				ScaleGroupBox.Location = new Point(ScaleGroupBox.Location.X, ScaleGroupBox.Location.Y - 22);
				ScaleGroupBox.Size = new Size(new Point(ScaleGroupBox.Size.Width, ScaleGroupBox.Size.Height + 22));
				Anims8Bit.Checked = soc;
			}

			SoCInfluence.Checked = soc;
			SmoothSoC.Checked = soc;
		}

		public int GetFlags(bool dbg_window)
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

			if (SmoothSoC.Checked)
				flags |= (1 << 10);

			if (SoCInfluence.Checked)
				flags |= (1 << 11);

			return flags;
		}

        private void button1_Click(object sender, EventArgs e)
        {
			res = true;
			Close();
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
					scale = Convert.ToSingle(curBox.Text);
				}
				catch (Exception)
				{
					curBox.Text = scale.ToString();

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
	}
}
