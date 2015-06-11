#include <angelscript.h>
#include <add_on/scriptbuilder/scriptbuilder.h>
#include <add_on/scriptstdstring/scriptstdstring.h>
#include <add_on/scriptarray/scriptarray.h>
#include <add_on/scriptfile/scriptfile.h>
#include <add_on/scripthelper/scripthelper.h>
#include <add_on/scriptmath/scriptmath.h>
#include <add_on/debugger/debugger.h>

#include <string>

#include <assert.h>

using namespace std;
#include <Ini/SimpleIni.h>

extern std::string MountPoint;

bool INI_SET_VALUE_STR(const string& file, const string& section, const string& key, const string& newvalue)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	std::string resultfile = MountPoint + file;

	if (ini.LoadFile(resultfile.c_str()) < 0)
	{
		return false;
	}

	if (ini.SetValue(section.c_str(), key.c_str(), newvalue.c_str()) < 0)
	{
		return false;
	}

	if (ini.SaveFile(resultfile.c_str(), false) < 0)
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

	REGISTER("bool INI_SET_VALUE_STR(const string& in, const string& in, const string& in, const string& in)", INI_SET_VALUE_STR);
	return 0;
}