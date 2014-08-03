#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "VGSBE.lib")
void DoVGSBEInit(DWORD Reason);


typedef BOOL *(WINAPI* DllMainType)(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved);
typedef void *(*XInputEnableType)(BOOL enable);
typedef DWORD *(*XInputGetAudioDeviceIdsType)(DWORD dwUserIndex,LPWSTR pRenderDeviceId,UINT *pRenderCount,LPWSTR pCaptureDeviceId,UINT *pCaptureCount);
typedef DWORD *(*XInputGetBatteryInformationType)(DWORD dwUserIndex,BYTE devType,XINPUT_BATTERY_INFORMATION *pBatteryInformation);
typedef DWORD *(*XInputGetCapabilitiesType)(DWORD dwUserIndex,DWORD dwFlags,XINPUT_CAPABILITIES *pCapabilities);
typedef DWORD *(*XInputGetDSoundAudioDeviceGuidsType)(DWORD dwUserIndex,GUID* pDSoundRenderGuid,GUID* pDSoundCaptureGuid);
typedef DWORD *(*XInputGetKeystrokeType)(DWORD dwUserIndex,DWORD dwReserved,PXINPUT_KEYSTROKE pKeystroke);
typedef DWORD *(*XInputGetStateType)(DWORD dwUserIndex,XINPUT_STATE *pState);
typedef DWORD *(*XInputSetStateType)(DWORD dwUserIndex,XINPUT_VIBRATION *pVibration);

namespace Original
{	
	HMODULE module= NULL;
	DllMainType	xDllMain;
	XInputEnableType xXInputEnable;
	XInputGetAudioDeviceIdsType xXInputGetAudioDeviceIds;
	XInputGetBatteryInformationType xXInputGetBatteryInformation;
	XInputGetCapabilitiesType xXInputGetCapabilities;
	XInputGetDSoundAudioDeviceGuidsType xXInputGetDSoundAudioDeviceGuids;
	XInputGetKeystrokeType xXInputGetKeystroke;
	XInputGetStateType xXInputGetState;
	XInputSetStateType xXInputSetState;
};

HMODULE Injected = NULL;
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	if (fdwReason == DLL_PROCESS_ATTACH)
	{
#if defined _DEBUG
		while (!IsDebuggerPresent()){}
#endif
		Original::xDllMain = (DllMainType)GetProcAddress(Original::module, "DllMain");
		Original::xXInputEnable = (XInputEnableType)GetProcAddress(Original::module, "XInputEnable");
		Original::xXInputGetAudioDeviceIds = (XInputGetAudioDeviceIdsType)GetProcAddress(Original::module, "XInputGetAudioDeviceIds");
		Original::xXInputGetBatteryInformation = (XInputGetBatteryInformationType)GetProcAddress(Original::module, "XInputGetBatteryInformation");
		Original::xXInputGetCapabilities = (XInputGetCapabilitiesType)GetProcAddress(Original::module, "XInputGetCapabilities");
		Original::xXInputGetDSoundAudioDeviceGuids = (XInputGetDSoundAudioDeviceGuidsType)GetProcAddress(Original::module, "XInputGetDSoundAudioDeviceGuids");
		Original::xXInputGetKeystroke = (XInputGetKeystrokeType)GetProcAddress(Original::module, "XInputGetKeystroke");
		Original::xXInputGetState = (XInputGetStateType)GetProcAddress(Original::module, "XInputGetState");
		Original::xXInputSetState = (XInputSetStateType)GetProcAddress(Original::module, "XInputSetState");
	}
	DoVGSBEInit(fdwReason);
	return true;
}

void mXInputEnable(BOOL enable)
{
	return (void)Original::xXInputEnable(enable);
}

DWORD mXInputGetAudioDeviceIds(DWORD dwUserIndex,LPWSTR pRenderDeviceId, UINT *pRenderCount,LPWSTR pCaptureDeviceId,UINT *pCaptureCount)
{
	return (DWORD)Original::xXInputGetAudioDeviceIds(dwUserIndex,pRenderDeviceId,pRenderCount,pCaptureDeviceId,pCaptureCount);
}

DWORD mXInputGetBatteryInformation(DWORD dwUserIndex,BYTE devType,XINPUT_BATTERY_INFORMATION *pBatteryInformation)
{
	return (DWORD)Original::xXInputGetBatteryInformation(dwUserIndex,devType,pBatteryInformation);
}

DWORD mXInputGetCapabilities(DWORD dwUserIndex,DWORD dwFlags,XINPUT_CAPABILITIES *pCapabilities)
{
	return (DWORD)Original::xXInputGetCapabilities(dwUserIndex,dwFlags,pCapabilities);
}

DWORD WINAPI mXInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex,GUID* pDSoundRenderGuid,GUID* pDSoundCaptureGuid)
{
	return (DWORD)Original::xXInputGetDSoundAudioDeviceGuids(dwUserIndex,pDSoundRenderGuid,pDSoundCaptureGuid);
}

DWORD WINAPI mXInputGetKeystroke(DWORD dwUserIndex,DWORD dwReserved,PXINPUT_KEYSTROKE pKeystroke)
{
	return (DWORD)Original::xXInputGetKeystroke(dwUserIndex,dwReserved,pKeystroke);
}

DWORD mXInputGetState(DWORD dwUserIndex,XINPUT_STATE *pState)
{
	return (DWORD)Original::xXInputGetState(dwUserIndex,pState);
}

DWORD mXInputSetState(DWORD dwUserIndex,XINPUT_VIBRATION *pVibration)
{
	return (DWORD)Original::xXInputSetState(dwUserIndex,pVibration);
}