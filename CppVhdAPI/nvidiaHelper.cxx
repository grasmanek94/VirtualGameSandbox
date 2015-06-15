#include "Nvidia/nvOptimusTool_x86.h"
#include "Nvidia/nvOptimusTool_x64.h"

#include "BoxedAppSDK/BoxedAppSDK.h"

#include <windows.h>

void LaunchNvidiaOptimusTool(void)
{
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processinformation = PROCESS_INFORMATION();

	SecureZeroMemory(&startupinfo, sizeof(startupinfo));
	startupinfo.cb = sizeof(STARTUPINFO);

#ifndef _WIN64			
	BOOL isWow64Process = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64Process);

	if (!isWow64Process)
	{
		BoxedAppSDK_CreateProcessFromMemoryA(nvOptimusTool_x86, sizeof(nvOptimusTool_x86), "NvOptimusTestViewer.exe", "", NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &processinformation);
	}
	else
#endif
	{
		BoxedAppSDK_CreateProcessFromMemoryA(nvOptimusTool_x64, sizeof(nvOptimusTool_x64), "NvOptimusTestViewer64.exe", "", NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &processinformation);
	}
}