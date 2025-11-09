#pragma once
#include "common.h"
#include "winErrorHandlers.h"

#include <Windows.h>

struct consoleInformations {
public:
	HANDLE cHandle;
	CONSOLE_SCREEN_BUFFER_INFOEX cScreenBufferInfoEx;
	CONSOLE_CURSOR_INFO cCursorInfo;
	DWORD cMode;

	consoleInformations();
	~consoleInformations();

	void refreshAllConsoleInformations();
	void setConsoleTitle(std::string newConsoleTitle);
	void setConsoleSize(unsigned int X, unsigned int Y);
	void setConsoleMode(DWORD newMode);
	void setConsoleDefaultMode();
	void setCursorVisibility(bool isVisible);
	void setCursorPosition(unsigned int X, unsigned int Y) const;
};
typedef consoleInformations cInfo;