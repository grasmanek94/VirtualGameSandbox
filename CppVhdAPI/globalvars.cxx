#include <Windows.h>
#include <string>

#include "config.hxx"

#include "globalvars.hxx"

std::string MountPoint(" :\\");
_Config Config;
HANDLE  isohandle;
HANDLE  handle;
HANDLE registrydathandle;
std::string PhysicalPath;
std::wstring volumeName;
std::string MountLetter;
std::string ISO_location;