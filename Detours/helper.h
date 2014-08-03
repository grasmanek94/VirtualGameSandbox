#define WIN32_LEAN_AND_MEAN

#include <SDKDDKVer.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <detours.h>
#include <initializer_list>

#pragma warning(disable:4996)

std::string string_format(const std::string fmt, ...);

typedef std::pair<PVOID*, PVOID> HookType;