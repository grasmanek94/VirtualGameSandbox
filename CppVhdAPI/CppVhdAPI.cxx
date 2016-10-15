#include <iostream>

#include "debugging.hxx"
#include "key_helper.hxx"
#include "mountpoint_helper.hxx"
#include "vhd_helper.hxx"
#include "scripting.hxx"
#include "cleanup.hxx"
#include "randomization.hxx"
#include "process_helper.hxx"
#include "virtualization_helper.hxx"
#include "config.hxx"
#include "nviddiaoptimusfixer.hxx"

void ShowHello()
{
	std::cout << "GameLauncher by Rafal 'Gamer_Z' Grasman" << std::endl;
	std::cout << "See config.cfg for more info" << std::endl;
	std::cout << "Please wait while preparing the game for start..." << std::endl;
}

int RunThisProgram()
{
	//FixNvidiaOptimusNotChainingGames();

	InitialiseRandomGenerator();

	ShowHello();

	if (!FixPrivilidges())
	{
		return KeyPressCleanup();
	}

	if (!LoadConfiguration())
	{
		return KeyPressCleanup();
	}

	if (!UpdateMountpoint())
	{
		return KeyPressCleanup();
	}

	if (!OpenGameDisk())
	{
		return KeyPressCleanup();
	}

	if (!ConfigurePathInformation())
	{
		return KeyPressCleanup();
	}

	if (!TakeDiskOnline())
	{
		return KeyPressCleanup();
	}

	if(!ConfigureVolumeInformation())
	{
		return KeyPressCleanup();
	}

	if (!MountVolume())
	{
		return KeyPressCleanup();
	}

	MountISO();

	PerformScriptExecution();

	CheckConfigAutoLaunch();

	if (!LaunchGame())
	{
		return KeyPressCleanup();
	}

	PerformCleanup();
	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	PerformCleanup();
	return 1;
}

int main(int argc, char *argv[])
{
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	return RunThisProgram();
}