#pragma once
#include <Windows.h>
#include <string>

std::string GetFirstAvailableVHD(void);
DWORD AttachVirtualDiskEx(LPCWSTR VirtualDiskPath, HANDLE& vhdHandle);

bool OpenGameDisk(void);
bool ConfigurePathInformation(void);
bool TakeDiskOnline(void);
bool ConfigureVolumeInformation(void);
bool MountVolume(void);
bool MountISO(void);
