 #include "consoleInformations.h"

//CONSOLE_SCREEN_BUFFER_INFO* cScreenBufferInfo;
//CONSOLE_SCREEN_BUFFER_INFOEX* cScreenBufferInfoEx;

void consoleInformations::getData() {
	this->cHandle = GetConsoleWindow();
	if (this->cHandle == NULL) ErrorHandler("Failed to get console window handle!");


	 
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

	if (!SetConsoleTitleA(cTitle)) ErrorHandler("Failed to set console window title!");
}
