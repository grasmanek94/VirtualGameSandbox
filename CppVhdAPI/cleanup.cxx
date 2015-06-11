#include <Windows.h>
#include <virtdisk.h>

#include "key_helper.hxx"
#include "globalvars.hxx"

void PerformCleanup(void)
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
}

int KeyPressCleanup(void)
{
	int ret = (int)pressanykey();
	PerformCleanup();
	return ret;
}