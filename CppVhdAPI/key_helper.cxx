#include <Windows.h>

#include "key_helper.hxx"

TCHAR pressanykey(const TCHAR* prompt)
{
	TCHAR  ch;
	DWORD  mode;
	DWORD  count;
	HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);

	// Prompt the user
	if (prompt == NULL) prompt = TEXT("Hit any key to quit the application\r\n");
	WriteConsole(
		GetStdHandle(STD_OUTPUT_HANDLE),
		prompt,
		lstrlen(prompt),
		&count,
		NULL
		);

	// Switch to raw mode
	GetConsoleMode(hstdin, &mode);
	SetConsoleMode(hstdin, 0);

	// Wait for the user's response
	WaitForSingleObject(hstdin, INFINITE);

	// Read the (single) key pressed
	ReadConsole(hstdin, &ch, 1, &count, NULL);

	// Restore the console to its previous state
	SetConsoleMode(hstdin, mode);

	// Return the key code
	return ch;
}
