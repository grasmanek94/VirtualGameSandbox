// proxydll.h
#pragma once

#define SDX_STATIC_LIB
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include "d3d9.h"
#include "myIDirect3D9.h"
#include "myIDirect3DDevice9.h"
#include "myIDirect3DSwapChain9.h"

// Exported function
IDirect3D9* WINAPI Direct3DCreate9 (UINT SDKVersion);

// regular functions
void LoadOriginalDll(void);