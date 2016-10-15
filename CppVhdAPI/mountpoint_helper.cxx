#include <Windows.h>
#include <iostream>

#include "globalvars.hxx"

bool UpdateMountpoint(void)
{
	std::cout << "Checking for free mountpoint..." << std::flush;

	char p = 'Z';
	for (; p >= 'A'; --p)
	{
		MountLetter = "A";
		MountLetter[0] = p;
		MountPoint[0] = p;
		if (!(GetDriveTypeA(MountPoint.c_str()) != 1 && GetDriveTypeA(MountPoint.c_str()) != 0))
		{
			std::cout << " Using ('" << MountPoint << "')... OK" << std::endl;
			return true;
		}
	}

	std::cout << " No available mountpoints. " << std::endl;
	return false;
}