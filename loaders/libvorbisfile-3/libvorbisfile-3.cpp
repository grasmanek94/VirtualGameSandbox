#include <windows.h>
#pragma pack(1)


HINSTANCE hLThis = 0;
HINSTANCE hL = 0;
FARPROC p[36] = { 0 };

#pragma comment(lib, "VGSBE.lib")

void DoVGSBEInit(DWORD Reason);

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		hLThis = hInst;
		hL = LoadLibraryA(".\\libvorbisfile-3_.dll");
		if (!hL) return false;


		p[0] = GetProcAddress(hL, "ov_bitrate");
		p[1] = GetProcAddress(hL, "ov_bitrate_instant");
		p[2] = GetProcAddress(hL, "ov_clear");
		p[3] = GetProcAddress(hL, "ov_comment");
		p[4] = GetProcAddress(hL, "ov_crosslap");
		p[5] = GetProcAddress(hL, "ov_fopen");
		p[6] = GetProcAddress(hL, "ov_halfrate");
		p[7] = GetProcAddress(hL, "ov_halfrate_p");
		p[8] = GetProcAddress(hL, "ov_info");
		p[9] = GetProcAddress(hL, "ov_open");
		p[10] = GetProcAddress(hL, "ov_open_callbacks");
		p[11] = GetProcAddress(hL, "ov_pcm_seek");
		p[12] = GetProcAddress(hL, "ov_pcm_seek_lap");
		p[13] = GetProcAddress(hL, "ov_pcm_seek_page");
		p[14] = GetProcAddress(hL, "ov_pcm_seek_page_lap");
		p[15] = GetProcAddress(hL, "ov_pcm_tell");
		p[16] = GetProcAddress(hL, "ov_pcm_total");
		p[17] = GetProcAddress(hL, "ov_raw_seek");
		p[18] = GetProcAddress(hL, "ov_raw_seek_lap");
		p[19] = GetProcAddress(hL, "ov_raw_tell");
		p[20] = GetProcAddress(hL, "ov_raw_total");
		p[21] = GetProcAddress(hL, "ov_read");
		p[22] = GetProcAddress(hL, "ov_read_filter");
		p[23] = GetProcAddress(hL, "ov_read_float");
		p[24] = GetProcAddress(hL, "ov_seekable");
		p[25] = GetProcAddress(hL, "ov_serialnumber");
		p[26] = GetProcAddress(hL, "ov_streams");
		p[27] = GetProcAddress(hL, "ov_test");
		p[28] = GetProcAddress(hL, "ov_test_callbacks");
		p[29] = GetProcAddress(hL, "ov_test_open");
		p[30] = GetProcAddress(hL, "ov_time_seek");
		p[31] = GetProcAddress(hL, "ov_time_seek_lap");
		p[32] = GetProcAddress(hL, "ov_time_seek_page");
		p[33] = GetProcAddress(hL, "ov_time_seek_page_lap");
		p[34] = GetProcAddress(hL, "ov_time_tell");
		p[35] = GetProcAddress(hL, "ov_time_total");


	}
	if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(hL);
	}
	DoVGSBEInit(reason);
	return 1;
}

// ov_bitrate
extern "C" __declspec(naked) void __stdcall __E__0__()
{
	__asm
	{
		jmp p[0 * 4];
	}
}

// ov_bitrate_instant
extern "C" __declspec(naked) void __stdcall __E__1__()
{
	__asm
	{
		jmp p[1 * 4];
	}
}

// ov_clear
extern "C" __declspec(naked) void __stdcall __E__2__()
{
	__asm
	{
		jmp p[2 * 4];
	}
}

// ov_comment
extern "C" __declspec(naked) void __stdcall __E__3__()
{
	__asm
	{
		jmp p[3 * 4];
	}
}

// ov_crosslap
extern "C" __declspec(naked) void __stdcall __E__4__()
{
	__asm
	{
		jmp p[4 * 4];
	}
}

// ov_fopen
extern "C" __declspec(naked) void __stdcall __E__5__()
{
	__asm
	{
		jmp p[5 * 4];
	}
}

// ov_halfrate
extern "C" __declspec(naked) void __stdcall __E__6__()
{
	__asm
	{
		jmp p[6 * 4];
	}
}

// ov_halfrate_p
extern "C" __declspec(naked) void __stdcall __E__7__()
{
	__asm
	{
		jmp p[7 * 4];
	}
}

// ov_info
extern "C" __declspec(naked) void __stdcall __E__8__()
{
	__asm
	{
		jmp p[8 * 4];
	}
}

// ov_open
extern "C" __declspec(naked) void __stdcall __E__9__()
{
	__asm
	{
		jmp p[9 * 4];
	}
}

// ov_open_callbacks
extern "C" __declspec(naked) void __stdcall __E__10__()
{
	__asm
	{
		jmp p[10 * 4];
	}
}

// ov_pcm_seek
extern "C" __declspec(naked) void __stdcall __E__11__()
{
	__asm
	{
		jmp p[11 * 4];
	}
}

// ov_pcm_seek_lap
extern "C" __declspec(naked) void __stdcall __E__12__()
{
	__asm
	{
		jmp p[12 * 4];
	}
}

// ov_pcm_seek_page
extern "C" __declspec(naked) void __stdcall __E__13__()
{
	__asm
	{
		jmp p[13 * 4];
	}
}

// ov_pcm_seek_page_lap
extern "C" __declspec(naked) void __stdcall __E__14__()
{
	__asm
	{
		jmp p[14 * 4];
	}
}

// ov_pcm_tell
extern "C" __declspec(naked) void __stdcall __E__15__()
{
	__asm
	{
		jmp p[15 * 4];
	}
}

// ov_pcm_total
extern "C" __declspec(naked) void __stdcall __E__16__()
{
	__asm
	{
		jmp p[16 * 4];
	}
}

// ov_raw_seek
extern "C" __declspec(naked) void __stdcall __E__17__()
{
	__asm
	{
		jmp p[17 * 4];
	}
}

// ov_raw_seek_lap
extern "C" __declspec(naked) void __stdcall __E__18__()
{
	__asm
	{
		jmp p[18 * 4];
	}
}

// ov_raw_tell
extern "C" __declspec(naked) void __stdcall __E__19__()
{
	__asm
	{
		jmp p[19 * 4];
	}
}

// ov_raw_total
extern "C" __declspec(naked) void __stdcall __E__20__()
{
	__asm
	{
		jmp p[20 * 4];
	}
}

// ov_read
extern "C" __declspec(naked) void __stdcall __E__21__()
{
	__asm
	{
		jmp p[21 * 4];
	}
}

// ov_read_filter
extern "C" __declspec(naked) void __stdcall __E__22__()
{
	__asm
	{
		jmp p[22 * 4];
	}
}

// ov_read_float
extern "C" __declspec(naked) void __stdcall __E__23__()
{
	__asm
	{
		jmp p[23 * 4];
	}
}

// ov_seekable
extern "C" __declspec(naked) void __stdcall __E__24__()
{
	__asm
	{
		jmp p[24 * 4];
	}
}

// ov_serialnumber
extern "C" __declspec(naked) void __stdcall __E__25__()
{
	__asm
	{
		jmp p[25 * 4];
	}
}

// ov_streams
extern "C" __declspec(naked) void __stdcall __E__26__()
{
	__asm
	{
		jmp p[26 * 4];
	}
}

// ov_test
extern "C" __declspec(naked) void __stdcall __E__27__()
{
	__asm
	{
		jmp p[27 * 4];
	}
}

// ov_test_callbacks
extern "C" __declspec(naked) void __stdcall __E__28__()
{
	__asm
	{
		jmp p[28 * 4];
	}
}

// ov_test_open
extern "C" __declspec(naked) void __stdcall __E__29__()
{
	__asm
	{
		jmp p[29 * 4];
	}
}

// ov_time_seek
extern "C" __declspec(naked) void __stdcall __E__30__()
{
	__asm
	{
		jmp p[30 * 4];
	}
}

// ov_time_seek_lap
extern "C" __declspec(naked) void __stdcall __E__31__()
{
	__asm
	{
		jmp p[31 * 4];
	}
}

// ov_time_seek_page
extern "C" __declspec(naked) void __stdcall __E__32__()
{
	__asm
	{
		jmp p[32 * 4];
	}
}

// ov_time_seek_page_lap
extern "C" __declspec(naked) void __stdcall __E__33__()
{
	__asm
	{
		jmp p[33 * 4];
	}
}

// ov_time_tell
extern "C" __declspec(naked) void __stdcall __E__34__()
{
	__asm
	{
		jmp p[34 * 4];
	}
}

// ov_time_total
extern "C" __declspec(naked) void __stdcall __E__35__()
{
	__asm
	{
		jmp p[35 * 4];
	}
}

