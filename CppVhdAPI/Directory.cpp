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

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool DIRECTORY_EXISTS(const string& dir)
{
	return dirExists(dir);
}

// This function will register the application interface
#define REGISTER(a,b) r = engine->RegisterGlobalFunction(a, asFUNCTION(b), asCALL_CDECL); assert(r >= 0)
int ConfigureDirectoryForScriptEngine(asIScriptEngine *engine)
{
	int r;

	REGISTER("bool DIRECTORY_EXISTS(const string& in)", DIRECTORY_EXISTS);
	return 0;
}