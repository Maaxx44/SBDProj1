 #include "consoleInformations.h"

consoleInformations::consoleInformations() {
	// getting handle
	this->cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->cHandle == NULL)
		ErrorHandler("Failed to get output hande!");

	// getting default console mode
	ZeroMemory(&this->cMode, sizeof(cMode));
	if (this->cHandle != NULL && !GetConsoleMode(this->cHandle, &this->cMode))
		ErrorHandler("Failed to get console mode!");

	// getting cursor informations
	ZeroMemory(&this->cCursorInfo, sizeof(cCursorInfo));
	if(this->cHandle != NULL && !GetConsoleCursorInfo(this->cHandle, &this->cCursorInfo))
		ErrorHandler("Failed to get console cursor info!");

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

	// Setting console mode to oryginal
	if (this->cHandle != NULL && !SetConsoleMode(this->cHandle, this->cMode))
		ErrorHandler("Failed to set oryginal console mode!");

	// Setting cursor to visible
	this->cCursorInfo.bVisible = true;
	if (this->cHandle != NULL && !SetConsoleCursorInfo(this->cHandle, &this->cCursorInfo))
		ErrorHandler("Failed to set cursor to visible!");
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
	char* cTitle = new char[(unsigned int)newConsoleTitle.length() + 1];
	strncpy_s(cTitle, (unsigned int)newConsoleTitle.length() + 1, newConsoleTitle.c_str(), newConsoleTitle.length());

	if (!SetConsoleTitleA(cTitle))
		ErrorHandler("Failed to set console window title!");

	delete[] cTitle;
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
void consoleInformations::setConsoleMode(DWORD newMode) {
	if (this->cHandle == NULL) throw std::runtime_error("setConsoleMode error: cHandle was NULL!");

	if(!SetConsoleMode(this->cHandle, newMode))
		ErrorHandler("Failed to set new console mode!");
}
void consoleInformations::setConsoleDefaultMode() {
	this->setConsoleMode(this->cMode);
}
void consoleInformations::setCursorVisibility(bool isVisible) {
	if (this->cHandle == NULL) throw std::runtime_error("setCursorVisibility error: cHandle was NULL!");

	this->cCursorInfo.bVisible = isVisible;
	if(!SetConsoleCursorInfo(this->cHandle, &this->cCursorInfo))
		ErrorHandler("Failed to set cursor visibility!");
}
void consoleInformations::setCursorPosition(unsigned int X, unsigned int Y) const {
	if (this->cHandle == NULL) throw std::runtime_error("setCursor error: cHandle was NULL!");
	if(!SetConsoleCursorPosition(this->cHandle, COORD(X, Y)))
		ErrorHandler("Failed to set cursor position!");
}

std::vector<INPUT_RECORD> consoleInformations::getUserInput() const {
	// Getting INPUT handle, because we need this rather than out cHandle that is OUTPUT handle (I hate windows I hate windows I hate windows)
	HANDLE cInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if(cInputHandle == INVALID_HANDLE_VALUE)
		ErrorHandler("Failed to get input handle!");

	DWORD eventCount = 0x0000;

	// Checking if there is any event to be read
	if(!GetNumberOfConsoleInputEvents(cInputHandle, &eventCount))
		ErrorHandler("Failed to get number of console input events!");

	// If there are no events - there was no input, so return empty vector
	if (eventCount == 0) return std::vector<INPUT_RECORD>();

	// If there WERE events - get array of that events, convert to vector and return
	/// create dynamic array with the size of eventCount
	INPUT_RECORD* inputBufferArray = new INPUT_RECORD[eventCount];

	/// get all events
	DWORD eventRead = 0x0000;
	if(!ReadConsoleInput(cInputHandle, inputBufferArray, eventCount, &eventRead))
		ErrorHandler("Failed to read console input!");

	/// convert array of events to vector
	std::vector<INPUT_RECORD> inputBufferVector(inputBufferArray, inputBufferArray + eventRead); // TODO - check if correct

	/// delete this dynamic mess and return our vector
	delete inputBufferArray;

	return inputBufferVector;
}
