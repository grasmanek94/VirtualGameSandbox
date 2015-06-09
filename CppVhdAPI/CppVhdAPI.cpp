#include <config.hpp>
#include <boost/algorithm/string.hpp>
#include <BoxedAppSDK.h>
#include <Shlobj.h>

#pragma comment(lib, "bxsdk32.lib")
#pragma comment(lib, "Shell32.lib")

std::wstring s2ws(const std::string& str)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}

DWORD AttachVirtualDiskEx(_In_    LPCWSTR     VirtualDiskPath,_In_	HANDLE&		vhdHandle)
{
	OPEN_VIRTUAL_DISK_PARAMETERS openParameters;
	VIRTUAL_DISK_ACCESS_MASK accessMask;
	ATTACH_VIRTUAL_DISK_PARAMETERS attachParameters;
	VIRTUAL_STORAGE_TYPE storageType;
	LPCWSTR extension;
	ATTACH_VIRTUAL_DISK_FLAG attachFlags = ATTACH_VIRTUAL_DISK_FLAG_NONE;
	DWORD opStatus;

	vhdHandle = INVALID_HANDLE_VALUE;

	storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_UNKNOWN;
	storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_UNKNOWN;

	memset(&openParameters, 0, sizeof(openParameters));

	extension = ::PathFindExtensionW(VirtualDiskPath);
	bool iso = false;

	if (extension != NULL && _wcsicmp(extension, L".iso") == 0)
	{
		iso = true;
		openParameters.Version = OPEN_VIRTUAL_DISK_VERSION_1;
		accessMask = VIRTUAL_DISK_ACCESS_READ;
		attachFlags = ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME | ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY;
	}
	else
	{
		openParameters.Version = OPEN_VIRTUAL_DISK_VERSION_1;
		openParameters.Version1.RWDepth = 1024;
		accessMask = VIRTUAL_DISK_ACCESS_ALL;
		attachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER;
	}

	opStatus = OpenVirtualDisk(&storageType,VirtualDiskPath,accessMask,OPEN_VIRTUAL_DISK_FLAG_NONE,&openParameters,&vhdHandle);

	if (opStatus != ERROR_SUCCESS)
	{
		return opStatus;
	}

	memset(&attachParameters, 0, sizeof(attachParameters));
	attachParameters.Version = ATTACH_VIRTUAL_DISK_VERSION_1;
	
	opStatus = AttachVirtualDisk(vhdHandle,NULL,attachFlags,0,&attachParameters,NULL);

	return opStatus;
}

TCHAR pressanykey(const TCHAR* prompt = NULL)
{
	TCHAR  ch;
	DWORD  mode;
	DWORD  count;
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

	// Prompt the user
	if (prompt == NULL) prompt = TEXT("Press any key to continue...");
	WriteConsole(
		GetStdHandle(STD_OUTPUT_HANDLE),
		prompt,
		lstrlen(prompt),
		&count,
		NULL
		);

	// Switch to raw mode
	GetConsoleMode(hstdin, &mode);
	SetConsoleMode(hstdin, 0);

	// Wait for the user's response
	WaitForSingleObject(hstdin, INFINITE);

	// Read the (single) key pressed
	ReadConsole(hstdin, &ch, 1, &count, NULL);

	// Restore the console to its previous state
	SetConsoleMode(hstdin, mode);

	// Return the key code
	return ch;
}

#define ARRAY_SIZE(a)                               \
	((sizeof(a) / sizeof(*(a))) /                   \
	static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

BOOL EnableDebugPrivilege()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
	{
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL))
	{
		return FALSE;
	}

	if (!CloseHandle(hToken))
	{
		return FALSE;
	}

	return TRUE;
}

#pragma warning(disable: 4996)

BOOL FileExists(LPCSTR szPath)
{
	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void CheckConfig()
{
	if (FileExists(".\\config.cfg"))
	{
		//OK
	}
	else
	{
		FILE * pFile;
		pFile = fopen(".\\config.cfg", "a+");
		if (pFile != NULL)
		{
			fwrite("{}", 1, 2, pFile);
			fclose(pFile);
		}
	}
}

BOOL IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return (ret == WAIT_TIMEOUT);
}

void TryScriptingShit();

void PrintSqs(int count)
{
	unsigned char s = 219;
	for (int i = 0; i < count; ++i)
	{
		std::cout << s;
	}
}

std::string GetConfig(const std::string& option);
_Config Config;
HANDLE  isohandle;
HANDLE  handle;

std::wstring GetVolumeByName(std::wstring name)
{
	wchar_t volumeNameBuffer[MAX_PATH];
	wchar_t volumeName2Buffer[MAX_PATH];

	HANDLE search = FindFirstVolumeW(volumeNameBuffer, ARRAY_SIZE(volumeNameBuffer));
	if (search != INVALID_HANDLE_VALUE)
	{
		do
		{
			GetVolumeInformationW(volumeNameBuffer,volumeName2Buffer,ARRAY_SIZE(volumeName2Buffer),0,0,0,0,0);
			std::wstring VolumeName = volumeName2Buffer;
			std::wstring VolumeGUID = volumeNameBuffer;
			if (!VolumeName.compare(name))
			{
				FindVolumeClose(search);
				return VolumeGUID;
			}
		} while (FindNextVolumeW(search, volumeNameBuffer, ARRAY_SIZE(volumeNameBuffer)) != FALSE);
		FindVolumeClose(search);
		return L"";
	}
	return L"";
}

//
// Simple macro to release non-null interfaces.
//
#define _SafeRelease(x) {if (NULL != x) { x->Release(); x = NULL; } }

BOOL disk_offline(HANDLE h_file, bool enable){
	DWORD bytes_returned = 0;
	BOOL b_offline = 0;
	if (h_file != INVALID_HANDLE_VALUE){
		SET_DISK_ATTRIBUTES disk_attr;
		ZeroMemory(&disk_attr, sizeof(disk_attr));
		disk_attr.Version = sizeof(SET_DISK_ATTRIBUTES);
		disk_attr.Attributes = enable ? DISK_ATTRIBUTE_OFFLINE : 0;
		disk_attr.AttributesMask = DISK_ATTRIBUTE_OFFLINE;
		b_offline = DeviceIoControl(h_file, IOCTL_DISK_SET_DISK_ATTRIBUTES, &disk_attr, disk_attr.Version, NULL, 0, &bytes_returned, NULL);
		// Invalidates the cached partition table and re-enumerates the device.
		if (!enable) BOOL b_update = DeviceIoControl(h_file, IOCTL_DISK_UPDATE_PROPERTIES, NULL, 0, NULL, 0, &bytes_returned, NULL);
	}
	return b_offline;
}

void PerformRedirectionEnv(const char* source, const char* dest)
{
	char * current = getenv(source);

	std::cout << "Redirecting<" << std::string(current) << ">=<" << std::string(dest) << ">" << std::endl;
	SetEnvironmentVariableA(source, dest);
	BoxedAppSDK_SetFileIsolationModeA(BxIsolationMode_WriteCopy, current, dest);
}

int RunThisProgram()
{
	std::cout << "GameLauncher by Rafal 'Gamer_Z' Grasman" << std::endl;
	std::cout << "See config.cfg for more info" << std::endl;
	std::cout << "Please wait while preparing the game for start..." << std::endl;

	std::cout << "Fixing debug privlidges..." << std::flush;
	EnableDebugPrivilege();
	std::cout << "OK" << std::endl;

	std::cout << "Loading configuration..." << std::flush;

	Config.Load();

	std::cout << "OK" << std::endl;

	std::cout << "Checking for free mountpoint..." << std::flush;
	if (GetDriveTypeA(Config.MountPoint.c_str()) != 1 && GetDriveTypeA(Config.MountPoint.c_str()) != 0)
	{
		std::cout << Config.MountPoint << " already exists? Cannot use drive letter as mount point." << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}
	std::cout << "OK" << std::endl;

	std::cout << "Opening Game Disk..." << std::flush;

	DWORD   result;
	//ULONG   bytesUsed;

	//
	if (AttachVirtualDiskEx(s2ws(Config.VHD_location).c_str(), handle) != ERROR_SUCCESS)
	{
		std::cout << "Unable to open virtual disk '" << Config.VHD_location << "'" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}

	std::cout << "OK" << std::endl;
	std::cout << "Configuring path information..." << std::flush;

	// Now we need to grab the device name \\.\PhysicalDrive#
	wchar_t  physicalDriveName[MAX_PATH];
	DWORD   physicalDriveNameSize = ARRAY_SIZE(physicalDriveName);


	result = GetVirtualDiskPhysicalPath(handle, &physicalDriveNameSize, physicalDriveName);
	if (result != ERROR_SUCCESS)
	{
		std::wcout << "Unable to retrieve virtual disk path" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}
	std::cout << "OK" << std::endl;

	std::cout << "Onlineing VDISK..." << std::flush;
	{
		std::string PhysicalPath = (ws2s(std::wstring(physicalDriveName)));
		PhysicalPath.erase(PhysicalPath.begin(), PhysicalPath.begin() + strlen("\\\\.\\PhysicalDrive"));
		std::cout << "Drive : " << PhysicalPath << std::endl;
		{
			DWORD dwFlags = CREATE_SUSPENDED | INHERIT_PARENT_AFFINITY;
			STARTUPINFO si;
			PROCESS_INFORMATION pi = PROCESS_INFORMATION();
			BOOL fSuccess;
			SecureZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);

			std::string sysDir(getenv("SystemRoot"));
			std::cout << "Starting mount..." << std::flush;
			fSuccess = CreateProcess((sysDir + "\\system32\\WindowsPowerShell\\v1.0\\powershell.exe").c_str(), (LPSTR)(("\"" + sysDir + "\\system32\\WindowsPowerShell\\v1.0\\powershell.exe\" ") + "-Command \"initialize-disk " + PhysicalPath + " *>$null\"").c_str(), NULL, NULL, TRUE, dwFlags, NULL, NULL, &si, &pi);
			if (!fSuccess)
			{
				DWORD error = GetLastError();
				std::cout << "Unable to summon powershell: " << error << std::endl;
				return (int)pressanykey("Hit any key to quit the application\r\n");
			}
			std::cout << "Executing mount..." << std::endl;
			ResumeThread(pi.hThread);
			WaitForSingleObject(pi.hProcess, INFINITE);
			std::cout << "Executed" << std::endl;
		}
	}

	std::cout << "Configuring volume information..." << std::flush;

	std::wstring volumeName = GetVolumeByName(s2ws(Config.VolumeNameToMount));
	if (volumeName.size() == 0)
	{
		int i = 0;
		while (++i < 10)
		{
			volumeName = GetVolumeByName(s2ws(Config.VolumeNameToMount));
			if (volumeName.size() <= 6)
			{
				Sleep(25);
			}
			else
			{
				break;
			}
		}
	}

	if (volumeName.size() <= 6)
	{
		std::wcout << "Unable to locate a volume on this device" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}
	std::cout << "OK" << std::endl;
	
	std::cout << "Mounting to configured mountpoint..." << std::flush;
	if (!SetVolumeMountPointW(s2ws(Config.MountPoint).c_str(), volumeName.c_str()))
	{
		std::cout << "Unable to mount VHD to " << Config.MountPoint << " (" << GetLastError() << ")" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}

	UINT dtype = GetDriveTypeA(Config.MountPoint.c_str());
	while (dtype == 1 || dtype == 0)
	{
		dtype = GetDriveTypeA(Config.MountPoint.c_str());
		Sleep(10);
	}
	std::cout << "OK" << std::endl;

	
	if (FileExists(Config.ISO_location.c_str()))
	{
		std::cout << "Mounting ISO... " << std::flush;
		if (AttachVirtualDiskEx(s2ws(Config.ISO_location).c_str(), isohandle) == ERROR_SUCCESS)
		{
			std::cout << "ISO mounted.\r\n";
		}
		else
		{
			DWORD errorcode = GetLastError();
			std::cout << "ISO NOT mounted (" << errorcode << ").\r\n";
		}
	}

	std::cout << "Updating environment to configured variables...\r\n" << std::flush;

	std::string MountLetter = Config.MountPoint.substr(0, 1);

	BoxedAppSDK_Init();

	//BoxedAppSDK_SetLogFileW(L"boxedapp.log");
	//BoxedAppSDK_EnableDebugLog(TRUE);

	//BoxedAppSDK_SetPersistentRegistryPathA("reg.dat");

	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__EMBED_BOXEDAPP_IN_CHILD_PROCESSES, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_FILE_SYSTEM, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_REGISTRY, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__INHERIT_OPTIONS, TRUE);
	//BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__RECREATE_VIRTUAL_FILE_AS_VIRTUAL, TRUE);

	/*
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x86\\CR\\", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x86\\CU\\", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x86\\LM\\", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"HKEY_CURRENT_USER\\VirtualGameSandbox\\x86\\US\\", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"HKEY_CURRENT_USER\\VirtualGameSandbox\\x86\\CC\\", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);

	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x64\\CR\\", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x64\\CU\\", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE,		"HKEY_CURRENT_USER\\VirtualGameSandbox\\x64\\LM\\", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"HKEY_CURRENT_USER\\VirtualGameSandbox\\x64\\US\\", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"HKEY_CURRENT_USER\\VirtualGameSandbox\\x64\\CC\\", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	*/

	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Electronic Arts", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Electronic Arts", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\Electronic Arts", NULL, BxIsolationMode_WriteCopy);


	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);

	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	//BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);

	SetEnvironmentVariable("LOGONSERVER", "\\\\User");
	SetEnvironmentVariable("USERDOMAIN", "User");
	SetEnvironmentVariable("USERDOMAIN_ROAMINGPROFILE", "User");
	SetEnvironmentVariable("COMPUTERNAME", "User");
	SetEnvironmentVariable("USERNAME", "User");
	SetEnvironmentVariable("HOMEPATH", "\\User");


	PerformRedirectionEnv("USERPROFILE", (MountLetter + ":\\User").c_str());
	PerformRedirectionEnv("PUBLIC", (MountLetter + ":\\User").c_str());
	PerformRedirectionEnv("APPDATA", (MountLetter + ":\\User\\AppData").c_str());
	PerformRedirectionEnv("LOCALAPPDATA", (MountLetter + ":\\User\\AppData\\Local").c_str());
	PerformRedirectionEnv("ALLUSERSPROFILE", (MountLetter + ":\\User\\ProgramData").c_str());
	PerformRedirectionEnv("ProgramData", (MountLetter + ":\\User\\ProgramData").c_str());
	PerformRedirectionEnv("ProgramFiles", (MountLetter + ":\\User\\ProgramFiles\\x64").c_str());
	PerformRedirectionEnv("ProgramFiles(x86)", (MountLetter + ":\\User\\ProgramFiles\\x86").c_str());
	PerformRedirectionEnv("ProgramW6432", (MountLetter + ":\\User\\ProgramFiles\\x64").c_str());
	PerformRedirectionEnv("CommonProgramFiles", (MountLetter + ":\\User\\ProgramFiles\\Common\\x64").c_str());
	PerformRedirectionEnv("CommonProgramFiles(x86)", (MountLetter + ":\\User\\ProgramFiles\\Common\\x86").c_str());
	PerformRedirectionEnv("CommonProgramW6432", (MountLetter + ":\\User\\ProgramFiles\\Common\\x64").c_str());
	//PerformRedirectionEnv("SystemRoot", (MountLetter + ":\\User\\Windows").c_str());
	//PerformRedirectionEnv("windir", (MountLetter + ":\\User\\Windows").c_str());
	//PerformRedirectionEnv("SystemDrive", (MountLetter + ":").c_str());
	//PerformRedirectionEnv("HOMEDRIVE", (MountLetter + ":").c_str());

	std::cout << "OK" << std::endl;

	if (FileExists(Config.Exec_Script.c_str()))
	{
		std::cout << "Executing setup script..." << std::endl;
		PrintSqs(32); 
		std::cout << " SCRIPT  OUTPUT "; 
		PrintSqs(112);
		std::cout << std::endl;
		TryScriptingShit(); 
		std::cout << std::endl;
		PrintSqs(160);
		std::cout << std::endl;
	}
	else
	{
		std::cout << "No script available, continuing" << std::endl;
	}

	if (Config.ApplicationAutoLaunch.size() <= 2)
	{
		pressanykey("Press any key to launch the game...\r\n");
	}
	else
	{
		std::cout << "Automatically launching game..." << std::endl;
	}
	//mounting VHD done, now prepare game stuff

	//
	std::cout << "Preparing process startup..." << std::flush;
	bool r = true;
	while (r)
	{
		DWORD dwFlags = CREATE_SUSPENDED | INHERIT_PARENT_AFFINITY;
		STARTUPINFO si;
		PROCESS_INFORMATION pi = PROCESS_INFORMATION();
		BOOL fSuccess;
		std::cout << "OK" << std::endl;

		if (Config.UseShellExecute.compare("force") != 0)
		{
			// Create the child process, specifying a new environment block. 
			SecureZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);

			std::cout << "Starting process..." << std::flush;
			fSuccess = CreateProcess(Config.ApplicationProcess.c_str(), (LPSTR)("\"" + Config.ApplicationProcess + "\" " + Config.ApplicationParameters).c_str(), NULL, NULL, TRUE, dwFlags, NULL, Config.ApplicationWorkingDir.c_str(), &si, &pi);
			if (!fSuccess)
			{
				std::cout << "Unable to launch process " << Config.ApplicationProcess.c_str() << " (" << GetLastError() << ")" << std::endl;
				return (int)pressanykey("Hit any key to quit the application\r\n");
			}
			ResumeThread(pi.hThread);
			std::cout << "OK" << std::endl;
		}
		else if (!Config.UseShellExecute.compare("true") || !Config.UseShellExecute.compare("force"))
		{
			HANDLE hInstance = INVALID_HANDLE_VALUE;
			Sleep(1000);
			if (!IsProcessRunning(pi.dwProcessId))
			{
				std::cout << "Trying shellexecute..." << std::flush;
				CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
				hInstance = ShellExecute(NULL, "open", Config.ApplicationProcess.c_str(), (LPSTR)Config.ApplicationParameters.c_str(), Config.ApplicationWorkingDir.c_str(), SW_SHOWNORMAL);
				if (hInstance > (HANDLE)32)
				{
					std::cout << "OK" << std::endl;
				}
				else
				{
					std::cout << "Unable to launch process " << Config.ApplicationProcess.c_str() << " (" << GetLastError() << ")" << std::endl;
					return (int)pressanykey("Hit any key to quit the application\r\n");
				}
			}
		}

		int ret = pressanykey("Press any key when done playing to close this window (will detach the Virtual Hard Drive too!)\r\n");
		r = ret == 'R' || ret == 'r';
		while (IsProcessRunning(pi.dwProcessId))
		{
			ret = pressanykey("Game is still running.\r\n");
			r = ret == 'R' || ret == 'r';
		}
	}
	DetachVirtualDisk(isohandle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	DetachVirtualDisk(handle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	if (isohandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(isohandle);
	}
	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
	}
	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	DetachVirtualDisk(isohandle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	DetachVirtualDisk(handle, DETACH_VIRTUAL_DISK_FLAG_NONE, 0);
	if (isohandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(isohandle);
	}
	if (handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
	}
	return 1;
}

int main(int argc, char *argv[])
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof (statex);

	GlobalMemoryStatusEx(&statex);

	srand((unsigned int)(((DWORDLONG)time(NULL)) + ((DWORDLONG)statex.dwMemoryLoad) + statex.ullTotalVirtual + statex.ullTotalPhys + statex.ullTotalPageFile + statex.ullAvailExtendedVirtual + statex.ullAvailPageFile + statex.ullAvailPhys + statex.ullAvailVirtual));

	SetConsoleCtrlHandler(HandlerRoutine,TRUE);

	return RunThisProgram();
}
