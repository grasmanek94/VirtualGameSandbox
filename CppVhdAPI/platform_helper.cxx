#include <Windows.h>

#include "globalvars.hxx"
#include "randomization.hxx"
#include "process_helper.hxx"

bool IsSystem32BitOnly()
{	
#ifndef _WIN64
	BOOL isWow64Process = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64Process);

	if (isWow64Process)
	{
		//do useless stuff
		InitialiseRandomGenerator();
		return false;
	}
	else
#endif
	{
		//do other useless stuff
		GetCurrentThread();
		return false;
	}
}
