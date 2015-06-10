#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <string>
#include <tchar.h>
#include <strsafe.h>
#include <angelscript.h>
#define DEFIND_GUID
#include <initguid.h>
#include <virtdisk.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <locale>
#include <codecvt>
#include <add_on/scriptbuilder/scriptbuilder.h>
#include <add_on/scriptstdstring/scriptstdstring.h>
#include <add_on/scriptarray/scriptarray.h>
#include <add_on/scriptfile/scriptfile.h>
#include <add_on/scripthelper/scripthelper.h>
#include <add_on/scriptmath/scriptmath.h>
#include <add_on/debugger/debugger.h>
#include <config.hpp>

#pragma comment(lib, "angelscript.lib")

std::wstring s2ws(const std::string& str);
std::string ws2s(const std::wstring& wstr);
TCHAR pressanykey(const TCHAR* prompt = NULL);
DWORD OpenVDisk(const char* virtualDiskFilePath, HANDLE *handle);
DWORD OpenISO(const char* isoFilePath, HANDLE *handle);

#define ARRAY_SIZE(a)                               \
	((sizeof(a) / sizeof(*(a))) /                   \
	static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

BOOL EnableDebugPrivilege();
void CheckConfig();
BOOL FileExists(LPCSTR szPath);

BOOL IsProcessRunning(DWORD pid);

using namespace std;
#include <Ini/SimpleIni.h>

bool INI_SET_VALUE_STR(const string& file, const string& section, const string& key, const string& newvalue)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (ini.LoadFile(file.c_str()) < 0)
	{
		return false;
	}

	if (ini.SetValue(section.c_str(), key.c_str(), newvalue.c_str()) < 0)
	{
		return false;
	}

	if (ini.SaveFile(file.c_str(), false) < 0)
	{
		return false;
	}

	return true;
}

// This function will register the application interface
#define REGISTER(a,b) r = engine->RegisterGlobalFunction(a, asFUNCTION(b), asCALL_CDECL); assert(r >= 0)
int ConfigureIniForScriptEngine(asIScriptEngine *engine)
{
	int r;

	REGISTER("bool INI_SET_VALUE_STR(const string&, const string&, const string&, const string&)", INI_SET_VALUE_STR);
	return 0;
}