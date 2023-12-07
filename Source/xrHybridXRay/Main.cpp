#include "..\XrEngine\stdafx.h"

ENGINE_API int EngineLaunch(EGamePath Game);
int APIENTRY   WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    EngineLaunch(EGamePath::NONE);
    return (0);
}
