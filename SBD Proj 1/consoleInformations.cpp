#include "consoleInformations.h"

//HWND cHandle;
//CONSOLE_CURSOR_INFO* cCursorInfo;
//CONSOLE_FONT_INFO* cFontInfo;
//CONSOLE_SCREEN_BUFFER_INFO* cScreenBufferInfo;
//CONSOLE_SCREEN_BUFFER_INFOEX* cScreenBufferInfoEx;

void consoleInformations::getConsoleHWND() {
	#define TITLE_BUFFER_SIZE 1024

	// Getting current window title
	wchar_t windowTitle[TITLE_BUFFER_SIZE];
	unsigned int windowTitleLength = GetConsoleTitleW(windowTitle, TITLE_BUFFER_SIZE);
	if (windowTitleLength == 0) throw std::runtime_error("GetConsoleTitleW function failed with code: " + std::to_string(GetLastError()));

	// Creating and setting a new, unique window title, based on proc ID and time
	wchar_t newWindowTitle[TITLE_BUFFER_SIZE];
	wsprintf(newWindowTitle, L"%d/%d", GetTickCount(), GetCurrentProcessId());
	if(!SetConsoleTitleW(newWindowTitle)) throw std::runtime_error("SetConsoleTitleW function for new name failed with code: " + std::to_string(GetLastError()));

	Sleep(50); // I hate every single line of code that I wrote here, but as it turns out it is the recommended way by Microsoft (why goddamit?) https://learn.microsoft.com/en-us/troubleshoot/windows-server/performance/obtain-console-window-handle

	// Finding window handle based on unique name
	this->cHandle = FindWindowW(NULL, newWindowTitle);
	if(this->cHandle == NULL) throw std::runtime_error("FindWindowW function failed with code: " + std::to_string(GetLastError()));

	// Restoring old name
	if (!SetConsoleTitleW(windowTitle)) throw std::runtime_error("SetConsoleTitleW function for old name failed with code: " + std::to_string(GetLastError()));
}
void consoleInformations::initData() {
	getConsoleHWND();





}


consoleInformations::consoleInformations() {
	initData();
}
consoleInformations::~consoleInformations() {
	if (this->cCursorInfo != nullptr) delete this->cCursorInfo;
	if (this->cFontInfo != nullptr) delete this->cFontInfo;
	if (this->cScreenBufferInfo != nullptr) delete this->cScreenBufferInfo;
	if (this->cScreenBufferInfoEx != nullptr) delete this->cScreenBufferInfoEx;
}

void consoleInformations::refreshAllConsoleInformations() {

}
