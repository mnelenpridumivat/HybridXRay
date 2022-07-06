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
    public partial class GenerateLod : Form
    {
        public bool res = false;
        public float lod_quality = 0.5f;
        public bool progressive = false;

        public GenerateLod()
        {
            InitializeComponent();
        }

        private void CloseForm(object sender, FormClosingEventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            lod_quality = (float)(trackBar1.Value * 0.1f);
            progressive = checkBox1.Checked;
            res = true;
            Close();
        }
    }
}
