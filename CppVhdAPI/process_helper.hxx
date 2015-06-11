#pragma once
#include <Windows.h>

BOOL IsProcessRunning(DWORD pid);
HANDLE GetRegistryFileHandle(void);
bool LaunchGame(void);