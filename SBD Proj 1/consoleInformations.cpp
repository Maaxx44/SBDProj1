 #include "consoleInformations.h"

consoleInformations::consoleInformations() {
	// getting handle
	this->cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->cHandle == NULL) ErrorHandler("Failed to get output hande!");

	// getting console informations
	ZeroMemory(&this->cScreenBufferInfoEx, sizeof(cScreenBufferInfoEx));
	this->cScreenBufferInfoEx.cbSize = sizeof(cScreenBufferInfoEx);
	if (this->cHandle != NULL && !GetConsoleScreenBufferInfoEx(this->cHandle, &this->cScreenBufferInfoEx))
		ErrorHandler("Failed to get extended screen buffer info!");
}
consoleInformations::~consoleInformations() {
	// Setting font to oryginal
	if(this->cHandle != NULL && !SetConsoleTextAttribute(this->cHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY))
		ErrorHandler("Failed to set console font attributes!");
}

void consoleInformations::refreshAllConsoleInformations() {
	if (this->cHandle == NULL) throw std::runtime_error("setConsoleBuffer error: cHandle was NULL!");
	ZeroMemory(&this->cScreenBufferInfoEx, sizeof(cScreenBufferInfoEx));
	this->cScreenBufferInfoEx.cbSize = sizeof(cScreenBufferInfoEx);
	if (!GetConsoleScreenBufferInfoEx(this->cHandle, &this->cScreenBufferInfoEx))
		ErrorHandler("Failed to get extended screen buffer info!");
}
void consoleInformations::setConsoleTitle(std::string newConsoleTitle) {
	// converting string to char array (yay windows...)
	unsigned int titleLength = (unsigned int)newConsoleTitle.length();
	char* cTitle = new char[titleLength + 1];

	for (unsigned int i = 0; i < titleLength; i++) {
		cTitle[i] = newConsoleTitle[i];
	}
	cTitle[titleLength] = '\0';

	if (!SetConsoleTitleA(cTitle))
		ErrorHandler("Failed to set console window title!");
}
void consoleInformations::setConsoleSize(unsigned int X, unsigned int Y) {
	if (this->cHandle == NULL) throw std::runtime_error("setConsoleSize error: cHandle was NULL!");

	this->refreshAllConsoleInformations();
	this->cScreenBufferInfoEx.dwSize.X = X + 1;
	this->cScreenBufferInfoEx.dwSize.Y = Y + 1;
	this->cScreenBufferInfoEx.srWindow.Right = X;
	this->cScreenBufferInfoEx.srWindow.Bottom = Y;
	
	if (!SetConsoleScreenBufferInfoEx(this->cHandle, &this->cScreenBufferInfoEx))
		ErrorHandler("Failed to set extended screen buffer info!");

	if(!SetConsoleWindowInfo(this->cHandle, TRUE, &this->cScreenBufferInfoEx.srWindow))
		ErrorHandler("Failed to set console window info!");
}