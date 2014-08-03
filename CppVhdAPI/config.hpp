#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <virtdisk.h>
#include <codecvt>
#include <Shlwapi.h>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#define PHYS_PATH_LEN (1024+1)

#pragma comment(lib, "VirtDisk.lib")
#pragma comment(lib, "Shlwapi.lib")

#define CONFIG_SET(type,option,var) \
{\
	{\
	boost::optional<type> v = pt.get_optional<type>(option);\
	if (v.is_initialized())\
	var = v.get();\
	}\
	pt.put<type>(option, var);\
}

void CheckConfig();
BOOL Is64BitWindows();
struct _Config
{
	std::string VHD_location;
	std::string MountPoint;
	std::string ISO_location;
	std::string Exec_Script;
	std::string VolumeNameToMount;
	std::string UseShellExecute;

	std::string ApplicationProcess;
	std::string ApplicationWorkingDir;
	std::string ApplicationParameters;
	std::string ApplicationAutoLaunch;

	std::string InjectionProcess;
	std::string InjectionDLL;

	std::string info[2];

	std::string EnvironmentVariables[28];

	boost::property_tree::ptree pt;

	std::string GameConfigNickname;

	std::string GetEnvironmentString(std::string name)
	{
		char buffer_env[1024];
		GetEnvironmentVariableA(name.c_str(), buffer_env, 1024);
		return std::string(buffer_env);
	}
	_Config()
	{
		//default config
		VHD_location = ".\\game.vhd";
		ISO_location = "Y:\\file.iso";
		MountPoint = "Y:\\";
		Exec_Script = "Y:\\configuration.cxx";
		VolumeNameToMount = "VirtualGameSandbox";
		UseShellExecute = "false";

		ApplicationProcess = "Y:\\Game\\game.exe";
		ApplicationWorkingDir = "Y:\\Game\\";
		ApplicationParameters = "-debug -sv_cheats \"1\"";
		ApplicationAutoLaunch = "yes";

		info[0] = "If updating this file make sure to update all settings accordingly.";
		info[1] = "This program has been created by Rafal 'Gamer_Z' Grasman";

		EnvironmentVariables[0] = GetEnvironmentString("LOGONSERVER");//"\\\\User";
		EnvironmentVariables[1] = GetEnvironmentString("USERDOMAIN");//"User";
		EnvironmentVariables[2] = GetEnvironmentString("USERDOMAIN_ROAMINGPROFILE");//"User";
		EnvironmentVariables[3] = GetEnvironmentString("COMPUTERNAME");//"User";
		EnvironmentVariables[4] = GetEnvironmentString("USERNAME");//"User";
		EnvironmentVariables[5] = GetEnvironmentString("NUMBER_OF_PROCESSORS");//"8";
		EnvironmentVariables[6] = GetEnvironmentString("OS");//"Windows_NT";
		EnvironmentVariables[7] = "Y:";
		EnvironmentVariables[8] = "Y:\\User";
		EnvironmentVariables[9] = "Y:\\User";
		EnvironmentVariables[10] = "\\User";
		EnvironmentVariables[11] = "Y:\\User\\AppData";
		EnvironmentVariables[12] = "Y:\\User\\AppData\\Local";
		EnvironmentVariables[13] = "Y:\\User\\ProgramData";
		EnvironmentVariables[14] = "Y:\\User\\ProgramData";
		EnvironmentVariables[15] = "Y:\\User\\ProgramFiles\\x64";
		EnvironmentVariables[16] = "Y:\\User\\ProgramFiles\\x86";
		EnvironmentVariables[17] = "Y:\\User\\ProgramFiles\\x64";
		EnvironmentVariables[18] = "Y:\\User\\ProgramFiles\\Common\\x64";
		EnvironmentVariables[19] = "Y:\\User\\ProgramFiles\\Common\\x86";
		EnvironmentVariables[20] = "Y:\\User\\ProgramFiles\\Common\\x64";
		EnvironmentVariables[21] = "Y:\\User\\TEMP";
		EnvironmentVariables[22] = "Y:\\User\\TEMP";
		EnvironmentVariables[23] = GetEnvironmentString("windir");//"C:\\Windows";
		EnvironmentVariables[24] = "";
		EnvironmentVariables[25] = "";
		EnvironmentVariables[26] = "NONE";
		if (Is64BitWindows())
		{
			EnvironmentVariables[27] = "64";
		}
		else
		{
			EnvironmentVariables[27] = "32";
		}

		GameConfigNickname = "Player";
	}

	void Save()
	{
		CheckConfig();
		boost::property_tree::json_parser::write_json(".\\config.cfg", pt);
	}

	void Load()
	{
		CheckConfig();
		boost::property_tree::json_parser::read_json(".\\config.cfg", pt);

		CONFIG_SET(std::string, "Info.a", info[0]);
		CONFIG_SET(std::string, "Info.b", info[1]);

		CONFIG_SET(std::string, "VHD.Location", VHD_location);
		CONFIG_SET(std::string, "VHD.MountPoint", MountPoint);
		CONFIG_SET(std::string, "VHD.IsoToMountAfterVHD", ISO_location);
		CONFIG_SET(std::string, "VHD.VolumeName", VolumeNameToMount);

		CONFIG_SET(std::string, "Automation.Script", Exec_Script);

		CONFIG_SET(std::string, "Application.Process", ApplicationProcess);
		CONFIG_SET(std::string, "Application.WorkingDirectory", ApplicationWorkingDir);
		CONFIG_SET(std::string, "Application.CommandLine", ApplicationParameters);
		CONFIG_SET(std::string, "Application.AutoLaunch", ApplicationAutoLaunch);
		CONFIG_SET(std::string, "Application.UseShellExecute", UseShellExecute);

		CONFIG_SET(std::string, "EnvironmentVariables.LOGONSERVER", EnvironmentVariables[0]);
		CONFIG_SET(std::string, "EnvironmentVariables.USERDOMAIN", EnvironmentVariables[1]);
		CONFIG_SET(std::string, "EnvironmentVariables.USERDOMAIN_ROAMINGPROFILE", EnvironmentVariables[2]);
		CONFIG_SET(std::string, "EnvironmentVariables.COMPUTERNAME", EnvironmentVariables[3]);
		CONFIG_SET(std::string, "EnvironmentVariables.USERNAME", EnvironmentVariables[4]);

		CONFIG_SET(std::string, "EnvironmentVariables.NUMBER_OF_PROCESSORS", EnvironmentVariables[5]);
		CONFIG_SET(std::string, "EnvironmentVariables.OS", EnvironmentVariables[6]);

		CONFIG_SET(std::string, "EnvironmentVariables.HOMEDRIVE", EnvironmentVariables[7]);
		CONFIG_SET(std::string, "EnvironmentVariables.USERPROFILE", EnvironmentVariables[8]);
		CONFIG_SET(std::string, "EnvironmentVariables.PUBLIC", EnvironmentVariables[9]);
		CONFIG_SET(std::string, "EnvironmentVariables.HOMEPATH", EnvironmentVariables[10]);
		CONFIG_SET(std::string, "EnvironmentVariables.APPDATA", EnvironmentVariables[11]);
		CONFIG_SET(std::string, "EnvironmentVariables.LOCALAPPDATA", EnvironmentVariables[12]);
		CONFIG_SET(std::string, "EnvironmentVariables.ALLUSERSPROFILE", EnvironmentVariables[13]);

		CONFIG_SET(std::string, "EnvironmentVariables.ProgramData", EnvironmentVariables[14]);
		CONFIG_SET(std::string, "EnvironmentVariables.ProgramFiles", EnvironmentVariables[15]);
		CONFIG_SET(std::string, "EnvironmentVariables.ProgramFiles(x86)", EnvironmentVariables[16]);
		CONFIG_SET(std::string, "EnvironmentVariables.ProgramW6432", EnvironmentVariables[17]);

		CONFIG_SET(std::string, "EnvironmentVariables.CommonProgramFiles", EnvironmentVariables[18]);
		CONFIG_SET(std::string, "EnvironmentVariables.CommonProgramFiles(x86)", EnvironmentVariables[19]);
		CONFIG_SET(std::string, "EnvironmentVariables.CommonProgramW6432", EnvironmentVariables[20]);

		CONFIG_SET(std::string, "EnvironmentVariables.TEMP", EnvironmentVariables[21]);
		CONFIG_SET(std::string, "EnvironmentVariables.TMP", EnvironmentVariables[22]);
		CONFIG_SET(std::string, "EnvironmentVariables.windir", EnvironmentVariables[23]);

		CONFIG_SET(std::string, "EnvironmentVariables.VirtualGameSandBoxEmulatorDLLToInject", EnvironmentVariables[24]);
		CONFIG_SET(std::string, "EnvironmentVariables.VirtualGameSandBoxEmulatorOriginalDLLPath", EnvironmentVariables[25]);
		CONFIG_SET(std::string, "EnvironmentVariables.VirtualGameSandBoxEmulatorInjectionType", EnvironmentVariables[26]);
		CONFIG_SET(std::string, "EnvironmentVariables.WindowsRuntimeArchitecture", EnvironmentVariables[27]);

		if (Is64BitWindows())
		{
			EnvironmentVariables[27] = "64";
		}
		else
		{
			EnvironmentVariables[27] = "32";
		}

		CONFIG_SET(std::string, "GameConfig.Nickname", GameConfigNickname);

		Save();
	}
	~_Config()
	{
		Save();
		pt.clear();
	}
};
#undef CONFIG_SET