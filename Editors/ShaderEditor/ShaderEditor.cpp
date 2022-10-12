// ShaderEditor.cpp : ���������� ����� ����� ��� ����������.

#include "stdafx.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if (!IsDebuggerPresent())
        Debug._initialize(false);
    Core._initialize("shader", ELogCallback, 1, "fs.ltx", true);

    STools = xr_new<CShaderTool>();
    Tools = STools;

    UI = xr_new<CShaderMain>();
    UI->RegisterCommands();

    UIMainForm *MainForm = xr_new<UIMainForm>();
    ::MainForm = MainForm;
    UI->Push(MainForm, false);

    while (MainForm->Frame())
    {
    }
    
    xr_delete(MainForm);
    Core._destroy();
    return 0;
}
