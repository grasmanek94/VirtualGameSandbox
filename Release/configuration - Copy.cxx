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

array<string> charset =
{
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

int main()
{
	string serial;
	for (int i = 0; i < 20; ++i)
	{
		serial += charset[rand() % 36];
	}
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3\\ergc", "", serial, REG_SZ, KEY_ALL_ACCESS);

	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "CD Drive", GetConfig("VHD.MountPoint"), REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "DisplayName", "Command & Conquer(TM) Red Alert(TM) 3", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "ProductName", "Command & Conquer(TM) Red Alert(TM) 3", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Install Dir", GetConfig("VHD.MountPoint") + "Game\\", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Installed From", GetConfig("VHD.MountPoint"), REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Locale", "en_US", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Language", "English (US)", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Patch URL", "http://www.ea.com/redalert", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Product GUID", "{296D8550-CB06-48E4-9A8B-E5034FB64715}", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Suppression Exe", "", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Registration", "Software\\Electronic Arts\\Electronic Arts\\Red Alert 3\\ergc", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "Readme", GetConfig("VHD.MountPoint") + "Game\\Support\\readme.txt", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "UserDataLeafName", "Red Alert 3", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "ScreenshotsFolderName", "Screenshots", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "ReplayFolderName", "Replays", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "ProfileFolderName", "Profiles", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "SaveFolderName", "SaveGames", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_DWORD(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3", "UseLocalUserMaps", 0, REG_DWORD, KEY_ALL_ACCESS);

	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3\\1.0", "DisplayName", "Red Alert 3", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3\\1.0", "LanguageName", "English (US)", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_DWORD(HKLM, "Electronic Arts\\Electronic Arts\\Red Alert 3\\1.0", "Language", 1, REG_DWORD, KEY_ALL_ACCESS);
		
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts, Los Angeles\\Red Alert 3 Mod SDK\\1.00.0000", "", "", REG_SZ, KEY_ALL_ACCESS);
	REG_SOFTWARE_SET_STR(HKLM, "Electronic Arts\\Command & Conquer(tm) Red Alert(tm) 3 Worldbuilder\\1.0", "", "", REG_SZ, KEY_ALL_ACCESS);

	print("Config OK\r\n");
	return 0;
}