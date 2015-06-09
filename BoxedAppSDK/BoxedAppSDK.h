// Copyright (c) Softanics
//
// BoxedApp SDK
//
// Ask your questions here: http://boxedapp.com/support.html
// Our forum: http://boxedapp.com/forum/
// License SDK: http://boxedapp.com/boxedappsdk/order.html
// Online help: http://boxedapp.com/boxedappsdk/help/
//

#ifndef __BOXEDAPPSDK_H__
#define __BOXEDAPPSDK_H__

#define BOXEDAPPSDKAPI __stdcall

#include <pshpack4.h>

// IStream* declaration
#include <objidl.h>

#include "BoxedAppSDK_Interfaces.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Options
#define DEF_BOXEDAPPSDK_OPTION__ALL_CHANGES_ARE_VIRTUAL                (1) // default: 0 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__EMBED_BOXEDAPP_IN_CHILD_PROCESSES      (2) // default: 0 (FALSE, don't enable BoxedApp to a new process by default)
#define DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_FILE_SYSTEM             (3) // default: 1 (TRUE)
#define DEF_BOXEDAPPSDK_OPTION__RECREATE_VIRTUAL_FILE_AS_VIRTUAL       (4) // default: 1 (TRUE)
#define DEF_BOXEDAPPSDK_OPTION__ENABLE_VIRTUAL_REGISTRY                (5) // default: 1 (TRUE)
#define DEF_BOXEDAPPSDK_OPTION__HIDE_VIRTUAL_FILES_FROM_FILE_DIALOG    (6) // default: 0 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__EMULATE_OUT_OF_PROC_COM_SERVERS        (7) // default: 0 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__INHERIT_OPTIONS                        (8) // default: 1 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__ENABLE_REGISTRY_RMSC_REDIRECTOR        (9) // default: 0 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__ENABLE_ALL_HOOKS                      (10) // default: 1 (TRUE)
#define DEF_BOXEDAPPSDK_OPTION__NTACCESSCHECK_ALWAYS_RETURNS_SUCCESS  (11) // default: 0 (FALSE)
#define DEF_BOXEDAPPSDK_OPTION__ENABLE_EXPERIMENTAL_IMPORT_PROCESSING (12) // default: 0 (FALSE)

// Startup flags
#define DEF_BOXEDAPPSDK_STARTUP_OPTION__HOOK_ENGINE                    (1) // default: 0 (FALSE)

/// Isolation mode enumeration
typedef enum _BX_ISOLATION_MODE
{
    // No isolation specified
    BxIsolationMode_None, 

    // File system: 
    //
    // When enumerating, process views files from the destination directory only
    // Modifications of files in the destination directory are saved in the destination directory
    // Files from the source directory are not reading or writing
    // New files are created in the destination directory
    //
    // Registry:
    //
    // Process works with persistent registry only, reading and writing are passed
    // persistent registry only; real registry is not asked at all
    BxIsolationMode_Full, 

    // When enumerating, process views files from source 
    // directory plus files from destination directory, 
    // but if the same files are in both, then file in 
    // the destination directory gets priority
    //
    // Modifications of files in the destination directory 
    // are saved in the destination directory
    //
    // Modifications of files in the source directory are 
    // saved to this files
    //
    // New files are created in the destination directory
    //
    // Registry:
    //
    // Process views subkeys / values of persistent registry and real registry; if keys are present
    // in both registries, the key of persistent registry is included
    //
    // New keys are created in persistent registry
    BxIsolationMode_Merged, 

    // When enumerating, process views files from source 
    // directory plus files from destination directory, 
    // but if the same files are in both, then file in 
    // the destination directory gets priority
    //
    // Modifications of files in the destination directory 
    // are saved in the destination directory
    //
    // Modifications of files in the source directory are 
    // saved to the destination directory
    //
    // New files are created in the destination directory
    //
    // Registry:
    //
    // Process views subkeys / values of persistent registry and real registry; if keys are present
    // in both registries, the key of persistent registry is included
    //
    // New keys are created in persistent registry
    //
    // If real real is opened and one writes to the key, the virtual key is created; so real
    // registry is not changed
    BxIsolationMode_WriteCopy
} BX_ISOLATION_MODE;

void WINAPI BoxedAppSDK_PackerStubEntryPoint(HMODULE, PVOID, DWORD);

// Initialization
BOOL BOXEDAPPSDKAPI BoxedAppSDK_Init();
// Internal: initialization by environment id
BOOL BOXEDAPPSDKAPI BoxedAppSDK_InitById(ULONGLONG nEnvId);
// Finalization
void BOXEDAPPSDKAPI BoxedAppSDK_Exit();

/// A structure that is used for EnumBoxedAppSDK_RequestId__RedirectFilePath
struct SBoxedAppSDK__RedirectFilePath
{
    // in
    LPCWSTR m_szPath;

    // out
    BOOL m_bHandled;

    LPWSTR m_szRedirectToPath;
};

// Internal: do attach process to virtual environment
void CALLBACK BoxedAppSDK_AttachMixedBitnessProcessHelper();

// Internal: do run virtual process to virtual environment
void CALLBACK BoxedAppSDK_TryCreateProcessForVirtualEXE_AnotherBitnessPartHelper();

// Enable / disable logging
void BOXEDAPPSDKAPI BoxedAppSDK_EnableDebugLog(BOOL bEnable);

// Log file
void BOXEDAPPSDKAPI BoxedAppSDK_SetLogFileA(LPCSTR szLogFilePath);
void BOXEDAPPSDKAPI BoxedAppSDK_SetLogFileW(LPCWSTR szLogFilePath);

#ifdef UNICODE
#define BoxedAppSDK_SetLogFile BoxedAppSDK_SetLogFileW
#else
#define BoxedAppSDK_SetLogFile BoxedAppSDK_SetLogFileA
#endif // UNICODE

void WINAPI BoxedAppSDK_WriteLogA(LPCSTR szMessage);
void WINAPI BoxedAppSDK_WriteLogW(LPCWSTR szMessage);

#ifdef UNICODE
#define BoxedAppSDK_WriteLog BoxedAppSDK_WriteLogW
#else
#define BoxedAppSDK_WriteLog BoxedAppSDK_WriteLogA
#endif // UNICODE

// Virtual file system

#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualFile BoxedAppSDK_CreateVirtualFileW
#else
#define BoxedAppSDK_CreateVirtualFile BoxedAppSDK_CreateVirtualFileA
#endif // UNICODE

// Create new virtual file
HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileA(
    LPCSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileW(
    LPCWSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
);


#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualDirectory BoxedAppSDK_CreateVirtualDirectoryW
#else
#define BoxedAppSDK_CreateVirtualDirectory BoxedAppSDK_CreateVirtualDirectoryA
#endif // UNICODE

// Create new virtual directory
BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualDirectoryA(
    LPCSTR lpPathName, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualDirectoryW(
    LPCWSTR lpPathName, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

#ifdef UNICODE
#define BoxedAppSDK_IsVirtualFile BoxedAppSDK_IsVirtualFileW
#else
#define BoxedAppSDK_IsVirtualFile BoxedAppSDK_IsVirtualFileA
#endif // UNICODE

// Create new virtual directory
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualFileA(
    LPCSTR szPath
);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualFileW(
    LPCWSTR szPath
);

// Virtual registry

#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualRegKey BoxedAppSDK_CreateVirtualRegKeyW
#else
#define BoxedAppSDK_CreateVirtualRegKey BoxedAppSDK_CreateVirtualRegKeyA
#endif // UNICODE

LONG BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualRegKeyA(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD Reserved,
    LPCSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
);

LONG BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualRegKeyW(
    HKEY hKey,
    LPCWSTR lpSubKey,
    DWORD Reserved,
    LPCWSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
);

// Isolation mode for registry keys

#ifdef UNICODE
#define BoxedAppSDK_SetRegKeyIsolationMode BoxedAppSDK_SetRegKeyIsolationModeW
#else
#define BoxedAppSDK_SetRegKeyIsolationMode BoxedAppSDK_SetRegKeyIsolationModeA
#endif // UNICODE

/// Sets isolation mode for the key
/// To specify registry view use samDesired (pass KEY_WOW64_32KEY or KEY_WOW64_64KEY), pass zero to don't specify registry view
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetRegKeyIsolationModeW(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired, BX_ISOLATION_MODE IsolationMode);

/// Sets isolation mode for the key
/// To specify registry view use samDesired (pass KEY_WOW64_32KEY or KEY_WOW64_64KEY), pass zero to don't specify registry view
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetRegKeyIsolationModeA(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired, BX_ISOLATION_MODE IsolationMode);

#ifdef UNICODE
#define BoxedAppSDK_GetRegKeyIsolationMode BoxedAppSDK_GetRegKeyIsolationModeW
#else
#define BoxedAppSDK_GetRegKeyIsolationMode BoxedAppSDK_GetRegKeyIsolationModeA
#endif // UNICODE

/// Gets isolation mode for the key
/// To specify registry view use samDesired (pass KEY_WOW64_32KEY or KEY_WOW64_64KEY), pass zero to don't specify registry view
BX_ISOLATION_MODE BOXEDAPPSDKAPI BoxedAppSDK_GetRegKeyIsolationModeW(HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired);

/// Gets isolation mode for the key
/// To specify registry view use samDesired (pass KEY_WOW64_32KEY or KEY_WOW64_64KEY), pass zero to don't specify registry view
BX_ISOLATION_MODE BOXEDAPPSDKAPI BoxedAppSDK_GetRegKeyIsolationModeA(HKEY hKey, LPCSTR lpSubKey, REGSAM samDesired);

/// Set persistent registry data file path
/// Call it before BoxedAppSDK_Init()
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetPersistentRegistryPathW(LPCWSTR szPersistentRegistryFilePath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetPersistentRegistryPathA(LPCSTR szPersistentRegistryFilePath);

#ifdef UNICODE
#define BoxedAppSDK_SetPersistentRegistryPath BoxedAppSDK_SetPersistentRegistryPathW
#else
#define BoxedAppSDK_SetPersistentRegistryPath BoxedAppSDK_SetPersistentRegistryPathA
#endif // UNICODE

// Adding virtual registry key from the virtual environment

#ifdef UNICODE
#define BoxedAppSDK_AddVirtualRegKey BoxedAppSDK_AddVirtualRegKeyW
#else
#define BoxedAppSDK_AddVirtualRegKey BoxedAppSDK_AddVirtualRegKeyA
#endif // UNICODE

LONG BOXEDAPPSDKAPI BoxedAppSDK_AddVirtualRegKeyA(HKEY hKey, LPCSTR lpSubKey);
LONG BOXEDAPPSDKAPI BoxedAppSDK_AddVirtualRegKeyW(HKEY hKey, LPCWSTR lpSubKey);

// Removing virtual registry key from the virtual environment

#ifdef UNICODE
#define BoxedAppSDK_DeleteVirtualRegKey BoxedAppSDK_DeleteVirtualRegKeyW
#else
#define BoxedAppSDK_DeleteVirtualRegKey BoxedAppSDK_DeleteVirtualRegKeyA
#endif // UNICODE

LONG BOXEDAPPSDKAPI BoxedAppSDK_DeleteVirtualRegKeyA(HKEY hKey, LPCSTR lpSubKey);
LONG BOXEDAPPSDKAPI BoxedAppSDK_DeleteVirtualRegKeyW(HKEY hKey, LPCWSTR lpSubKey);

LONG BOXEDAPPSDKAPI BoxedAppSDK_DeleteVirtualRegKeyByHandle(HKEY hKey);

void BOXEDAPPSDKAPI BoxedAppSDK_SetContext(LPCSTR szContext);

DWORD BOXEDAPPSDKAPI BoxedAppSDK_RegisterCOMLibraryInVirtualRegistryA(LPCSTR szPath);
DWORD BOXEDAPPSDKAPI BoxedAppSDK_RegisterCOMLibraryInVirtualRegistryW(LPCWSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_RegisterCOMLibraryInVirtualRegistry BoxedAppSDK_RegisterCOMLibraryInVirtualRegistryW
#else
#define BoxedAppSDK_RegisterCOMLibraryInVirtualRegistry BoxedAppSDK_RegisterCOMLibraryInVirtualRegistryA
#endif // UNICODE

void BOXEDAPPSDKAPI BoxedAppSDK_EnableOption(DWORD dwOptionIndex, BOOL bEnable);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsOptionEnabled(DWORD dwOptionIndex);

void BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_EnableOption(DWORD dwProcessId, DWORD dwOptionIndex, BOOL bEnable);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_IsOptionEnabled(DWORD dwProcessId, DWORD dwOptionIndex);

// Set startup flags
void BOXEDAPPSDKAPI BoxedAppSDK_SetStartupFlags(DWORD dwFlags);

typedef enum _ENUM_BOXEDAPPSDK_REQUEST_ID
{
    EnumBoxedAppSDK_RequestId__RegQueryValue = 1, 
    EnumBoxedAppSDK_RequestId__RedirectFilePath = 2
} ENUM_BOXEDAPPSDK_REQUEST_ID;

typedef DWORD (BOXEDAPPSDKAPI *PBOXEDAPPHANDLER)(PVOID Param, ENUM_BOXEDAPPSDK_REQUEST_ID RequestId, PVOID pAdditionalInfo);
DWORD BOXEDAPPSDKAPI BoxedAppSDK_AddHandler(PBOXEDAPPHANDLER pHandler, PVOID Param);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveHandler(DWORD dwHandlerId);

/// A structure that is used for EnumBoxedAppSDK_RequestId__RegQueryValue
struct SBoxedAppSDK__RegQueryValue
{
    // in
    HKEY m_Root;
    LPCWSTR m_szPath;
    LPCWSTR m_szValue;

    // out
    BOOL m_bHandled;

    DWORD m_dwType;
    PVOID m_pData;
    DWORD m_dwSize;
};

PVOID BOXEDAPPSDKAPI BoxedAppSDK_Alloc(DWORD dwSize);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_Free(PVOID pData);

DWORD BOXEDAPPSDKAPI BoxedAppSDK_DeleteFileFromVirtualFileSystemW(LPCWSTR szPath);
DWORD BOXEDAPPSDKAPI BoxedAppSDK_DeleteFileFromVirtualFileSystemA(LPCSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_DeleteFileFromVirtualFileSystem BoxedAppSDK_DeleteFileFromVirtualFileSystemW
#else
#define BoxedAppSDK_DeleteFileFromVirtualFileSystem BoxedAppSDK_DeleteFileFromVirtualFileSystemA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateProcessFromMemoryA(
    LPCVOID pBuffer, 
    DWORD dwSize, 

    LPCSTR lpApplicationName,
    LPSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCSTR lpCurrentDirectory,
    LPSTARTUPINFOA lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateProcessFromMemoryW(
    LPCVOID pBuffer, 
    DWORD dwSize, 

    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
);

#ifdef UNICODE
#define BoxedAppSDK_CreateProcessFromMemory BoxedAppSDK_CreateProcessFromMemoryW
#else
#define BoxedAppSDK_CreateProcessFromMemory BoxedAppSDK_CreateProcessFromMemoryA
#endif // UNICODE

// Set param #0
void BOXEDAPPSDKAPI BoxedAppSDK_SetParam0(LPCWSTR param);

// Create a virtual file based on IStream
HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileBasedOnIStreamA(
    LPCSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile, 

    LPSTREAM pStream
);

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileBasedOnIStreamW(
    LPCWSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile, 

    LPSTREAM pStream
);

#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualFileBasedOnIStream BoxedAppSDK_CreateVirtualFileBasedOnIStreamW
#else
#define BoxedAppSDK_CreateVirtualFileBasedOnIStream BoxedAppSDK_CreateVirtualFileBasedOnIStreamA
#endif // UNICODE

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileBasedOnBufferA(
    LPCSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile, 

    PVOID pData, DWORD dwSize
);

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileBasedOnBufferW(
    LPCWSTR szPath, 
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile, 

    PVOID pData, DWORD dwSize
);

#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualFileBasedOnBuffer BoxedAppSDK_CreateVirtualFileBasedOnBufferW
#else
#define BoxedAppSDK_CreateVirtualFileBasedOnBuffer BoxedAppSDK_CreateVirtualFileBasedOnBufferA
#endif // UNICODE

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileOnFilePartW(
    LPCWSTR szPath,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile,

    LPCWSTR szSourcePath,
    DWORD dwOffsetLow,
    DWORD dwOffsetHigh,
    DWORD dwSizeLow,
    DWORD dwSizeHigh);

HANDLE BOXEDAPPSDKAPI BoxedAppSDK_CreateVirtualFileOnFilePartA(
    LPCSTR szPath,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile,

    LPCSTR szSourcePath,
    DWORD dwOffsetLow,
    DWORD dwOffsetHigh,
    DWORD dwSizeLow,
    DWORD dwSizeHigh);

#ifdef UNICODE
#define BoxedAppSDK_CreateVirtualFileOnFilePart BoxedAppSDK_CreateVirtualFileOnFilePartW
#else
#define BoxedAppSDK_CreateVirtualFileOnFilePart BoxedAppSDK_CreateVirtualFileOnFilePartA
#endif // UNICODE

// Get parameter for hooked function
LPVOID BOXEDAPPSDKAPI BoxedAppSDK_GetHookParam();

// Attach BoxedApp SDK to another process
BOOL BOXEDAPPSDKAPI BoxedAppSDK_AttachToProcess(HANDLE hProcess);
// Detach BoxedApp SDK from a process
BOOL BOXEDAPPSDKAPI BoxedAppSDK_DetachFromProcess(HANDLE hProcess);
//
BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddStartupDLLW(LPCWSTR szPath);
//
BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddStartupDLLA(LPCSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_AddStartupDLL BoxedAppSDK_AddStartupDLLW
#else
#define BoxedAppSDK_AddStartupDLL BoxedAppSDK_AddStartupDLLA
#endif // UNICODE

#ifdef UNICODE
#define BoxedAppSDK_RemoveStartupDLL BoxedAppSDK_RemoveStartupDLLW
#else
#define BoxedAppSDK_RemoveStartupDLL BoxedAppSDK_RemoveStartupDLLA
#endif // UNICODE
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveStartupDLLW(LPCWSTR szPath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveStartupDLLA(LPCSTR szPath);


BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddExeToAttachableChildProcExclusionListA(LPCSTR szExeName);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddExeToAttachableChildProcExclusionListW(LPCWSTR szExeName);

#ifdef UNICODE
#define BoxedAppSDK_AddExeToAttachableChildProcExclusionList BoxedAppSDK_AddExeToAttachableChildProcExclusionListW
#else
#define BoxedAppSDK_AddExeToAttachableChildProcExclusionList BoxedAppSDK_AddExeToAttachableChildProcExclusionListA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionListA(LPCSTR szExeName);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionListW(LPCWSTR szExeName);

#ifdef UNICODE
#define BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionList BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionListW
#else
#define BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionList BoxedAppSDK_RemoveExeFromAttachableChildProcExclusionListA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddExeToAttachableChildProcListA(LPCSTR szExeName);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddExeToAttachableChildProcListW(LPCWSTR szExeName);

#ifdef UNICODE
#define BoxedAppSDK_AddExeToAttachableChildProcList BoxedAppSDK_AddExeToAttachableChildProcListW
#else
#define BoxedAppSDK_AddExeToAttachableChildProcList BoxedAppSDK_AddExeToAttachableChildProcListA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveExeFromAttachableChildProcListA(LPCSTR szExeName);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveExeFromAttachableChildProcListW(LPCWSTR szExeName);

#ifdef UNICODE
#define BoxedAppSDK_RemoveExeFromAttachableChildProcList BoxedAppSDK_RemoveExeFromAttachableChildProcListW
#else
#define BoxedAppSDK_RemoveExeFromAttachableChildProcList BoxedAppSDK_RemoveExeFromAttachableChildProcListA
#endif // UNICODE

// Execute .net application
DWORD BOXEDAPPSDKAPI BoxedAppSDK_ExecuteDotNetApplicationW(LPCWSTR szPath, LPCWSTR szArgs);
DWORD BOXEDAPPSDKAPI BoxedAppSDK_ExecuteDotNetApplicationA(LPCSTR szPath, LPCSTR szArgs);

#ifdef UNICODE
#define BoxedAppSDK_ExecuteDotNetApplication BoxedAppSDK_ExecuteDotNetApplicationW
#else
#define BoxedAppSDK_ExecuteDotNetApplication BoxedAppSDK_ExecuteDotNetApplicationA
#endif // UNICODE

// Internal; don't use
DWORD BOXEDAPPSDKAPI BoxedAppSDK_GetInternalValue(DWORD nValueId, LPVOID buf, DWORD dwSize);

// Function hooking
HANDLE BOXEDAPPSDKAPI BoxedAppSDK_HookFunctionWithParam(PVOID pFunction, PVOID pHook, LPVOID pParam, BOOL bEnable);
HANDLE BOXEDAPPSDKAPI BoxedAppSDK_HookFunction(PVOID pFunction, PVOID pHook, BOOL bEnable);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetHookParam(HANDLE pHook, LPVOID pParam);
PVOID BOXEDAPPSDKAPI BoxedAppSDK_GetOriginalFunction(HANDLE hHook);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_EnableHook(HANDLE hHook, BOOL bEnable);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_UnhookFunction(HANDLE hHook);

#ifndef DWORD_PTR
#define DWORD_PTR SIZE_T
#endif // !DWORD_PTR

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_ReadStringW(DWORD dwProcessId, const VOID* pAddress, LPWSTR* pString);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_ReadStringA(DWORD dwProcessId, const VOID* pAddress, LPSTR* pString);

#ifdef UNICODE
#define BoxedAppSDK_RemoteProcess_ReadString BoxedAppSDK_RemoteProcess_ReadStringW
#else
#define BoxedAppSDK_RemoteProcess_ReadString BoxedAppSDK_RemoteProcess_ReadStringA
#endif // UNICODE

PVOID BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_Alloc(DWORD dwProcessId, DWORD dwSize);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_Free(DWORD dwProcessId, PVOID pMemory);

PVOID BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_AllocStringW(DWORD dwProcessId, LPCWSTR szSource);
PVOID BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_AllocStringA(DWORD dwProcessId, LPCSTR szSource);

#ifdef UNICODE
#define BoxedAppSDK_RemoteProcess_AllocString BoxedAppSDK_RemoteProcess_AllocStringW
#else
#define BoxedAppSDK_RemoteProcess_AllocString BoxedAppSDK_RemoteProcess_AllocStringA
#endif // UNICODE

HMODULE BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_LoadLibraryA(DWORD dwProcessId, LPCSTR szPath);
HMODULE BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_LoadLibraryW(DWORD dwProcessId, LPCWSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_RemoteProcess_LoadLibrary BoxedAppSDK_RemoteProcess_LoadLibraryW
#else
#define BoxedAppSDK_RemoteProcess_LoadLibrary BoxedAppSDK_RemoteProcess_LoadLibraryA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoteProcess_FreeLibrary(DWORD dwProcessId, HMODULE hModule);

DWORD BOXEDAPPSDKAPI BoxedAppSDK_RegisterCOMServerInVirtualRegistryA(LPCSTR szCommandLine);
DWORD BOXEDAPPSDKAPI BoxedAppSDK_RegisterCOMServerInVirtualRegistryW(LPCWSTR szCommandLine);

#ifdef UNICODE
#define BoxedAppSDK_RegisterCOMServerInVirtualRegistry BoxedAppSDK_RegisterCOMServerInVirtualRegistryW
#else
#define BoxedAppSDK_RegisterCOMServerInVirtualRegistry BoxedAppSDK_RegisterCOMServerInVirtualRegistryA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsMainProcess();
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualProcessStub();
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualProcessId(DWORD dwProcessId);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsAttachedProcessId(DWORD dwProcessId);

typedef BOOL (WINAPI *P_BoxedAppSDK_EnumVirtualRegKeysCallbackA)(HKEY hRootKey, LPCSTR szSubKey, LPARAM lParam);
typedef BOOL (WINAPI *P_BoxedAppSDK_EnumVirtualRegKeysCallbackW)(HKEY hRootKey, LPCWSTR szSubKey, LPARAM lParam);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_EnumVirtualRegKeysA(P_BoxedAppSDK_EnumVirtualRegKeysCallbackA pEnumFunc, LPARAM lParam);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_EnumVirtualRegKeysW(P_BoxedAppSDK_EnumVirtualRegKeysCallbackW pEnumFunc, LPARAM lParam);

#ifdef UNICODE
#define BoxedAppSDK_EnumVirtualRegKeys BoxedAppSDK_EnumVirtualRegKeysW
#define P_BoxedAppSDK_EnumVirtualRegKeysCallback P_BoxedAppSDK_EnumVirtualRegKeysCallbackW
#else
#define BoxedAppSDK_EnumVirtualRegKeys BoxedAppSDK_EnumVirtualRegKeysA
#define P_BoxedAppSDK_EnumVirtualRegKeysCallback P_BoxedAppSDK_EnumVirtualRegKeysCallbackA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateCustomVirtualDirectoryA(
    LPCSTR lpPathName, 
    IBxFile* pFile, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_CreateCustomVirtualDirectoryW(
    LPCWSTR lpPathName, 
    IBxFile* pFile, 
    LPSECURITY_ATTRIBUTES lpSecurityAttributes);

#ifdef UNICODE
#define BoxedAppSDK_CreateCustomVirtualDirectory BoxedAppSDK_CreateCustomVirtualDirectoryW
#else
#define BoxedAppSDK_CreateCustomVirtualDirectory BoxedAppSDK_CreateCustomVirtualDirectoryA
#endif // UNICODE

/// Retrieves global file isolation mode
BX_ISOLATION_MODE BOXEDAPPSDKAPI BoxedAppSDK_GetGlobalFileIsolationMode();

/// Sets global file isolation mode
/// Some file requests (depending on isolation mode) are passed to szDestinationPath
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetGlobalFileIsolationModeA(BX_ISOLATION_MODE IsolationMode, LPCSTR szDestinationPath);

/// Sets global file isolation mode
/// Some file requests (depending on isolation mode) are passed to szDestinationPath
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetGlobalFileIsolationModeW(BX_ISOLATION_MODE IsolationMode, LPCWSTR szDestinationPath);

#ifdef UNICODE
#define BoxedAppSDK_SetGlobalFileIsolationMode BoxedAppSDK_SetGlobalFileIsolationModeW
#else
#define BoxedAppSDK_SetGlobalFileIsolationMode BoxedAppSDK_SetGlobalFileIsolationModeA
#endif // UNICODE

/// Sets isolation mode for directory / file
/// Some file requests (depending on isolation mode) are passed from szRedirectedPath to szDestinationPath
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetFileIsolationModeW(BX_ISOLATION_MODE IsolationMode, LPCWSTR szRedirectedPath, LPCWSTR szDestinationPath);

/// Sets isolation mode for directory / file
/// Some file requests (depending on isolation mode) are passed from szRedirectedPath to szDestinationPath
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetFileIsolationModeA(BX_ISOLATION_MODE IsolationMode, LPCSTR szRedirectedPath, LPCSTR szDestinationPath);

#ifdef UNICODE
#define BoxedAppSDK_SetFileIsolationMode BoxedAppSDK_SetFileIsolationModeW
#else
#define BoxedAppSDK_SetFileIsolationMode BoxedAppSDK_SetFileIsolationModeA
#endif // UNICODE

/// Returns whether the handle virtual
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualHandle(HANDLE Handle);

// If bxsdk32.dll / bxsdk64.dll can't be loaded, it creates a virtual bxsdk32.dll / bxsdk64.dll, 
// all exports points to the correct code
// Useful when you link BoxedAppSDK statically, but some code (another DLL) needs bxsdk32.dll / bxsdk64.dll
// Also see function BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDir
void BOXEDAPPSDKAPI BoxedAppSDK_EmulateBoxedAppSDKDLL();

// Specifies a base directory where virtual bxsdk32.dll / bxsdk64.dll will be placed
// Actually for each process the virtual bxsdk32.dll / bxsdk64.dll is placed into <base dir>\\process_id\\
// Virtual bxsdk32.dll / bxsdk64.dll can be created when BoxedAppSDK_EmulateBoxedAppSDKDLL()
// is called, or when new process is begin attached to virtual environment (you know, 
// it's important to have a virtual bxsdk32.dll / bxsdk64.dll if main process uses
// static library of BoxedApp SDK, so real bxsdk32.dll / bxsdk64.dll doesn't exist; but
// attached process should be able to call functions of bxsdk32.dll / bxsdk64.dll -- that's
// the reason)
// It's not required to call this function; boxedapp can use a default value of 
// the directory
// Also see function BoxedAppSDK_EmulateBoxedAppSDKDLL
void BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDirA(LPCSTR szDir);
void BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDirW(LPCWSTR szDir);

#ifdef UNICODE
#define BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDir BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDirW
#else
#define BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDir BoxedAppSDK_SetVirtualBoxedAppSDKDLLBaseDirA
#endif // UNICODE

//
// Shared memory API
//

typedef LONGLONG BOXEDAPP_SHARED_PTR;

BOXEDAPP_SHARED_PTR BOXEDAPPSDKAPI BoxedAppSDK_SharedMem_Alloc(int nSize);
void BOXEDAPPSDKAPI BoxedAppSDK_SharedMem_Free(BOXEDAPP_SHARED_PTR shared_ptr);

PVOID BOXEDAPPSDKAPI BoxedAppSDK_SharedMem_Lock(BOXEDAPP_SHARED_PTR shared_ptr);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SharedMem_Unlock(BOXEDAPP_SHARED_PTR shared_ptr);

HRESULT BOXEDAPPSDKAPI BoxedAppSDK_SharedMem_CreateStreamOnSharedMem(LPSTREAM* ppStream);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddStubCandidateA(LPCSTR szStubPath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_AddStubCandidateW(LPCWSTR szStubPath);

#ifdef UNICODE
#define BoxedAppSDK_AddStubCandidate BoxedAppSDK_AddStubCandidateW
#else
#define BoxedAppSDK_AddStubCandidate BoxedAppSDK_AddStubCandidateA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveStubCandidateA(LPCSTR szStubPath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_RemoveStubCandidateW(LPCWSTR szStubPath);

#ifdef UNICODE
#define BoxedAppSDK_RemoveStubCandidate BoxedAppSDK_RemoveStubCandidateW
#else
#define BoxedAppSDK_RemoveStubCandidate BoxedAppSDK_RemoveStubCandidateA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualRegValueW(HKEY hKey, LPCWSTR szValueName);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IsVirtualRegValueA(HKEY hKey, LPCSTR szValueName);

#ifdef UNICODE
#define BoxedAppSDK_IsVirtualRegValue BoxedAppSDK_IsVirtualRegValueW
#else
#define BoxedAppSDK_IsVirtualRegValue BoxedAppSDK_IsVirtualRegValueA
#endif // UNICODE

// Virtual installing of assembly
BOOL BoxedAppSDK_InstallAssemblyA(LPCSTR szManifestPath, LPCSTR szPolicyPath);
BOOL BoxedAppSDK_InstallAssemblyW(LPCWSTR szManifestPath, LPCWSTR szPolicyPath);

#ifdef UNICODE
#define BoxedAppSDK_InstallAssembly BoxedAppSDK_InstallAssemblyW
#else
#define BoxedAppSDK_InstallAssembly BoxedAppSDK_InstallAssemblyA
#endif // UNICODE

// Specify path to bxsdk32.dll / bxsdk64.dll
// It's useful in the following case: 
// your application statically linked with BoxedApp SDK, so it doesn't require the DLL to work
// if a process that has another bitness should be attached to the virtual environment, BoxedApp SDK
// will try to use bxsdk32.dll (to attach 32 bit process) or bxsdk64.dll (to attach 64 bit process)
// But your application doesn't need the DLLs. To help BoxedApp SDK you could place 
// bxsdk32.dll / bxsdk64.dll somewhere and make BoxedApp SDK uses these paths
// If you just put bxsdk32.dll and bxsdk64.dll to the exe's directory - you don't need to call
// these functions: BoxedApp SDK will find these dlls automatically

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetBxSdk32DllPathW(LPCWSTR szPath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetBxSdk32DllPathA(LPCSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_SetBxSdk32DllPath BoxedAppSDK_SetBxSdk32DllPathW
#else
#define BoxedAppSDK_SetBxSdk32DllPath BoxedAppSDK_SetBxSdk32DllPathA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetBxSdk64DllPathW(LPCWSTR szPath);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetBxSdk64DllPathA(LPCSTR szPath);

#ifdef UNICODE
#define BoxedAppSDK_SetBxSdk64DllPath BoxedAppSDK_SetBxSdk64DllPathW
#else
#define BoxedAppSDK_SetBxSdk64DllPath BoxedAppSDK_SetBxSdk64DllPathA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeSerialW(LPCWSTR szPath, DWORD nVolumeSerial);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeSerialA(LPCSTR szPath, DWORD nVolumeSerial);

#ifdef UNICODE
#define BoxedAppSDK_SetVirtualVolumeSerial BoxedAppSDK_SetVirtualVolumeSerialW
#else
#define BoxedAppSDK_SetVirtualVolumeSerial BoxedAppSDK_SetVirtualVolumeSerialA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeCreationTimeW(LPCWSTR szPath, FILETIME nVolumeCreationTime);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeCreationTimeA(LPCSTR szPath, FILETIME nVolumeCreationTime);

#ifdef UNICODE
#define BoxedAppSDK_SetVirtualVolumeCreationTime BoxedAppSDK_SetVirtualVolumeCreationTimeW
#else
#define BoxedAppSDK_SetVirtualVolumeCreationTime BoxedAppSDK_SetVirtualVolumeCreationTimeA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeNameW(LPCWSTR szPath, LPCWSTR szVolumeName);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetVirtualVolumeNameA(LPCSTR szPath, LPCSTR szVolumeName);

#ifdef UNICODE
#define BoxedAppSDK_SetVirtualVolumeName BoxedAppSDK_SetVirtualVolumeNameW
#else
#define BoxedAppSDK_SetVirtualVolumeName BoxedAppSDK_SetVirtualVolumeNameA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_SetBxSdkRawData(PVOID pData, DWORD dwSize);

typedef void (WINAPI* P_BoxedAppSDK_IPC_Listener)(PVOID pRequestData, DWORD dwRequestSize);

VOID BOXEDAPPSDKAPI BoxedAppSDK_IPC_AddListenerW(P_BoxedAppSDK_IPC_Listener pListener, LPCWSTR szMessageClass);
VOID BOXEDAPPSDKAPI BoxedAppSDK_IPC_AddListenerA(P_BoxedAppSDK_IPC_Listener pListener, LPCSTR szMessageClass);

#ifdef UNICODE
#define BoxedAppSDK_IPC_AddListener BoxedAppSDK_IPC_AddListenerW
#else
#define BoxedAppSDK_IPC_AddListener BoxedAppSDK_IPC_AddListenerA
#endif // UNICODE

VOID BOXEDAPPSDKAPI BoxedAppSDK_IPC_RemoveListener(P_BoxedAppSDK_IPC_Listener pListener);

BOOL BOXEDAPPSDKAPI BoxedAppSDK_IPC_SendMessageW(LPCWSTR szMessageClass, LPCVOID lpcMessage, DWORD dwMessageSize);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IPC_SendMessageA(LPCSTR szMessageClass, LPCVOID lpcMessage, DWORD dwMessageSize);

#ifdef UNICODE
#define BoxedAppSDK_IPC_SendMessage BoxedAppSDK_IPC_SendMessageW
#else
#define BoxedAppSDK_IPC_SendMessage BoxedAppSDK_IPC_SendMessageA
#endif // UNICODE

BOOL BOXEDAPPSDKAPI BoxedAppSDK_IPC_SendMessageToProcessW(DWORD dwPid, LPCWSTR szMessageClass, LPCVOID lpcMessage, DWORD dwMessageSize);
BOOL BOXEDAPPSDKAPI BoxedAppSDK_IPC_SendMessageToProcessA(DWORD dwPid, LPCSTR szMessageClass, LPCVOID lpcMessage, DWORD dwMessageSize);

#ifdef UNICODE
#define BoxedAppSDK_IPC_SendMessageToProcess BoxedAppSDK_IPC_SendMessageToProcessW
#else
#define BoxedAppSDK_IPC_SendMessageToProcess BoxedAppSDK_IPC_SendMessageToProcessA
#endif // UNICODE

#ifdef __cplusplus
}
#endif

#include <poppack.h>

#endif // !__BOXEDAPPSDK_H__
