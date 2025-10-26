#pragma once
#include "common.h"

#include <Windows.h>

struct consoleInformations {
private:
	void getConsoleHWND();
	void initData();

public:
	HWND cHandle;

	CONSOLE_CURSOR_INFO* cCursorInfo;
	CONSOLE_FONT_INFO* cFontInfo;
	CONSOLE_SCREEN_BUFFER_INFO* cScreenBufferInfo;
	CONSOLE_SCREEN_BUFFER_INFOEX* cScreenBufferInfoEx;




	consoleInformations();
	~consoleInformations();

	void refreshAllConsoleInformations();

};
typedef consoleInformations cInfo;