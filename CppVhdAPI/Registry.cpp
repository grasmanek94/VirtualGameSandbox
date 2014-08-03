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
#include <Registry/RegistryKey.hpp>

using namespace JetByteTools;

bool REG_SOFTWARE_SetValueStr(int type, const string& key, const string& valueName, const string& value, int datatype /*= REG_SZ*/, int ACCESS)
{
	try
	{
		CRegistryKey((type == 0) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, "SOFTWARE").CreateOrOpenKey(key.c_str(), NULL, _T(""), REG_OPTION_NON_VOLATILE, ACCESS).SetValue((LPCTSTR)valueName.c_str(), (LPCTSTR)value.c_str(), (DWORD)datatype);
		return true;
	}
	catch (CRegistryKey::Exception &e)
	{
		std::cout << "Exception caught in AngelScript/" << __FILE__ << "/" << __FUNCTION__ << "/ErrorCode(" << e.GetError() << ")" << std::endl;
		return false;
	}
}

bool REG_SOFTWARE_SetValueByte(int type, const string& key, const string& valueName, string& bytes, int size, int datatype,  int ACCESS)
{
	try
	{
		CRegistryKey((type == 0) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, "SOFTWARE").CreateOrOpenKey(key.c_str(), NULL, _T(""), REG_OPTION_NON_VOLATILE, ACCESS).SetValue((LPCTSTR)valueName.c_str(), (LPBYTE)bytes.c_str(), (DWORD)size);
		return true;
	}
	catch (CRegistryKey::Exception &e)
	{
		std::cout << "Exception caught in AngelScript/" << __FILE__ << "/" << __FUNCTION__ << "/ErrorCode(" << e.GetError() << ")" << std::endl;
		return false;
	}
}

bool REG_SOFTWARE_SetValueDword(int type, const string& key, const string& valueName, int value, int datatype /*= REG_SZ*/, int ACCESS)
{
	try
	{
		CRegistryKey((type == 0) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, "SOFTWARE").CreateOrOpenKey(key.c_str(), NULL, _T(""), REG_OPTION_NON_VOLATILE, ACCESS).SetValue((LPCTSTR)valueName.c_str(), (DWORD)value, (DWORD)datatype);
		return true;
	}
	catch (CRegistryKey::Exception &e)
	{
		std::cout << "Exception caught in AngelScript/" << __FILE__ << "/" << __FUNCTION__ << "/ErrorCode(" << e.GetError() << ")" << std::endl;
		return false;
	}
}

bool REG_SOFTWARE_FlushKey(int type, const string& key, int ACCESS)
{
	try
	{
		CRegistryKey((type == 0) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, "SOFTWARE").CreateOrOpenKey(key.c_str(), NULL, _T(""), REG_OPTION_NON_VOLATILE, ACCESS).FlushKey();
		return true;
	}
	catch (CRegistryKey::Exception &e)
	{
		std::cout << "Exception caught in AngelScript/" << __FILE__ << "/" << __FUNCTION__ << "/ErrorCode(" << e.GetError() << ")" << std::endl;
		return false;
	}
}

// This function will register the application interface
#define REGISTER(a,b) r = engine->RegisterGlobalFunction(a, asFUNCTION(b), asCALL_CDECL); assert(r >= 0)
int ConfigureRegistryForScriptEngine(asIScriptEngine *engine)
{
	int r;

	REGISTER("bool REG_SOFTWARE_SET_STR(int, const string& in, const string& in, const string& in, int, int)", REG_SOFTWARE_SetValueStr);
	REGISTER("bool REG_SOFTWARE_SET_BYTES(int, const string& in, const string& in, const string& in, int, int, int)", REG_SOFTWARE_SetValueByte);
	REGISTER("bool REG_SOFTWARE_SET_DWORD(int, const string& in, const string& in, int, int, int)", REG_SOFTWARE_SetValueDword);
	REGISTER("bool REG_SOFTWARE_FLUSH(int, const string& in, int)", REG_SOFTWARE_FlushKey);
	return 0;
}