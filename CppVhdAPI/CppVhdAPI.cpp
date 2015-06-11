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

HRESULT ModifyPrivilege(
	IN LPCTSTR szPrivilege,
	IN BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken = NULL;

	// Open the process token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("Failed OpenProcessToken\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique ID for the privilege.
	if (!LookupPrivilegeValue(NULL,
		szPrivilege,
		&luid))
	{
		CloseHandle(hToken);
		printf("Failed LookupPrivilegeValue\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Assign values to the TOKEN_PRIVILEGE structure.
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes =
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	// Adjust the token privilege.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		0,
		NULL,
		NULL))
	{
		printf("Failed AdjustTokenPrivileges\n");
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}

BOOL EnableDebugPrivilege()
{
	return ModifyPrivilege(SE_DEBUG_NAME, TRUE) != ERROR_FUNCTION_FAILED;
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

void TryScriptingShit(const std::string& script);

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

std::wstring GetFirstVolumeByName(int physicalDriveNumber)
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

			VOLUME_DISK_EXTENTS extends;
			SecureZeroMemory(&extends, sizeof(extends));
			DWORD bytesReturned = 0;
			OVERLAPPED overlapped;
			SecureZeroMemory(&overlapped, sizeof(overlapped));

			std::wstring forCreateFile(VolumeGUID);
			if (forCreateFile[forCreateFile.length() - 1] == L'\\')
			{
				forCreateFile = forCreateFile.erase(forCreateFile.length() - 1);
			}
			HANDLE device = CreateFileW(forCreateFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			int error = GetLastError();
			if (device != INVALID_HANDLE_VALUE && !error)
			{
				if (DeviceIoControl((HANDLE)device,			// handle to device
					IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,	// dwIoControlCode
					NULL,									// lpInBuffer
					0,										// nInBufferSize
					(LPVOID)&extends,						// output buffer
					(DWORD)sizeof(extends),					// size of output buffer
					(LPDWORD)&bytesReturned,				// number of bytes returned
					(LPOVERLAPPED)&overlapped				// OVERLAPPED structure
					))
				{
					if (extends.Extents[0].DiskNumber == physicalDriveNumber)
					{
						CloseHandle(device);
						FindVolumeClose(search);
						return VolumeGUID;
					}
				}
				CloseHandle(device);
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

BOOL disk_offline(HANDLE h_file, bool enable)
{
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

std::string MountPoint(" :\\");

//////////////////////////////////
#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);
typedef NTSTATUS(NTAPI *_NtDuplicateObject)(
	HANDLE SourceProcessHandle,
	HANDLE SourceHandle,
	HANDLE TargetProcessHandle,
	PHANDLE TargetHandle,
	ACCESS_MASK DesiredAccess,
	ULONG Attributes,
	ULONG Options
	);
typedef NTSTATUS(NTAPI *_NtQueryObject)(
	HANDLE ObjectHandle,
	ULONG ObjectInformationClass,
	PVOID ObjectInformation,
	ULONG ObjectInformationLength,
	PULONG ReturnLength
	);

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef enum _POOL_TYPE
{
	NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed,
	DontUseThisType,
	NonPagedPoolCacheAligned,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS
} POOL_TYPE, *PPOOL_TYPE;

typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING Name;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccess;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	USHORT MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG PagedPoolUsage;
	ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

PVOID GetLibraryProcAddress(PSTR LibraryName, PSTR ProcName)
{
	return GetProcAddress(GetModuleHandleA(LibraryName), ProcName);
}

HANDLE GetRegistryFileHandle(void)
{
	_NtQuerySystemInformation NtQuerySystemInformation =
		(_NtQuerySystemInformation)GetLibraryProcAddress("ntdll.dll", "NtQuerySystemInformation");
	_NtDuplicateObject NtDuplicateObject =
		(_NtDuplicateObject)GetLibraryProcAddress("ntdll.dll", "NtDuplicateObject");
	_NtQueryObject NtQueryObject =
		(_NtQueryObject)GetLibraryProcAddress("ntdll.dll", "NtQueryObject");
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION handleInfo;
	ULONG handleInfoSize = 0x10000;
	ULONG pid;
	HANDLE processHandle;
	ULONG i;

	HANDLE toreturn = INVALID_HANDLE_VALUE;

	pid = GetCurrentProcessId();

	if (processHandle = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pid))
	{
		handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);

		/* NtQuerySystemInformation won't give us the correct buffer size,
		so we guess by doubling the buffer size. */
		while ((status = NtQuerySystemInformation(
			SystemHandleInformation,
			handleInfo,
			handleInfoSize,
			NULL
			)) == STATUS_INFO_LENGTH_MISMATCH)
			handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);

		/* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
		if (!NT_SUCCESS(status))
		{
			//printf("NtQuerySystemInformation failed!\n");
			return toreturn;
		}

		for (i = 0; i < handleInfo->HandleCount; i++)
		{
			SYSTEM_HANDLE handle = handleInfo->Handles[i];
			HANDLE dupHandle = NULL;
			POBJECT_TYPE_INFORMATION objectTypeInfo;
			PVOID objectNameInfo;
			UNICODE_STRING objectName;
			ULONG returnLength;

			/* Check if this handle belongs to the PID the user specified. */
			if (handle.ProcessId != pid)
				continue;

			/* Duplicate the handle so we can query it. */
			if (!NT_SUCCESS(NtDuplicateObject(
				processHandle,
				(HANDLE)handle.Handle,
				GetCurrentProcess(),
				&dupHandle,
				0,
				0,
				0
				)))
			{
				//printf("[%#x] Error!\n", handle.Handle);
				continue;
			}

			/* Query the object type. */
			objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x1000);
			if (!NT_SUCCESS(NtQueryObject(
				dupHandle,
				ObjectTypeInformation,
				objectTypeInfo,
				0x1000,
				NULL
				)))
			{
				//printf("[%#x] Error!\n", handle.Handle);
				CloseHandle(dupHandle);
				continue;
			}

			/* Query the object name (unless it has an access of
			0x0012019f, on which NtQueryObject could hang. */
			if (handle.GrantedAccess == 0x0012019f)
			{
				/* We have the type, so display that. */
				//printf(
				//	"[%#x] %.*S: (did not get name)\n",
				//	handle.Handle,
				//	objectTypeInfo->Name.Length / 2,
				//	objectTypeInfo->Name.Buffer
				//	);
				free(objectTypeInfo);
				CloseHandle(dupHandle);
				continue;
			}

			objectNameInfo = malloc(0x1000);
			if (!NT_SUCCESS(NtQueryObject(
				dupHandle,
				ObjectNameInformation,
				objectNameInfo,
				0x1000,
				&returnLength
				)))
			{
				/* Reallocate the buffer and try again. */
				objectNameInfo = realloc(objectNameInfo, returnLength);
				if (!NT_SUCCESS(NtQueryObject(
					dupHandle,
					ObjectNameInformation,
					objectNameInfo,
					returnLength,
					NULL
					)))
				{
					/* We have the type name, so just display that. */
					//printf(
					//	"[%#x] %.*S: (could not get name)\n",
					//	handle.Handle,
					//	objectTypeInfo->Name.Length / 2,
					//	objectTypeInfo->Name.Buffer
					//	);
					free(objectTypeInfo);
					free(objectNameInfo);
					CloseHandle(dupHandle);
					continue;
				}
			}

			/* Cast our buffer into an UNICODE_STRING. */
			objectName = *(PUNICODE_STRING)objectNameInfo;

			/* Print the information! */
			if (objectName.Length)
			{
				/* The object has a name. */
				if (std::wstring(objectName.Buffer).find(L"\\User\\ProgramData\\registry.dat") != std::wstring::npos)
				{
					toreturn = (HANDLE)handle.Handle;
					free(objectTypeInfo);
					free(objectNameInfo);
					CloseHandle(dupHandle);
					break;
				}
				//printf(
				//	"[%#x] %.*S: %.*S\n",
				//	handle.Handle,
				//	objectTypeInfo->Name.Length / 2,
				//	objectTypeInfo->Name.Buffer,
				//	objectName.Length / 2,
				//	objectName.Buffer
				//	);
			}
			else
			{
				/* Print something else. */
				//printf(
				//	"[%#x] %.*S: (unnamed)\n",
				//	handle.Handle,
				//	objectTypeInfo->Name.Length / 2,
				//	objectTypeInfo->Name.Buffer
				//	);
			}

			free(objectTypeInfo);
			free(objectNameInfo);
			CloseHandle(dupHandle);
		}

		free(handleInfo);
		CloseHandle(processHandle);
	}

	return toreturn;
}

HANDLE registrydathandle;

std::string GetFirstAvailableVHD()
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	hFind = FindFirstFileA("*.vhd", &fileInfo);

	if (hFind == INVALID_HANDLE_VALUE) 
	{ 
		return ""; 
	}

	do
	{
		std::string file(fileInfo.cFileName);
		std::reverse(file.begin(), file.end());
		std::transform(file.begin(), file.end(), file.begin(), ::toupper);
		if (file.size() > 3 && file.substr(0, 4).compare("DHV.") == 0)//.VHD
		{
			return std::string(fileInfo.cFileName);
		}
	} 
	while (FindNextFileA(hFind, &fileInfo) != 0);

	return "";
}


//////////////////////////////////

int RunThisProgram()
{
	std::cout << "GameLauncher by Rafal 'Gamer_Z' Grasman" << std::endl;
	std::cout << "See config.cfg for more info" << std::endl;
	std::cout << "Please wait while preparing the game for start..." << std::endl;

	std::cout << "Fixing debug privlidges..." << std::flush;
	BOOL edp_a = ModifyPrivilege(SE_BACKUP_NAME, TRUE) != ERROR_FUNCTION_FAILED;
	BOOL edp_b = ModifyPrivilege(SE_RESTORE_NAME, TRUE) != ERROR_FUNCTION_FAILED;
	BOOL edp_c = ModifyPrivilege(SE_DEBUG_NAME, TRUE) != ERROR_FUNCTION_FAILED;

	if (edp_a && edp_b && edp_c)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "FAIL" << std::endl;
	}

	std::cout << "Loading configuration..." << std::flush;

	Config.Load();

	std::cout << "OK" << std::endl;

	std::cout << "Checking for free mountpoint..." << std::flush;

	char p = 'Z';
	bool availablemountpoint = false;
	for (; p >= 'A'; --p)
	{
		MountPoint[0] = p;
		if (!(GetDriveTypeA(MountPoint.c_str()) != 1 && GetDriveTypeA(MountPoint.c_str()) != 0))
		{
			availablemountpoint = true;
			break;
		}
	}

	if (availablemountpoint == false)
	{
		std::cout << " No available mountpoints. " << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}

	std::cout << " Using ('" << MountPoint << "')... OK" << std::endl;

	std::cout << "Opening Game Disk..." << std::flush;

	DWORD   result;
	//ULONG   bytesUsed;
	std::string VHD_location = "./" + GetFirstAvailableVHD();

	std::cout << " (" << VHD_location << ") " << std::flush;
	//
	if (AttachVirtualDiskEx(s2ws(VHD_location).c_str(), handle) != ERROR_SUCCESS)
	{
		std::cout << "Unable to open virtual disk '" << VHD_location << "'" << std::endl;
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
	std::string PhysicalPath = (ws2s(std::wstring(physicalDriveName)));
	{		
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

	int physicalDriveNumber = std::atoi(PhysicalPath.c_str());
	std::wstring volumeName = GetFirstVolumeByName(physicalDriveNumber);
	if (volumeName.size() <= 0 || volumeName.size() > 256)
	{
		int i = 0;
		while (++i < 40)
		{
			volumeName = GetFirstVolumeByName(physicalDriveNumber);
			if (volumeName.size() <= 0 || volumeName.size() > 256)
			{
				Sleep(25);
			}
			else
			{
				break;
			}
		}
	}

	if (volumeName.size() <= 0 || volumeName.size() > 256)
	{
		std::wcout << "Unable to locate a volume ('" << volumeName << "') on this VHD" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}
	std::cout << "OK" << std::endl;
	
	std::cout << "Mounting to configured mountpoint..." << std::flush;
	if (!SetVolumeMountPointW(s2ws(MountPoint).c_str(), volumeName.c_str()))
	{
		std::cout << "Unable to mount VHD to " << MountPoint << " (" << GetLastError() << ")" << std::endl;
		return (int)pressanykey("Hit any key to quit the application\r\n");
	}

	UINT dtype = GetDriveTypeA(MountPoint.c_str());
	while (dtype == 1 || dtype == 0)
	{
		dtype = GetDriveTypeA(MountPoint.c_str());
		Sleep(10);
	}
	std::cout << "OK" << std::endl;

	std::string MountLetter = MountPoint.substr(0, 1);

	std::string ISO_location(MountLetter + ":\\mount.iso");

	if (FileExists(ISO_location.c_str()))
	{
		std::cout << "Mounting ISO... " << std::flush;
		if (AttachVirtualDiskEx(s2ws(ISO_location).c_str(), isohandle) == ERROR_SUCCESS)
		{
			std::cout << "ISO mounted.\r\n";
		}
		else
		{
			DWORD errorcode = GetLastError();
			std::cout << "ISO NOT mounted (" << errorcode << ").\r\n";
		}
	}

	std::cout << "Setting BoxedAppSDK Registry Path..." << std::flush;

	BoxedAppSDK_SetPersistentRegistryPathW(
		s2ws(VHD_location + ".registry").c_str()
		//s2ws(MountLetter + ":\\User\\ProgramData\\registry.dat").c_str()
		);

	std::cout << "OK\r\n" << std::flush;
	std::cout << "Initialising BoxedAppSDK..." << std::flush;

	BoxedAppSDK_Init();

	std::cout << "OK\r\n" << std::flush;
	std::cout << "Setting BoxedAppSDK options..." << std::flush;

	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__EMBED_BOXEDAPP_IN_CHILD_PROCESSES, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_FILE_SYSTEM, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_REGISTRY, TRUE);
	BoxedAppSDK_EnableOption(DEF_BOXEDAPPSDK_OPTION__INHERIT_OPTIONS, TRUE);

	//BoxedAppSDK_SetLogFileW(L"boxedapp.log");

	//BoxedAppSDK_EnableDebugLog(TRUE);

	std::cout << "OK\r\n" << std::flush;
	std::cout << "Updating environment to configured variables...\r\n" << std::flush;

	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE,		"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"", KEY_WOW64_32KEY, BxIsolationMode_WriteCopy);															
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CLASSES_ROOT,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_USER,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_LOCAL_MACHINE,		"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_USERS,				"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);
	BoxedAppSDK_SetRegKeyIsolationModeA(HKEY_CURRENT_CONFIG,	"", KEY_WOW64_64KEY, BxIsolationMode_WriteCopy);

	HKEY handle = NULL;

	if (ERROR_SUCCESS == BoxedAppSDK_CreateVirtualRegKeyW(HKEY_CLASSES_ROOT, L"", NULL, NULL, REG_OPTION_BACKUP_RESTORE, NULL, NULL, &handle, NULL)){ RegCloseKey(handle); }

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

	std::string Exec_Script(MountLetter + ":\\configuration.cxx");

	if (FileExists(Exec_Script.c_str()))
	{
		std::cout << "Executing setup script..." << std::endl;
		PrintSqs(32); 
		std::cout << " SCRIPT  OUTPUT "; 
		PrintSqs(112);
		std::cout << std::endl;
		TryScriptingShit(Exec_Script);
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

	registrydathandle = GetRegistryFileHandle();
	if (registrydathandle != INVALID_HANDLE_VALUE)
	{
		std::cout << "(RegData handle: " << (unsigned long)registrydathandle << ")..." << std::flush;
	}
	bool r = true;
	while (r)
	{
		DWORD dwFlags = CREATE_SUSPENDED | INHERIT_PARENT_AFFINITY;
		STARTUPINFO si;
		PROCESS_INFORMATION pi = PROCESS_INFORMATION();
		BOOL fSuccess;
		std::cout << "OK" << std::endl;

		//if (Config.UseShellExecute.compare("force") != 0)
		{
			// Create the child process, specifying a new environment block. 
			SecureZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);

			std::cout << "Starting process..." << std::flush;
			fSuccess = CreateProcess((MountPoint + Config.ApplicationProcess).c_str(), (LPSTR)("\"" + (MountPoint + Config.ApplicationProcess) + "\" " + Config.ApplicationParameters).c_str(), NULL, NULL, TRUE, dwFlags, NULL, (MountPoint + Config.ApplicationWorkingDir).c_str(), &si, &pi);
			if (!fSuccess)
			{
				std::cout << "Unable to launch process " << (MountPoint + Config.ApplicationProcess).c_str() << " (" << GetLastError() << ")" << std::endl;
				return (int)pressanykey("Hit any key to quit the application\r\n");
			}
			ResumeThread(pi.hThread);
			std::cout << "OK" << std::endl;
		}
		//else if (!Config.UseShellExecute.compare("true") || !Config.UseShellExecute.compare("force"))
		//{
		//	HANDLE hInstance = INVALID_HANDLE_VALUE;
		//	Sleep(1000);
		//	if (!IsProcessRunning(pi.dwProcessId))
		//	{
		//		std::cout << "Trying shellexecute..." << std::flush;
		//		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		//		hInstance = ShellExecute(NULL, "open", (MountPoint + Config.ApplicationProcess).c_str(), (LPSTR)Config.ApplicationParameters.c_str(), (MountPoint + Config.ApplicationWorkingDir).c_str(), SW_SHOWNORMAL);
		//		if (hInstance > (HANDLE)32)
		//		{
		//			std::cout << "OK" << std::endl;
		//		}
		//		else
		//		{
		//			std::cout << "Unable to launch process " << (MountPoint + Config.ApplicationProcess).c_str() << " (" << GetLastError() << ")" << std::endl;
		//			return (int)pressanykey("Hit any key to quit the application\r\n");
		//		}
		//	}
		//}

		int ret = pressanykey("Press any key when done playing to close this window (will detach the Virtual Hard Drive too!)\r\n");
		r = ret == 'R' || ret == 'r';
		while (IsProcessRunning(pi.dwProcessId))
		{
			ret = pressanykey("Game is still running.\r\n");
			r = ret == 'R' || ret == 'r';
		}
	}

	if (registrydathandle != INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(registrydathandle);
		CloseHandle(registrydathandle);
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
	if (registrydathandle != INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(registrydathandle);
		CloseHandle(registrydathandle);
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