#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

#include "file_functions.hxx"
#include "key_helper.hxx"
#include "globalvars.hxx"

#include "config.hxx"

#define CONFIG_SET(type,option,var) \
{\
	{\
	boost::optional<type> v = pt.get_optional<type>(option);\
	if (v.is_initialized())\
	var = v.get();\
	}\
	pt.put<type>(option, var);\
}

void CheckConfig()
{
	if (FileExists(".\\config.cfg"))
	{
		//OK
	}
	else
	{
		FILE * pFile;
		pFile = fopen(".\\config.cfg", "a+");
		if (pFile != NULL)
		{
			fwrite("{}", 1, 2, pFile);
			fclose(pFile);
		}
	}
}

_Config::_Config()
{
	ApplicationProcess = "Game\\game.exe";
	ApplicationWorkingDir = "Game\\";
	ApplicationParameters = "-debug -sv_cheats \"1\"";
	ApplicationAutoLaunch = "yes";

	GameConfigNickname = "Player";
}

void _Config::Save()
{
	CheckConfig();
	boost::property_tree::json_parser::write_json(".\\config.cfg", pt);
}

void _Config::Load()
{
	CheckConfig();
	boost::property_tree::json_parser::read_json(".\\config.cfg", pt);

	CONFIG_SET(std::string, "Application.Process", ApplicationProcess);
	CONFIG_SET(std::string, "Application.WorkingDirectory", ApplicationWorkingDir);
	CONFIG_SET(std::string, "Application.CommandLine", ApplicationParameters);
	CONFIG_SET(std::string, "Application.AutoLaunch", ApplicationAutoLaunch);

	CONFIG_SET(std::string, "GameConfig.Nickname", GameConfigNickname);

	Save();
}

_Config::~_Config()
{
	pt.clear();
}

bool LoadConfiguration(void)
{
	std::cout << "Loading configuration..." << std::flush;

	Config.Load();

	std::cout << "OK" << std::endl;
	return true;
}

void CheckConfigAutoLaunch(void)
{
	if (Config.ApplicationAutoLaunch.size() <= 2)
	{
		pressanykey("Press any key to launch the game...\r\n");
	}
	else
	{
		std::cout << "Automatically launching game..." << std::endl;
	}
}