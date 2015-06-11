#define DEFIND_GUID
#include <Windows.h>
#include <initguid.h>
#include <virtdisk.h>
#include <Shlwapi.h>
#include <string>
#include <iostream>
#include <algorithm>

#include "string_converters.hxx"
#include "file_functions.hxx"
#include "globalvars.hxx"

#include "vhd_helper.hxx"

DWORD AttachVirtualDiskEx(_In_    LPCWSTR     VirtualDiskPath, _In_	HANDLE&		vhdHandle)
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

	opStatus = OpenVirtualDisk(&storageType, VirtualDiskPath, accessMask, OPEN_VIRTUAL_DISK_FLAG_NONE, &openParameters, &vhdHandle);

	if (opStatus != ERROR_SUCCESS)
	{
		return opStatus;
	}

	memset(&attachParameters, 0, sizeof(attachParameters));
	attachParameters.Version = ATTACH_VIRTUAL_DISK_VERSION_1;

	opStatus = AttachVirtualDisk(vhdHandle, NULL, attachFlags, 0, &attachParameters, NULL);

	return opStatus;
}

std::wstring GetFirstVolumeByName(int physicalDriveNumber)
{
	wchar_t volumeNameBuffer[MAX_PATH];
	wchar_t volumeName2Buffer[MAX_PATH];

	HANDLE search = FindFirstVolumeW(volumeNameBuffer, ARRAY_SIZE(volumeNameBuffer));
	if (search != INVALID_HANDLE_VALUE)
	{
		do
		{
			GetVolumeInformationW(volumeNameBuffer, volumeName2Buffer, ARRAY_SIZE(volumeName2Buffer), 0, 0, 0, 0, 0);

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
	if (h_file != INVALID_HANDLE_VALUE) {
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

std::string GetFirstAvailableVHD(void)
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
	} while (FindNextFileA(hFind, &fileInfo) != 0);

	return "";
}

bool OpenGameDisk(void)
{
	std::cout << "Opening Game Disk..." << std::flush;

	std::string VHD_location = "./" + GetFirstAvailableVHD();

	std::cout << " (" << VHD_location << ") " << std::flush;

	if (AttachVirtualDiskEx(s2ws(VHD_location).c_str(), handle) != ERROR_SUCCESS)
	{
		std::cout << "Unable to open virtual disk '" << VHD_location << "'" << std::endl;
		return false;
	}

	std::cout << "OK" << std::endl;
	return true;
}

bool ConfigurePathInformation(void)
{
	std::cout << "Configuring path information..." << std::flush;

	// Now we need to grab the device name \\.\PhysicalDrive#
	wchar_t  physicalDriveName[MAX_PATH];
	DWORD   physicalDriveNameSize = ARRAY_SIZE(physicalDriveName);


	DWORD result = GetVirtualDiskPhysicalPath(handle, &physicalDriveNameSize, physicalDriveName);
	if (result != ERROR_SUCCESS)
	{
		std::wcout << "Unable to retrieve virtual disk path" << std::endl;
		return false;
	}
	std::cout << "OK" << std::endl;

	PhysicalPath = (ws2s(std::wstring(physicalDriveName)));
	return true;
}

bool TakeDiskOnline(void)
{
	std::cout << "Onlineing VDISK..." << std::flush;

	PhysicalPath.erase(PhysicalPath.begin(), PhysicalPath.begin() + strlen("\\\\.\\PhysicalDrive"));
	std::cout << "Drive : " << PhysicalPath << std::endl;

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
		return false;
	}

	std::cout << "Executing mount..." << std::flush;
	ResumeThread(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	std::cout << "(Executed) OK" << std::endl;
	return true;
}

bool ConfigureVolumeInformation(void)
{
	std::cout << "Configuring volume information..." << std::flush;

	int physicalDriveNumber = std::atoi(PhysicalPath.c_str());
	volumeName = GetFirstVolumeByName(physicalDriveNumber);
	if (volumeName.size() <= 0 || volumeName.size() > 256)
	{
		int i = 0;
		while (++i < 40)
		{
			volumeName = GetFirstVolumeByName(physicalDriveNumber);
			if (volumeName.size() <= 0 || volumeName.size() > 256)
			{
				std::cout << "." << std::flush;
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
		return false;
	}
	std::cout << "OK" << std::endl;
	return true;
}

bool MountVolume(void)
{
	std::cout << "Mounting to configured mountpoint..." << std::flush;
	if (!SetVolumeMountPointW(s2ws(MountPoint).c_str(), volumeName.c_str()))
	{
		std::cout << "Unable to mount VHD to " << MountPoint << " (" << GetLastError() << ")" << std::endl;
		return false;
	}

	UINT dtype;
	do
	{
		Sleep(25);
		dtype = GetDriveTypeA(MountPoint.c_str());
	} while (dtype == 1 || dtype == 0);

	MountLetter = MountPoint.substr(0, 1);
	std::cout << "OK" << std::endl;
	return true;
}

bool MountISO(void)
{
	ISO_location = (MountLetter + ":\\mount.iso");

	if (FileExists(ISO_location.c_str()))
	{
		std::cout << "Mounting ISO... " << std::flush;
		if (AttachVirtualDiskEx(s2ws(ISO_location).c_str(), isohandle) == ERROR_SUCCESS)
		{
			std::cout << "ISO mounted.\r\n";
			return true;
		}
		else
		{
			DWORD errorcode = GetLastError();
			std::cout << "ISO NOT mounted (" << errorcode << ").\r\n";
			return false;
		}
	}
	return false;
}