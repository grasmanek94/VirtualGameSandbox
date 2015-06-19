#include <Windows.h>

#include "globalvars.hxx"
#include "randomization.hxx"
#include "process_helper.hxx"

bool IsSystem32BitOnly()
{	
#ifdef _WIN64
	return false;
#endif

	BOOL isWow64Process = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64Process);

	if (isWow64Process)
	{
		//do useless stuff
		InitialiseRandomGenerator();
		return false;
	}
	else
	{
		//do other useless stuff
		GetCurrentThread();
		return true;
	}
}
