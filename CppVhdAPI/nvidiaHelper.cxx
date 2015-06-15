#include "Nvidia/nvOptimusTool_x86.h"
#include "Nvidia/nvOptimusTool_x64.h"

#include "file_functions.hxx"

#include "BoxedAppSDK/BoxedAppSDK.h"

#include <windows.h>

#include <string>
#include <fstream>

void LaunchNvidiaOptimusTool(void)
{
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processinformation = PROCESS_INFORMATION();

	SecureZeroMemory(&startupinfo, sizeof(startupinfo));
	startupinfo.cb = sizeof(STARTUPINFO);

	std::string tmp;
	tmp.reserve(MAX_PATH);

	GetEnvironmentVariableA("TEMP", &tmp[0], MAX_PATH);

	unsigned char* application = nullptr;
	size_t size = 0;

#ifndef _WIN64			
	BOOL isWow64Process = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64Process);

	if (!isWow64Process)
	{
		application = nvOptimusTool_x86;
		size = sizeof(nvOptimusTool_x86);
		tmp += "\\NvOptimusTestViewer.exe";
	}
	else
#endif
	{
		application = nvOptimusTool_x64;
		size = sizeof(nvOptimusTool_x64);
		tmp += "\\NvOptimusTestViewer64.exe";
	}

	if (!FileExists(tmp.c_str()))
	{
		std::ofstream outfile(tmp, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
		if (outfile.good())
		{
			outfile.write((const char*)application, size);
			outfile.close();
		}
	}
	CreateProcessA(tmp.c_str(), "", NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &processinformation);
}