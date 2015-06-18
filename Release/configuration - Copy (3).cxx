/*
void;print(const string &in str);
array<string> @getCommandLineArgs();
string GetConfig(const string &in option);
int rand();

bool REG_SOFTWARE_SET_STR(int type, const string& key, const string& valueName, const string& value, int datatype, int ACCESS);
bool REG_SOFTWARE_SET_BYTES(int type, const string& key, const string& valueName, const char bytes[], int size, int ACCESS);
bool REG_SOFTWARE_SET_DWORD(int type, const string& key, const string& valueName, int value, int datatype, int ACCESS);
bool REG_SOFTWARE_FLUSH(int type, const string& key, int ACCESS);
bool INI_SET_VALUE_STR(const string& file, const string& section, const string& key, const string& newvalue);
bool XML_SET_VALUE_STR(const string& file, const string& path, const string& newvalue);
bool DIRECTORY_EXISTS(const string& directory);
bool MOVE(const string& old, const string& new); //rename or move file or directory
bool SET_VALUE(const string& file, const string& content);

Some objects that are available:

string
array<T>
file
*/

uint8 HKLM = 0;
uint8 HKCU = 1;

uint8 REG_NONE = 0;
uint8 REG_SZ = 1;
uint8 REG_EXPAND_SZ = 2;
uint8 REG_BINARY = 3;
uint8 REG_DWORD = 4;
uint8 REG_DWORD_LITTLE_ENDIAN = 4;
uint8 REG_DWORD_BIG_ENDIAN = 5;
uint8 REG_LINK = 6;
uint8 REG_MULTI_SZ = 7;
uint8 REG_RESOURCE_LIST = 8;
uint8 REG_FULL_RESOURCE_DESCRIPTOR = 9;
uint8 REG_RESOURCE_REQUIREMENTS_LIST = 10;
uint8 REG_QWORD = 11;
uint8 REG_QWORD_LITTLE_ENDIAN = 11;

//
// Registry Specific Access Rights.
//

int KEY_WOW64_32KEY = (0x0200);
int KEY_WOW64_64KEY = (0x0100);
int KEY_WOW64_RES = (0x0300);

//
//  The following are masks for the predefined standard access types
//
int KEY_ALL_ACCESS = 0xF003F;

int main()
{
	INI_SET_VALUE_STR("Game\\NODENAME.INI", GetConfig("GameConfig.Nickname"));

	print("Config OK\r\n");
	return 0;
}