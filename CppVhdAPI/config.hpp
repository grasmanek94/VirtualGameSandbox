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

	boost::property_tree::ptree pt;

	std::string GameConfigNickname;

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

		CONFIG_SET(std::string, "GameConfig.Nickname", GameConfigNickname);

		Save();
	}
	~_Config()
	{
		pt.clear();
	}
};
#undef CONFIG_SET