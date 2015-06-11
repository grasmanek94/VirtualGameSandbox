#pragma once
#include <Windows.h>

HRESULT ModifyPrivilege(
	IN LPCTSTR szPrivilege,
	IN BOOL fEnable);

bool FixPrivilidges(void);