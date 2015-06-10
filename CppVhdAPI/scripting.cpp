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
// Function prototypes
int ConfigureEngine(asIScriptEngine *engine);
int CompileScript(asIScriptEngine *engine, const char *scriptFile);
int ExecuteScript(asIScriptEngine *engine, const char *scriptFile, bool debug);

// For the scripts
void PrintString(const string &str);
CScriptArray *GetCommandLineArgs();
int script_rand();
string GetConfig(const string& option);

CScriptArray *g_commandLineArgs = 0;
int           g_argc = 0;
char        **g_argv = 0;

// This message callback is used by the engine to send compiler messages
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

extern _Config Config;

void TryScriptingShit(const std::string& script)
{
	asIScriptEngine *engine = nullptr;
	if (FileExists(script.c_str()))
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		if (engine == 0)
		{
			std::cout << "Failed to create script engine." << std::endl;
		}
		else
		{
			// The script compiler will send any compiler messages to the callback
			engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
			// Configure the script engine with all the functions, 
			// and variables that the script should be able to use.
			int r = ConfigureEngine(engine);
			if (r < 0)
			{
				std::cout << "Failed to configure script engine." << std::endl;
			}
			else
			{
				// Compile the script code
				r = CompileScript(engine, script.c_str());
				if (r < 0)
				{
					std::cout << "Failed to compile script." << std::endl;
				}
				else
				{

					// Execute the script
					r = ExecuteScript(engine, script.c_str(), false);

				}
			}
			// Release the engine
			engine->Release();
		}
	}
}

int ConfigureRegistryForScriptEngine(asIScriptEngine *engine);
int ConfigureIniForScriptEngine(asIScriptEngine *engine);
int ConfigureDirectoryForScriptEngine(asIScriptEngine *engine);
// This function will register the application interface
int ConfigureEngine(asIScriptEngine *engine)
{
	int r;

	RegisterStdString(engine);
	RegisterScriptArray(engine, false);
	RegisterStdStringUtils(engine);
	RegisterScriptMath(engine);

	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(PrintString), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("array<string> @getCommandLineArgs()", asFUNCTION(GetCommandLineArgs), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("int rand()", asFUNCTION(script_rand), asCALL_CDECL); assert(r >= 0);
	r = engine->RegisterGlobalFunction("string GetConfig(const string &in)", asFUNCTION(GetConfig), asCALL_CDECL); assert(r >= 0);
	
	ConfigureRegistryForScriptEngine(engine);
	ConfigureIniForScriptEngine(engine);
	ConfigureDirectoryForScriptEngine(engine);
	// TODO: There should be an option of outputting the engine 
	//       configuration for use with the offline compiler asbuild.
	//       It should then be possible to execute pre-compiled bytecode.

	return 0;
}

// This is the to-string callback for the string type
std::string StringToString(void *obj, bool expandMembers, CDebugger *dbg)
{
	// We know the received object is a string
	std::string *val = reinterpret_cast<std::string*>(obj);

	// Format the output string
	// TODO: Should convert non-readable characters to escape sequences
	std::stringstream s;
	s << "(len=" << val->length() << ") \"";
	if (val->length() < 20)
		s << *val << "\"";
	else
		s << val->substr(0, 20) << "...";

	return s.str();
}

// This is the to-string callback for the array type
// This is generic and will take care of all template instances based on the array template
std::string ArrayToString(void *obj, bool expandMembers, CDebugger *dbg)
{
	CScriptArray *arr = reinterpret_cast<CScriptArray*>(obj);

	std::stringstream s;
	s << "(len=" << arr->GetSize() << ")";

	if (expandMembers)
	{
		s << " [";
		for (asUINT n = 0; n < arr->GetSize(); n++)
		{
			s << dbg->ToString(arr->At(n), arr->GetElementTypeId(), false, arr->GetArrayObjectType()->GetEngine());
			if (n < arr->GetSize() - 1)
				s << ", ";
		}
		s << "]";
	}

	return s.str();
}

// This function will register to-string callbacks in the debugger for the application registered types
void RegisterToStringCallbacks(CDebugger *dbg, asIScriptEngine *engine)
{
	dbg->RegisterToStringCallback(engine->GetObjectTypeByName("string"), StringToString);
	dbg->RegisterToStringCallback(engine->GetObjectTypeByName("array"), ArrayToString);
}

// This is where the script is compiled into bytecode that can be executed
int CompileScript(asIScriptEngine *engine, const char *scriptFile)
{
	int r;

	// We will only initialize the global variables once we're 
	// ready to execute, so disable the automatic initialization
	engine->SetEngineProperty(asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false);

	CScriptBuilder builder;
	r = builder.StartNewModule(engine, "script");
	if (r < 0) return -1;

	r = builder.AddSectionFromFile(scriptFile);
	if (r < 0) return -1;

	r = builder.BuildModule();
	if (r < 0)
	{
		engine->WriteMessage(scriptFile, 0, 0, asMSGTYPE_ERROR, "Script failed to build");
		return -1;
	}

	return 0;
}

// Execute the script by calling the main() function
int ExecuteScript(asIScriptEngine *engine, const char *scriptFile, bool debug)
{
	CDebugger *dbg = 0;

	asIScriptModule *mod = engine->GetModule("script", asGM_ONLY_IF_EXISTS);
	if (!mod) return -1;

	// Find the main function
	asIScriptFunction *func = mod->GetFunctionByDecl("int main()");
	if (func == 0)
	{
		// Try again with "void main()"
		func = mod->GetFunctionByDecl("void main()");
	}

	if (func == 0)
	{
		engine->WriteMessage(scriptFile, 0, 0, asMSGTYPE_ERROR, "Cannot find 'int main()' or 'void main()'");
		return -1;
	}

	// Set up a context to execute the script
	asIScriptContext *ctx = engine->CreateContext();

	if (debug)
	{
		cout << "Debugging, waiting for commands. Type 'h' for help." << endl;

		// Create the debugger instance
		dbg = new CDebugger();

		// Register the to-string callbacks so the user can see the contents of objects
		RegisterToStringCallbacks(dbg, engine);

		// Allow the user to initialize the debugging before moving on
		dbg->TakeCommands(ctx);
	}

	if (dbg)
	{
		// Set the line callback for the debugging
		ctx->SetLineCallback(asMETHOD(CDebugger, LineCallback), dbg, asCALL_THISCALL);
	}

	// Once we have the main function, we first need to initialize the global variables
	// Pass our own context so the initialization of the global variables can be debugged too
	int r = mod->ResetGlobalVars(ctx);
	if (r < 0)
	{
		engine->WriteMessage(scriptFile, 0, 0, asMSGTYPE_ERROR, "Failed while initializing global variables");
		return -1;
	}

	// Execute the script
	ctx->Prepare(func);
	r = ctx->Execute();
	if (r != asEXECUTION_FINISHED)
	{
		if (r == asEXECUTION_EXCEPTION)
		{
			cout << "The script failed with an exception" << endl;
			PrintException(ctx, true);
			r = -1;
		}
		else if (r == asEXECUTION_ABORTED)
		{
			cout << "The script was aborted" << endl;
			r = -1;
		}
		else
		{
			cout << "The script terminated unexpectedly (" << r << ")" << endl;
			r = -1;
		}
	}
	else
	{
		// Get the return value from the script
		if (func->GetReturnTypeId() == asTYPEID_INT32)
		{
			r = *(int*)ctx->GetAddressOfReturnValue();
		}
		else
			r = 0;
	}
	ctx->Release();

	// Destroy debugger
	if (dbg)
		delete dbg;

	return r;
}

// This little function allows the script to print a string to the screen
void PrintString(const string &str)
{
	cout << str;
}

// This function returns the command line arguments that were passed to the script
CScriptArray *GetCommandLineArgs()
{
	if (g_commandLineArgs)
	{
		g_commandLineArgs->AddRef();
		return g_commandLineArgs;
	}

	// Obtain a pointer to the engine
	asIScriptContext *ctx = asGetActiveContext();
	asIScriptEngine *engine = ctx->GetEngine();

	// Create the array object
	asIObjectType *arrayType = engine->GetObjectTypeById(engine->GetTypeIdByDecl("array<string>"));
	g_commandLineArgs = new CScriptArray(0, arrayType);

	// Find the existence of the delimiter in the input string
	for (int n = 0; n < g_argc; n++)
	{
		// Add the arg to the array
		g_commandLineArgs->Resize(g_commandLineArgs->GetSize() + 1);
		((string*)g_commandLineArgs->At(n))->assign(g_argv[n]);
	}

	// Return the array by handle
	g_commandLineArgs->AddRef();
	return g_commandLineArgs;
}

int script_rand()
{
	return rand();
}

extern std::string MountPoint;

string GetConfig(const string& option)
{
	if (option.compare("VHD.MountPoint") == 0)
	{
		return MountPoint;
	}

	std::string var("");
	boost::optional<std::string> v = Config.pt.get_optional<std::string>(option);
	if (v.is_initialized())
	{
		var = v.get();
	}
	return var;
}