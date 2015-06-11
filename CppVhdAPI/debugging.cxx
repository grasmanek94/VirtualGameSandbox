#include <Windows.h>
#include <iostream>

#include "debugging.hxx"

HRESULT ModifyPrivilege(
	IN LPCTSTR szPrivilege,
	IN BOOL fEnable)
{
	HRESULT hr = S_OK;
	TOKEN_PRIVILEGES NewState;
	LUID             luid;
	HANDLE hToken = NULL;

	// Open the process token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("Failed OpenProcessToken\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Get the local unique ID for the privilege.
	if (!LookupPrivilegeValue(NULL,
		szPrivilege,
		&luid))
	{
		CloseHandle(hToken);
		printf("Failed LookupPrivilegeValue\n");
		return ERROR_FUNCTION_FAILED;
	}

	// Assign values to the TOKEN_PRIVILEGE structure.
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luid;
	NewState.Privileges[0].Attributes =
		(fEnable ? SE_PRIVILEGE_ENABLED : 0);

	// Adjust the token privilege.
	if (!AdjustTokenPrivileges(hToken,
		FALSE,
		&NewState,
		0,
		NULL,
		NULL))
	{
		printf("Failed AdjustTokenPrivileges\n");
		hr = ERROR_FUNCTION_FAILED;
	}

	// Close the handle.
	CloseHandle(hToken);

	return hr;
}

BOOL EnableDebugPrivilege(void)
{
	return ModifyPrivilege(SE_DEBUG_NAME, TRUE) != ERROR_FUNCTION_FAILED;
}

bool FixPrivilidges(void)
{
	std::cout << "Fixing privlidges..." << std::flush;
	BOOL edp_a = ModifyPrivilege(SE_BACKUP_NAME, TRUE) != ERROR_FUNCTION_FAILED;
	BOOL edp_b = ModifyPrivilege(SE_RESTORE_NAME, TRUE) != ERROR_FUNCTION_FAILED;
	BOOL edp_c = ModifyPrivilege(SE_DEBUG_NAME, TRUE) != ERROR_FUNCTION_FAILED;

	if (edp_a && edp_b && edp_c)
	{
		std::cout << "OK" << std::endl;
		return true;
	}
	std::cout << "FAIL" << std::endl;
	return false;
}