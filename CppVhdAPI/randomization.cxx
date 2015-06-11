#include <Windows.h>
#include <time.h>

void InitialiseRandomGenerator(void)
{
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	srand((unsigned int)(((DWORDLONG)time(NULL)) + ((DWORDLONG)statex.dwMemoryLoad) + statex.ullTotalVirtual + statex.ullTotalPhys + statex.ullTotalPageFile + statex.ullAvailExtendedVirtual + statex.ullAvailPageFile + statex.ullAvailPhys + statex.ullAvailVirtual));
}