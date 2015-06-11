#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>

#define PHYS_PATH_LEN (1024+1)

void CheckConfig(void);

struct _Config
{
	std::string ApplicationProcess;
	std::string ApplicationWorkingDir;
	std::string ApplicationParameters;
	std::string ApplicationAutoLaunch;

	boost::property_tree::ptree pt;

	std::string GameConfigNickname;

	_Config(void);
	void Save(void);
	void Load(void);
	~_Config(void);
};

bool LoadConfiguration(void);
void CheckConfigAutoLaunch(void);