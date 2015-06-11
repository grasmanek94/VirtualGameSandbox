#pragma once
#include <Windows.h>
#include <string>

std::string GetFirstAvailableVHD(void);
BOOL disk_offline(HANDLE h_file, bool enable);
std::wstring GetFirstVolumeByName(int physicalDriveNumber);
DWORD AttachVirtualDiskEx(LPCWSTR VirtualDiskPath, HANDLE& vhdHandle);

bool OpenGameDisk(void);
bool ConfigurePathInformation(void);
bool TakeDiskOnline(void);
bool ConfigureVolumeInformation(void);
bool MountVolume(void);
bool MountISO(void);

#define ARRAY_SIZE(a)                               \
	((sizeof(a) / sizeof(*(a))) /                   \
	static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
