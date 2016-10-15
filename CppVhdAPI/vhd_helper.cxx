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

std::string GetFirstAvailableVHD(void)
{
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	hFind = FindFirstFile("*.vhd", &fileInfo);

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
	} while (FindNextFile(hFind, &fileInfo) != 0);

	return "";
}

bool OpenGameDisk(void)
{
	std::wcout << "Opening Game Disk..." << std::flush;

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
	DWORD   physicalDriveNameSize = sizeof(physicalDriveName);


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

bool ConfigureVolumeInformation(void)
{
	std::cout << "Configuring volume information..." << std::flush;

	int physicalDriveNumber = std::atoi(PhysicalPath.c_str() + strlen("\\\\.\\PhysicalDrive"));

	wchar_t volumeName[MAX_PATH];
	DWORD bytesReturned;
	VOLUME_DISK_EXTENTS diskExtents;
	HANDLE hFVol = FindFirstVolumeW(volumeName, sizeof(volumeName));
	bool hadTrailingBackslash = false;

	do {
		// I had a problem where CreateFile complained about the trailing \ and
		// SetVolumeMountPoint desperately wanted the backslash there. I ended up 
		// doing this to get it working but I'm not a fan and I'd greatly 
		// appreciate it if someone has any further info on this matter
		int backslashPos = lstrlenW(volumeName) - 1;
		if (hadTrailingBackslash = volumeName[backslashPos] == '\\') {
			volumeName[backslashPos] = 0;
		}

		HANDLE hVol = CreateFileW(volumeName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hVol == INVALID_HANDLE_VALUE) {
			std::cout << "Unable to mount VHD to " << MountPoint << " (Unable to acquire volume handle)" << std::endl;
			return false;
		}

		DeviceIoControl(hVol, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL,
			0, &diskExtents, sizeof(diskExtents), &bytesReturned, NULL);

		// If the volume were to span across multiple physical disks, you'd find 
		// more than one Extents here but we don't have to worry about that with VHD
		// Note that 'driveNumber' would be the integer you extracted out of 
		// 'physicalDrive' in the previous snippet
		if (diskExtents.Extents[0].DiskNumber == physicalDriveNumber) {
			if (hadTrailingBackslash) {
				volumeName[backslashPos] = '\\';
			}

			// Found volume that's on the VHD, let's mount it with a letter of our choosing.
			// Warning: SetVolumeMountPoint requires elevation
			std::cout << "Mounting to configured mountpoint..." << std::flush;
			if (!SetVolumeMountPointW(s2ws(MountPoint).c_str(), volumeName))
			{
				std::cout << "Unable to mount VHD to " << MountPoint << " (" << GetLastError() << ") : " << ws2s(volumeName) << ":" << physicalDriveNumber << std::endl;
				return false;
			}
			std::cout << "OK" << std::endl;
			return true;
		}
	} while (FindNextVolumeW(hFVol, volumeName, sizeof(volumeName)));
	FindVolumeClose(hFVol);
	std::cout << "Unable to mount VHD to " << MountPoint << " (No volumes found)" << std::endl;

	return false;
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