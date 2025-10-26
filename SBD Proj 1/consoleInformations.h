#pragma once
#include "common.h"
#include "winErrorHandlers.h"

#include <Windows.h>

struct consoleInformations {
private:
	void getConsoleHWND();

	void getData();

public:
	HWND cHandle;

	CONSOLE_FONT_INFO cFontInfo;
	CONSOLE_SCREEN_BUFFER_INFO cScreenBufferInfo;
	CONSOLE_SCREEN_BUFFER_INFOEX cScreenBufferInfoEx;




	consoleInformations();

	void refreshAllConsoleInformations();
	void setConsoleTitle(std::string newConsoleTitle);

};
typedef consoleInformations cInfo;