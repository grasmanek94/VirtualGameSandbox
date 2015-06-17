#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <Shlobj.h>

#include <boost/algorithm/string.hpp>

#include "globalvars.hxx"
#include "file_functions.hxx"

#include "BoxedAppSDK/bxsdk32.h"
#include "BoxedAppSDK/bxsdk64.h"

#include "BoxedAppSDK/BoxedAppSDK.h"

#include "virtualization_helper.hxx"

void BoxedAppDLLChecks(void)
{

#if 1
	CHAR syspath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, 0, syspath);
	std::string path_bxsdk32dll(syspath);
	std::string path_bxsdk64dll(syspath);
	path_bxsdk32dll += "\\bxsdk32.dll";
	path_bxsdk64dll += "\\bxsdk64.dll";

	if (!FileExists(path_bxsdk32dll.c_str()))
	{
		std::ofstream outfile(path_bxsdk32dll, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
		if (outfile.good())
		{
			outfile.write((const char*)bxsdk32dll, sizeof(bxsdk32dll));
			outfile.close();
		}

		PVOID oldValue;
		if (Wow64DisableWow64FsRedirection(&oldValue) != 0)
		{
			CopyFileA(boost::ireplace_all_copy(path_bxsdk32dll, "System32", "SysWOW64").c_str(), path_bxsdk32dll.c_str(), TRUE);
			Wow64RevertWow64FsRedirection(oldValue);
		}
	}

	if (!FileExists(path_bxsdk64dll.c_str()))
	{
		std::ofstream outfile(path_bxsdk64dll, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
		if (outfile.good())
		{
			outfile.write((const char*)bxsdk64dll, sizeof(bxsdk64dll));
			outfile.close();
		}
		PVOID oldValue;
		if (Wow64DisableWow64FsRedirection(&oldValue) != 0)
		{
			CopyFileA(boost::ireplace_all_copy(path_bxsdk64dll, "System32", "SysWOW64").c_str(), path_bxsdk64dll.c_str(), TRUE);
			Wow64RevertWow64FsRedirection(oldValue);
		}
	}
#endif

#ifdef _WIN64
	LoadLibraryA("bxsdk64.dll");
#else
	LoadLibraryA("bxsdk32.dll");
#endif
}

void PerformRedirectionEnv(const char* source, const char* dest)
{
	char * current = getenv(source);

	std::cout << "RD<" << std::string(current) << ">=<" << std::string(dest) << ">" << std::endl;
	SetEnvironmentVariableA(source, dest);
	BoxedAppSDK_SetFileIsolationModeA(BxIsolationMode_WriteCopy, current, dest);
}

std::string GetEnvString(std::string var)
{
	return std::string(getenv(var.c_str()));
}

void ConfigureBoxedAppSDK()
{
	std::cout << "Installing BoxedAppSDK libraries..." << std::flush;

	BoxedAppDLLChecks();

	std::cout << "OK\r\n" << std::flush;

	//#define REGEMU
#ifdef REGEMU
	std::cout << "Setting BoxedAppSDK Registry Path..." << std::flush;

	BoxedAppSDK_SetPersistentRegistryPathW(
		s2ws(VHD_location + ".registry").c_str()
		//s2ws(MountLetter + ":\\User\\ProgramData\\registry.dat").c_str()
		);

	std::cout << "OK\r\n" << std::flush;
#endif

	std::cout << "Initialising BoxedAppSDK..." << std::flush;

	BoxedAppSDK_Init();

	std::cout << "OK\r\n" << std::flush;
	std::cout << "Setting BoxedAppSDK options..." << std::flush;

	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__EMBED_BOXEDAPP_IN_CHILD_PROCESSES, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__INHERIT_OPTIONS, TRUE);

#ifdef BXSDK_DEBUG
	BoxedAppSDK_SetLogFileW(L"boxedapp.log");

	BoxedAppSDK_EnableDebugLog(TRUE);
#endif

	std::cout << "OK\r\n" << std::flush;
	std::cout << "================ Updating environment to configured variables... ===============" << std::flush;

#ifdef REGEMU
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT, "", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER, "", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE, "", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS, "", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG, "", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT, "", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER, "", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE, "", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS, "", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG, "", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);

	HKEY handle = NULL;

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CLASSES_ROOT, L"", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"AppEvents", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Console", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Control Panel", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Environment", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"EUDC", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Kayboard Layout", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Network", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Printers", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Software", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Software\\Wow6432Node", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"System", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Uninstall", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"Volatile Environment", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_USER, L"WXP", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"BCD00000000", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"HARDWARE", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"SAM", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"SECURITY", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_USERS, L"", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_USERS, L".DEFAULT", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_CONFIG, L"", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_CONFIG, L"Software", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CURRENT_CONFIG, L"System", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)) { RegCloseKey(handle); }
#endif

	SetEnvironmentVariable("LOGONSERVER", "\\\\User");
	SetEnvironmentVariable("USERDOMAIN", "User");
	SetEnvironmentVariable("USERDOMAIN_ROAMINGPROFILE", "User");
	SetEnvironmentVariable("COMPUTERNAME", "User");
	SetEnvironmentVariable("USERNAME", "User");
	SetEnvironmentVariable("HOMEPATH", "\\User");
	
	////NVIDIA Optimnus fix --START--
	typedef std::pair<std::string, std::string> redirection;
	std::vector<redirection> NvidiaOptimusFix = 
	{
		redirection(MountLetter + ":\\User\\ProgramData\\NVIDIA Corporation\\",							GetEnvString("ProgramData") + "\\NVIDIA Corporation\\"),
		redirection(MountLetter + ":\\User\\ProgramFiles\\x86\\NVIDIA Corporation\\",					GetEnvString("ProgramFiles(x86)") + "\\NVIDIA Corporation\\"),
		redirection(MountLetter + ":\\User\\ProgramFiles\\x64\\NVIDIA Corporation\\",					GetEnvString("ProgramFiles") + "\\NVIDIA Corporation\\"),
		redirection(MountLetter + ":\\User\\ProgramData\\NVIDIA\\",										GetEnvString("ProgramData") + "\\NVIDIA\\"),
		redirection(MountLetter + ":\\User\\ProgramFiles\\x86\\NVIDIA Corporation\\coprocmanager\\",	GetEnvString("ProgramFiles(x86)") + "\\NVIDIA Corporation\\coprocmanager\\"),
		redirection(MountLetter + ":\\User\\ProgramFiles\\x64\\NVIDIA Corporation\\coprocmanager\\",	GetEnvString("ProgramFiles") + "\\NVIDIA Corporation\\coprocmanager\\"),
	};

	for (auto i : NvidiaOptimusFix)
	{
		//it seems a copy of these files is somehow needed for Trackmania because.. boxedapp + trackmania == ????
		std::string command(
			"robocopy \"" +
			boost::replace_all_copy(boost::replace_all_copy(i.second, "\"", ""), "\\", "/") + "\" \"" +
			boost::replace_all_copy(boost::replace_all_copy(i.first, "\"", ""), "\\", "/") +
			"\" /E /B /COPYALL /W:0 /R:0 > NUL");
		//std::cout << command << std::endl;
		system(command.c_str());
		//well at least the issues are fixed now too!

		BoxedAppSDK_SetFileIsolationModeA(BxIsolationMode_Full, i.first.c_str(), i.second.c_str());
		BoxedAppSDK_SetFileIsolationModeA(BxIsolationMode_Full, i.second.c_str(), i.first.c_str());
	}

	SetEnvironmentVariable("SESSIONNAME", "Console");
	SetEnvironmentVariable("SHIM_MCCOMPAT", "0x810000001");
	SetEnvironmentVariable("NVIDIAWHITELISTED", "0x01");
	////NVIDIA Optimnus fix ---END---

	//redirection
	PerformRedirectionEnv("USERPROFILE", (MountLetter + ":\\User").c_str());
	PerformRedirectionEnv("PUBLIC", (MountLetter + ":\\User").c_str());
	PerformRedirectionEnv("APPDATA", (MountLetter + ":\\User\\AppData\\Roaming").c_str());
	PerformRedirectionEnv("LOCALAPPDATA", (MountLetter + ":\\User\\AppData\\Local").c_str());
	PerformRedirectionEnv("ALLUSERSPROFILE", (MountLetter + ":\\User\\ProgramData").c_str());
	PerformRedirectionEnv("ProgramData", (MountLetter + ":\\User\\ProgramData").c_str());
	PerformRedirectionEnv("ProgramFiles", (MountLetter + ":\\User\\ProgramFiles\\x64").c_str());
	PerformRedirectionEnv("ProgramFiles(x86)", (MountLetter + ":\\User\\ProgramFiles\\x86").c_str());
	PerformRedirectionEnv("ProgramW6432", (MountLetter + ":\\User\\ProgramFiles\\x64").c_str());
	PerformRedirectionEnv("CommonProgramFiles", (MountLetter + ":\\User\\ProgramFiles\\Common\\x64").c_str());
	PerformRedirectionEnv("CommonProgramFiles(x86)", (MountLetter + ":\\User\\ProgramFiles\\Common\\x86").c_str());
	PerformRedirectionEnv("CommonProgramW6432", (MountLetter + ":\\User\\ProgramFiles\\Common\\x64").c_str());

#ifdef REGEMU
	registrydathandle = GetRegistryFileHandle();
	if (registrydathandle != INVALID_HANDLE_VALUE)
	{
		std::cout << " (RegData handle: " << (unsigned long)registrydathandle << ") ..." << std::flush;
	}
#endif

	std::cout << "===================================== OK =======================================" << std::flush;
}