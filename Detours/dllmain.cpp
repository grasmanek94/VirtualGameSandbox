#include "all.h"
#include <Shlobj.h>
#include <locale>
#include <codecvt>

std::string ws2s(const std::wstring& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

HRESULT(WINAPI *savedSHGetKnownFolderPath)(
	_In_      REFKNOWNFOLDERID rfid,
	_In_      DWORD dwFlags,
	_In_opt_  HANDLE hToken,
	_Out_     PWSTR *ppszPath
	) = SHGetKnownFolderPath;

HRESULT WINAPI hookSHGetKnownFolderPath
(_In_      REFKNOWNFOLDERID rfid,
_In_      DWORD dwFlags,
_In_opt_  HANDLE hToken,
_Out_     PWSTR *ppszPath)
{
	HRESULT x = savedSHGetKnownFolderPath(rfid, dwFlags, hToken, ppszPath);
//#if defined _DEBUG
	MessageBoxA(NULL, string_format("rfid(%s)|dwFlags(%s)|hToken(%d)|ppszPath(%s)", rfid, dwFlags, hToken, ws2s((wchar_t*)ppszPath).c_str()).c_str(), "hookSHGetKnownFolderPath", 0);
//#endif
	return x;
}

INT APIENTRY ALtDllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	switch(Reason)
	{
	case DLL_PROCESS_ATTACH:	//Do standard detouring
#if defined _DEBUG && _WAITFORDEBUGGER
		while (!IsDebuggerPresent()){}
#endif
		DisableThreadLibraryCalls(hDLL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)savedSHGetKnownFolderPath, (PVOID)hookSHGetKnownFolderPath);
		if (!DetourTransactionCommit())
		{
#if defined _DEBUG
			MessageBoxA(NULL, "Injection success", "Inject Info", 0);
#endif
		}
#if defined _DEBUG
		else MessageBoxA(NULL, "Injection fail", "Inject Info", 0);
#endif
		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)savedSHGetKnownFolderPath, (PVOID)hookSHGetKnownFolderPath);
		DetourTransactionCommit();
		break;
	}
	return TRUE;
}

void DoVGSBEInit(DWORD Reason)
{
	ALtDllMain(NULL, Reason, NULL);
}

std::string string_format(const std::string fmt, ...)
{
	int size = 512;
	std::string str;
	va_list ap;
	while (1) {
		str.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			str.resize(n);
			return str;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return str;
}

int hookerrors = 0;