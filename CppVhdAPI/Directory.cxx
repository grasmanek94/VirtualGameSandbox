#include <angelscript.h>
#include <add_on/scriptbuilder/scriptbuilder.h>
#include <add_on/scriptstdstring/scriptstdstring.h>
#include <add_on/scriptarray/scriptarray.h>
#include <add_on/scriptfile/scriptfile.h>
#include <add_on/scripthelper/scripthelper.h>
#include <add_on/scriptmath/scriptmath.h>
#include <add_on/debugger/debugger.h>

#include <iostream>
#include <string>

#include <Windows.h>
#include <assert.h>

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

extern std::string MountPoint;

bool DIRECTORY_EXISTS(const string& dir)
{
	return dirExists(MountPoint + dir);
}

bool IO_MOVE(const string& dir, const string& newdir)
{
	std::string resultdir = MountPoint + dir;
	std::string resultnewdir = MountPoint + newdir;

	return MoveFileA(resultdir.c_str(), resultnewdir.c_str()) != 0;
}

// This function will register the application interface
#define REGISTER(a,b) r = engine->RegisterGlobalFunction(a, asFUNCTION(b), asCALL_CDECL); assert(r >= 0)
int ConfigureDirectoryForScriptEngine(asIScriptEngine *engine)
{
	int r;

	REGISTER("bool DIRECTORY_EXISTS(const string& in)", DIRECTORY_EXISTS);
	REGISTER("bool MOVE(const string& in, const string& in)", IO_MOVE);
	return 0;
}