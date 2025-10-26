 #include "consoleInformations.h"

//CONSOLE_FONT_INFO* cFontInfo;
//CONSOLE_SCREEN_BUFFER_INFO* cScreenBufferInfo;
//CONSOLE_SCREEN_BUFFER_INFOEX* cScreenBufferInfoEx;

void consoleInformations::getConsoleHWND() {
	#define TITLE_BUFFER_SIZE 1024

	// Getting current window title
	wchar_t windowTitle[TITLE_BUFFER_SIZE];
	unsigned int windowTitleLength = GetConsoleTitleW(windowTitle, TITLE_BUFFER_SIZE);
	if (windowTitleLength == 0) throw std::runtime_error("getConsoleHWND error: GetConsoleTitleW function failed with code: " + std::to_string(GetLastError()));

	// Creating and setting a new, unique window title, based on proc ID and time
	wchar_t newWindowTitle[TITLE_BUFFER_SIZE];
	wsprintf(newWindowTitle, L"%d/%d", GetTickCount(), GetCurrentProcessId());
	if(!SetConsoleTitleW(newWindowTitle)) throw std::runtime_error("getConsoleHWND error: SetConsoleTitleW function for new name failed with code: " + std::to_string(GetLastError()));

	Sleep(50); // I hate every single line of code that I wrote here, but as it turns out it is the recommended way by Microsoft (why goddamit?) https://learn.microsoft.com/en-us/troubleshoot/windows-server/performance/obtain-console-window-handle

	// Finding window handle based on unique name
	this->cHandle = FindWindowW(NULL, newWindowTitle);
	if(this->cHandle == NULL) throw std::runtime_error("getConsoleHWND error: FindWindowW function failed with code: " + std::to_string(GetLastError()));

	// Restoring old name
	if (!SetConsoleTitleW(windowTitle)) throw std::runtime_error("getConsoleHWND error: SetConsoleTitleW function for old name failed with code: " + std::to_string(GetLastError()));
}
void consoleInformations::getData() {
	getConsoleHWND();
	 
	// TODO
}


consoleInformations::consoleInformations() {
	getData();
}

void consoleInformations::refreshAllConsoleInformations() {
	// TODO
}
void consoleInformations::setConsoleTitle(std::string newConsoleTitle) {
	// converting string to char array (yay windows...)
	unsigned int titleLength = (unsigned int)newConsoleTitle.length();
	char* cTitle = new char[titleLength + 1];

	for (unsigned int i = 0; i < titleLength; i++) {
		cTitle[i] = newConsoleTitle[i];
	}
	cTitle[titleLength] = '\0';

	if (!SetConsoleTitleA(cTitle)) throw std::runtime_error("setConsoleTitle error: SetConsoleTitleA function for new name failed with code: " + std::to_string(GetLastError()));
}
