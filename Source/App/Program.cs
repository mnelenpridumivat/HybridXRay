using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;

public class AutoClosingMessageBox
{
    System.Threading.Timer _timeoutTimer;
    string _caption;
    AutoClosingMessageBox(string text, string caption, int timeout, MessageBoxIcon icon = 0)
    {
        _caption = caption;
        _timeoutTimer = new System.Threading.Timer(OnTimerElapsed,
            null, timeout, System.Threading.Timeout.Infinite);
        using (_timeoutTimer)
            MessageBox.Show(text, caption, 0, icon);
    }
    public static void Show(string text, string caption, int timeout, MessageBoxIcon icon = 0)
    {
        new AutoClosingMessageBox(text, caption, timeout, icon);
    }
    void OnTimerElapsed(object state)
    {
        IntPtr mbWnd = FindWindow("#32770", _caption); // lpClassName is #32770 for MessageBox
        if (mbWnd != IntPtr.Zero)
            SendMessage(mbWnd, WM_CLOSE, IntPtr.Zero, IntPtr.Zero);
        _timeoutTimer.Dispose();
    }
    const int WM_CLOSE = 0x0010;
    [System.Runtime.InteropServices.DllImport("user32.dll", SetLastError = true)]
    static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
    [System.Runtime.InteropServices.DllImport("user32.dll", CharSet = System.Runtime.InteropServices.CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);
}

namespace Object_tool
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Object_Editor());
        }
    }
    public class EditorSettings
    {
        private IniFile pSettings = null;
        private string sMainSect = "settings";

        public int SETTINGS_VERS = 1;

        public bool CheckVers()
        {
            int vers = Convert.ToInt32(pSettings.ReadDef("SettingsVersion", sMainSect, "0"));
            return vers == SETTINGS_VERS;
        }

        public EditorSettings(string IniPath = null)
        {
            pSettings = new IniFile(IniPath);
        }

        public EditorSettings(string IniPath = null, string init_write = null)
        {
            pSettings = new IniFile(IniPath, init_write);
        }

        public void SaveVersion()
        {
            pSettings.Write("SettingsVersion", Convert.ToInt32(SETTINGS_VERS).ToString(), sMainSect);
        }

        public void Save(CheckBox box)
        {
            pSettings.Write(box.Name, Convert.ToUInt16(box.Checked).ToString(), sMainSect);
        }

        public void Save(RadioButton box)
        {
            pSettings.Write(box.Name, Convert.ToUInt16(box.Checked).ToString(), sMainSect);
        }

        public void Save(TextBox box)
        {
            pSettings.Write(box.Name, box.Text, sMainSect);
        }

        public void Save(string key, bool var)
        {
            pSettings.Write(key, Convert.ToUInt16(var).ToString(), sMainSect);
        }

        public void Save(string key, string var)
        {
            pSettings.Write(key, var, sMainSect);
        }

        public void Save(LinkLabel box)
        {
            pSettings.Write(box.Name, Convert.ToUInt16(box.LinkVisited).ToString(), sMainSect);
        }

        public void Save(string name, FileDialog dialog)
        {
            pSettings.Write(name, dialog.FileName, sMainSect);
        }

        public void Save(string name, FolderSelectDialog dialog)
        {
            Save(name, dialog.Parent);
        }

        public void Save(string key, int var)
        {
            pSettings.Write(key, var.ToString(), sMainSect);
        }

        public bool Load(CheckBox box, bool def = false)
        {
            if (CheckVers())
                box.Checked = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(box.Name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                box.Checked = def;
            return box.Checked;
        }

        public bool Load(RadioButton box, bool def = false)
        {
            if (CheckVers())
                box.Checked = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(box.Name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                box.Checked = def;
            return box.Checked;
        }

        public void Load(TextBox box, string def = "")
        {
            if (CheckVers())
                box.Text = pSettings.ReadDef(box.Name, sMainSect, def);
            else
                box.Text = def;
        }

        public bool Load(string key, ref bool var, bool def = false)
        {
            if (CheckVers())
                var = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(key, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                var = def;
            return var;
        }

        public void Load(string key, ref string var, string def = "")
        {
            if (CheckVers())
                var = pSettings.ReadDef(key, sMainSect, def);
            else
                var = def;
        }

        public bool Load(string name, CheckBox box, bool def = false)
        {
            if (CheckVers())
                box.Checked = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                box.Checked = def;
            return box.Checked;
        }

        public bool Load(string name, RadioButton box, bool def = false)
        {
            if (CheckVers())
                box.Checked = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                box.Checked = def;
            return box.Checked;
        }

        public bool Load(LinkLabel box, bool def = false)
        {
            if (CheckVers())
                box.LinkVisited = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(box.Name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                box.LinkVisited = def;
            return box.LinkVisited;
        }

        public bool LoadState(string name, ref bool state, bool def = false)
        {
            if (CheckVers())
                state = Convert.ToBoolean(Convert.ToUInt16(pSettings.ReadDef(name, sMainSect, Convert.ToUInt16(def).ToString())));
            else
                state = def;
            return state;
        }

        public string LoadText(string name, ref string text, string def = "")
        {
            if (CheckVers())
                text = pSettings.ReadDef(name, sMainSect, def);
            else
                text = def;
            return text;
        }

        public int Load(string name, ref int text, int def = 0)
        {
            if (CheckVers())
                text = Convert.ToInt32(pSettings.ReadDef(name, sMainSect, def.ToString()));
            else
                text = def;
            return text;
        }

        public string Load(string name, FileDialog dialog, bool is_folder = false)
        {
            if (CheckVers())
            {
                string full_path = pSettings.ReadDef(name, sMainSect, dialog.FileName);
                dialog.FileName = Path.GetFileName(dialog.FileName);
                if (full_path.Length > 0 && full_path.LastIndexOf('\\') > 0 && !is_folder)
                    dialog.InitialDirectory = full_path.Substring(0, full_path.LastIndexOf('\\'));
                else
                    dialog.InitialDirectory = full_path;
            }
            else
            {
                string full_filename = dialog.FileName;
                if (full_filename.Length > 0)
                {
                    dialog.FileName = Path.GetFileName(dialog.FileName);
                    dialog.InitialDirectory = full_filename.Substring(0, full_filename.LastIndexOf('\\'));
                }
            }
            return dialog.FileName;
        }

        public string Load(string name, FolderSelectDialog dialog)
        {
            return Load(name, dialog.Parent, true);
        }
    }

    public class IniFile   // revision 11
    {
        private FileInfo Ini;
        private string EXE = Assembly.GetExecutingAssembly().GetName().Name;

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        private static extern long WritePrivateProfileString(string Section, string Key, string Value, string FilePath);

        [DllImport("kernel32", CharSet = CharSet.Unicode)]
        private static extern int GetPrivateProfileString(string Section, string Key, string Default, StringBuilder RetVal, int Size, string FilePath);

        public IniFile(string IniPath = null)
        {
            if (!File.Exists(IniPath))
                File.Create(IniPath);

            string file_name = (IniPath ?? EXE + ".ini");
            Ini = new FileInfo(file_name);
        }

        public IniFile(string IniPath = null, string init_write = "")
        {
            if (!File.Exists(IniPath))
                File.WriteAllText(IniPath, init_write);

            string file_name = (IniPath ?? EXE + ".ini");
            Ini = new FileInfo(file_name);
        }

        public string Read(string Key, string Section = null)
        {
            var RetVal = new StringBuilder(255);
            GetPrivateProfileString(Section ?? this.EXE, Key, "", RetVal, 255, this.Ini.FullName);
            return RetVal.ToString();
        }

        public string ReadDef(string Key, string Section = null, string def = null)
        {
            var RetVal = new StringBuilder(255);
            GetPrivateProfileString(Section ?? this.EXE, Key, "", RetVal, 255, this.Ini.FullName);
            return RetVal.ToString() != "" ? RetVal.ToString() : def;
        }

        public void Write(string Key, string Value, string Section = null)
        {
            WritePrivateProfileString(Section ?? this.EXE, Key, Value, this.Ini.FullName);
        }

        public void DeleteKey(string Key, string Section = null)
        {
            Write(Key, null, Section ?? this.EXE);
        }

        public void DeleteSection(string Section = null)
        {
            Write(null, null, Section ?? this.EXE);
        }

        public bool KeyExists(string Key, string Section = null)
        {
            return Read(Key, Section).Length > 0;
        }
    }

    public class FolderSelectDialog
    {
        // Wrapped dialog
        System.Windows.Forms.OpenFileDialog ofd = null;

        /// <summary>
        /// Default constructor
        /// </summary>
        public FolderSelectDialog()
        {
            ofd = new System.Windows.Forms.OpenFileDialog();

            ofd.Filter = "Folders|\n";
            ofd.AddExtension = false;
            ofd.CheckFileExists = false;
            ofd.DereferenceLinks = true;
            ofd.Multiselect = false;
        }

        #region Properties

        /// <summary>
        /// Gets/Sets the initial folder to be selected. A null value selects the current directory.
        /// </summary>
        public string InitialDirectory
        {
            get { return ofd.InitialDirectory; }
            set { ofd.InitialDirectory = value == null || value.Length == 0 ? Environment.CurrentDirectory : value; }
        }

        public FileDialog Parent
        {
            get { return ofd; }
        }

        /// <summary>
        /// Gets/Sets the title to show in the dialog
        /// </summary>
        public string Title
        {
            get { return ofd.Title; }
            set { ofd.Title = value == null ? "Select a folder" : value; }
        }

        /// <summary>
        /// Gets the selected folder
        /// </summary>
        public string FileName
        {
            get { return ofd.FileName; }
            set { ofd.FileName = value; }
        }

        public string[] FileNames
        {
            get { return ofd.FileNames; }
        }

        public bool Multiselect
        {
            get { return ofd.Multiselect; }
            set { ofd.Multiselect = value; }
        }

        #endregion

        #region Methods

        /// <summary>
        /// Shows the dialog
        /// </summary>
        /// <returns>True if the user presses OK else false</returns>
        public bool ShowDialog()
        {
            return ShowDialog(IntPtr.Zero);
        }

        /// <summary>
        /// Shows the dialog
        /// </summary>
        /// <param name="hWndOwner">Handle of the control to be parent</param>
        /// <returns>True if the user presses OK else false</returns>
        public bool ShowDialog(IntPtr hWndOwner)
        {
            bool flag = false;

            if (Environment.OSVersion.Version.Major >= 6)
            {
                var r = new Reflector("System.Windows.Forms");

                uint num = 0;
                Type typeIFileDialog = r.GetType("FileDialogNative.IFileDialog");
                object dialog = r.Call(ofd, "CreateVistaDialog");
                r.Call(ofd, "OnBeforeVistaDialog", dialog);

                uint options = (uint)r.CallAs(typeof(System.Windows.Forms.FileDialog), ofd, "GetOptions");
                options |= (uint)r.GetEnum("FileDialogNative.FOS", "FOS_PICKFOLDERS");
                r.CallAs(typeIFileDialog, dialog, "SetOptions", options);

                object pfde = r.New("FileDialog.VistaDialogEvents", ofd);
                object[] parameters = new object[] { pfde, num };
                r.CallAs2(typeIFileDialog, dialog, "Advise", parameters);
                num = (uint)parameters[1];
                try
                {
                    int num2 = (int)r.CallAs(typeIFileDialog, dialog, "Show", hWndOwner);
                    flag = 0 == num2;
                }
                finally
                {
                    r.CallAs(typeIFileDialog, dialog, "Unadvise", num);
                    GC.KeepAlive(pfde);
                }
            }
            else
            {
                var fbd = new FolderBrowserDialog();
                fbd.Description = this.Title;
                fbd.SelectedPath = this.InitialDirectory;
                fbd.ShowNewFolderButton = false;
                if (fbd.ShowDialog(new WindowWrapper(hWndOwner)) != DialogResult.OK) return false;
                ofd.FileName = fbd.SelectedPath;
                flag = true;
            }

            return flag;
        }

        #endregion
    }

    /// <summary>
    /// Creates IWin32Window around an IntPtr
    /// </summary>
    public class WindowWrapper : System.Windows.Forms.IWin32Window
    {
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="handle">Handle to wrap</param>
        public WindowWrapper(IntPtr handle)
        {
            _hwnd = handle;
        }

        /// <summary>
        /// Original ptr
        /// </summary>
        public IntPtr Handle
        {
            get { return _hwnd; }
        }

        private IntPtr _hwnd;
    }

    /// <summary>
    /// This class is from the Front-End for Dosbox and is used to present a 'vista' dialog box to select folders.
    /// Being able to use a vista style dialog box to select folders is much better then using the shell folder browser.
    /// http://code.google.com/p/fed/
    ///
    /// Example:
    /// var r = new Reflector("System.Windows.Forms");
    /// </summary>
    public class Reflector
    {
        #region variables

        string m_ns;
        Assembly m_asmb;

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="ns">The namespace containing types to be used</param>
        public Reflector(string ns)
            : this(ns, ns)
        { }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="an">A specific assembly name (used if the assembly name does not tie exactly with the namespace)</param>
        /// <param name="ns">The namespace containing types to be used</param>
        public Reflector(string an, string ns)
        {
            m_ns = ns;
            m_asmb = null;
            foreach (AssemblyName aN in Assembly.GetExecutingAssembly().GetReferencedAssemblies())
            {
                if (aN.FullName.StartsWith(an))
                {
                    m_asmb = Assembly.Load(aN);
                    break;
                }
            }
        }

        #endregion

        #region Methods

        /// <summary>
        /// Return a Type instance for a type 'typeName'
        /// </summary>
        /// <param name="typeName">The name of the type</param>
        /// <returns>A type instance</returns>
        public Type GetType(string typeName)
        {
            Type type = null;
            string[] names = typeName.Split('.');

            if (names.Length > 0)
                type = m_asmb.GetType(m_ns + "." + names[0]);

            for (int i = 1; i < names.Length; ++i)
            {
                type = type.GetNestedType(names[i], BindingFlags.NonPublic);
            }
            return type;
        }

        /// <summary>
        /// Create a new object of a named type passing along any params
        /// </summary>
        /// <param name="name">The name of the type to create</param>
        /// <param name="parameters"></param>
        /// <returns>An instantiated type</returns>
        public object New(string name, params object[] parameters)
        {
            Type type = GetType(name);

            ConstructorInfo[] ctorInfos = type.GetConstructors();
            foreach (ConstructorInfo ci in ctorInfos)
            {
                try
                {
                    return ci.Invoke(parameters);
                }
                catch { }
            }

            return null;
        }

        /// <summary>
        /// Calls method 'func' on object 'obj' passing parameters 'parameters'
        /// </summary>
        /// <param name="obj">The object on which to excute function 'func'</param>
        /// <param name="func">The function to execute</param>
        /// <param name="parameters">The parameters to pass to function 'func'</param>
        /// <returns>The result of the function invocation</returns>
        public object Call(object obj, string func, params object[] parameters)
        {
            return Call2(obj, func, parameters);
        }

        /// <summary>
        /// Calls method 'func' on object 'obj' passing parameters 'parameters'
        /// </summary>
        /// <param name="obj">The object on which to excute function 'func'</param>
        /// <param name="func">The function to execute</param>
        /// <param name="parameters">The parameters to pass to function 'func'</param>
        /// <returns>The result of the function invocation</returns>
        public object Call2(object obj, string func, object[] parameters)
        {
            return CallAs2(obj.GetType(), obj, func, parameters);
        }

        /// <summary>
        /// Calls method 'func' on object 'obj' which is of type 'type' passing parameters 'parameters'
        /// </summary>
        /// <param name="type">The type of 'obj'</param>
        /// <param name="obj">The object on which to excute function 'func'</param>
        /// <param name="func">The function to execute</param>
        /// <param name="parameters">The parameters to pass to function 'func'</param>
        /// <returns>The result of the function invocation</returns>
        public object CallAs(Type type, object obj, string func, params object[] parameters)
        {
            return CallAs2(type, obj, func, parameters);
        }

        /// <summary>
        /// Calls method 'func' on object 'obj' which is of type 'type' passing parameters 'parameters'
        /// </summary>
        /// <param name="type">The type of 'obj'</param>
        /// <param name="obj">The object on which to excute function 'func'</param>
        /// <param name="func">The function to execute</param>
        /// <param name="parameters">The parameters to pass to function 'func'</param>
        /// <returns>The result of the function invocation</returns>
        public object CallAs2(Type type, object obj, string func, object[] parameters)
        {
            MethodInfo methInfo = type.GetMethod(func, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
            return methInfo.Invoke(obj, parameters);
        }

        /// <summary>
        /// Returns the value of property 'prop' of object 'obj'
        /// </summary>
        /// <param name="obj">The object containing 'prop'</param>
        /// <param name="prop">The property name</param>
        /// <returns>The property value</returns>
        public object Get(object obj, string prop)
        {
            return GetAs(obj.GetType(), obj, prop);
        }

        /// <summary>
        /// Returns the value of property 'prop' of object 'obj' which has type 'type'
        /// </summary>
        /// <param name="type">The type of 'obj'</param>
        /// <param name="obj">The object containing 'prop'</param>
        /// <param name="prop">The property name</param>
        /// <returns>The property value</returns>
        public object GetAs(Type type, object obj, string prop)
        {
            PropertyInfo propInfo = type.GetProperty(prop, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
            return propInfo.GetValue(obj, null);
        }

        /// <summary>
        /// Returns an enum value
        /// </summary>
        /// <param name="typeName">The name of enum type</param>
        /// <param name="name">The name of the value</param>
        /// <returns>The enum value</returns>
        public object GetEnum(string typeName, string name)
        {
            Type type = GetType(typeName);
            FieldInfo fieldInfo = type.GetField(name);
            return fieldInfo.GetValue(null);
        }

        #endregion
    }
}
