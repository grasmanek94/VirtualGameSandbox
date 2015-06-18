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

bool SET_VALUE(const string& file, const string& newvalue)
{
	std::string resultfile = MountPoint + file;

	FILE* fileptr = fopen(resultfile.c_str(), "w");
	if (!fileptr)
	{
		return false;
	}

	if (fwrite(newvalue.c_str(), newvalue.size() + 1, 1, fileptr) != 1)
	{
		fclose(fileptr);
		return false;
	}

	fclose(fileptr);
	return true;
}

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

bool XML_SET_VALUE_STR(const string& file, const string& path, const string& newvalue)
{
	std::string resultfile = MountPoint + file;
	boost::property_tree::ptree pt;
	try
	{
		boost::property_tree::read_xml(resultfile, pt, boost::property_tree::xml_parser::trim_whitespace, std::locale());

		pt.put(path, newvalue);

		boost::property_tree::xml_writer_settings<std::string> settings('\t', 1, "ASCII");
		boost::property_tree::write_xml(resultfile, pt, std::locale(), settings);
	}
	catch (...)
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
	REGISTER("bool SET_VALUE(const string& in, const string& in)", SET_VALUE);
	REGISTER("bool XML_SET_VALUE_STR(const string& in, const string& in, const string& in)", XML_SET_VALUE_STR);
	return 0;
}