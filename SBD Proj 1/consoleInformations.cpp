 #include "consoleInformations.h"

consoleInformations::consoleInformations() {
	// getting output handle
	this->cOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (this->cOutputHandle == NULL)
		ErrorHandler("Failed to get output hande!");

	// getting input handle
	this->cInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (this->cInputHandle == NULL)
		ErrorHandler("Failed to get input hande!");

	// getting default console mode
	ZeroMemory(&this->cMode, sizeof(cMode));
	if (this->cOutputHandle != NULL && !GetConsoleMode(this->cOutputHandle, &this->cMode))
		ErrorHandler("Failed to get console mode!");

	// getting cursor informations
	ZeroMemory(&this->cCursorInfo, sizeof(cCursorInfo));
	if(this->cOutputHandle != NULL && !GetConsoleCursorInfo(this->cOutputHandle, &this->cCursorInfo))
		ErrorHandler("Failed to get console cursor info!");

	// getting console informations
	ZeroMemory(&this->cScreenBufferInfoEx, sizeof(cScreenBufferInfoEx));
	this->cScreenBufferInfoEx.cbSize = sizeof(cScreenBufferInfoEx);
	if (this->cOutputHandle != NULL && !GetConsoleScreenBufferInfoEx(this->cOutputHandle, &this->cScreenBufferInfoEx))
		ErrorHandler("Failed to get extended screen buffer info!");
}
consoleInformations::~consoleInformations() {
	// Setting font to oryginal
	if(this->cOutputHandle != NULL && !SetConsoleTextAttribute(this->cOutputHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY))
		ErrorHandler("Failed to set console font attributes!");

	// Setting console mode to oryginal
	if (this->cOutputHandle != NULL && !SetConsoleMode(this->cOutputHandle, this->cMode))
		ErrorHandler("Failed to set oryginal console mode!");

	// Setting cursor to visible
	this->cCursorInfo.bVisible = true;
	if (this->cOutputHandle != NULL && !SetConsoleCursorInfo(this->cOutputHandle, &this->cCursorInfo))
		ErrorHandler("Failed to set cursor to visible!");
}

void consoleInformations::refreshAllConsoleInformations() {
	if (this->cOutputHandle == NULL) throw std::runtime_error("setConsoleBuffer error: cOutputHandle was NULL!");
	ZeroMemory(&this->cScreenBufferInfoEx, sizeof(cScreenBufferInfoEx));
	this->cScreenBufferInfoEx.cbSize = sizeof(cScreenBufferInfoEx);
	if (!GetConsoleScreenBufferInfoEx(this->cOutputHandle, &this->cScreenBufferInfoEx))
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
	if (this->cOutputHandle == NULL) throw std::runtime_error("setConsoleSize error: cOutputHandle was NULL!");

	this->refreshAllConsoleInformations();
	this->cScreenBufferInfoEx.dwSize.X = X + defaultConsoleSizeToBufferOffset;
	this->cScreenBufferInfoEx.dwSize.Y = Y + defaultConsoleSizeToBufferOffset;
	this->cScreenBufferInfoEx.srWindow.Right = X;
	this->cScreenBufferInfoEx.srWindow.Bottom = Y;
	
	if (!SetConsoleScreenBufferInfoEx(this->cOutputHandle, &this->cScreenBufferInfoEx))
		ErrorHandler("Failed to set extended screen buffer info!");

	if(!SetConsoleWindowInfo(this->cOutputHandle, TRUE, &this->cScreenBufferInfoEx.srWindow))
		ErrorHandler("Failed to set console window info!");
}
void consoleInformations::setConsoleMode(DWORD newMode) {
	if (this->cOutputHandle == NULL) throw std::runtime_error("setConsoleMode error: cOutputHandle was NULL!");

	if(!SetConsoleMode(this->cOutputHandle, newMode))
		ErrorHandler("Failed to set new console mode!");
}
void consoleInformations::setConsoleDefaultMode() {
	this->setConsoleMode(this->cMode);
}
void consoleInformations::setCursorVisibility(bool isVisible) {
	if (this->cOutputHandle == NULL) throw std::runtime_error("setCursorVisibility error: cOutputHandle was NULL!");

	this->cCursorInfo.bVisible = isVisible;
	if(!SetConsoleCursorInfo(this->cOutputHandle, &this->cCursorInfo))
		ErrorHandler("Failed to set cursor visibility!");
}
void consoleInformations::setCursorPosition(unsigned int X, unsigned int Y) const {
	if (this->cOutputHandle == NULL) throw std::runtime_error("setCursor error: cOutputHandle was NULL!");
	if(!SetConsoleCursorPosition(this->cOutputHandle, COORD(X, Y)))
		ErrorHandler("Failed to set cursor position!");
}

COR consoleInformations::getCurrentDwSize() const {
	return { .X = (unsigned int)this->cScreenBufferInfoEx.dwSize.X, .Y = (unsigned int)this->cScreenBufferInfoEx.dwSize.Y };
}
COR consoleInformations::getCurrentSrWindow() const {
	return { .X = (unsigned int)this->cScreenBufferInfoEx.srWindow.Right, .Y = (unsigned int)this->cScreenBufferInfoEx.srWindow.Bottom };
}

std::vector<INPUT_RECORD> consoleInformations::getUserInput() const {
	if(this->cInputHandle == NULL) throw std::runtime_error("getUserInput error: cInputHandle was NULL!");

	// Checking if there is any event to be read
	DWORD eventCount = 0x0000;
	if(!GetNumberOfConsoleInputEvents(this->cInputHandle, &eventCount))
		ErrorHandler("Failed to get number of console input events!");

	// If there are no events - there was no input, so return empty vector
	if (eventCount == 0) return std::vector<INPUT_RECORD>();

	// If there WERE events - get array of that events, convert to vector and return
	/// create dynamic array with the size of eventCount
	INPUT_RECORD* inputBufferArray = new INPUT_RECORD[eventCount];

	/// get all events
	DWORD eventRead = 0x0000;
	if(!ReadConsoleInput(this->cInputHandle, inputBufferArray, eventCount, &eventRead))
		ErrorHandler("Failed to read console input!");

	/// convert array of events to vector and filter out events
	//std::vector<INPUT_RECORD> inputBufferVector(inputBufferArray, inputBufferArray + eventRead);
	std::vector<INPUT_RECORD> inputBufferVector;
	for (unsigned int recNumber = 0; recNumber < eventRead; recNumber++) {
		if (inputBufferArray[recNumber].EventType != ignoredInputEvents)
			inputBufferVector.push_back(inputBufferArray[recNumber]);
	}

	/// delete this dynamic mess and return our vector
	delete[] inputBufferArray;

	return inputBufferVector;
}
std::vector<INPUT_RECORD> consoleInformations::getUserInput(std::vector<char> cBreak) const {
	if (this->cInputHandle == NULL) throw std::runtime_error("getUserInput(vector<char>) error: cInputHandle was NULL!");

	// Reading events one-by-one 
	std::vector<INPUT_RECORD> inputBufferVector;
	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;
	bool cBreakReached = false;

	while (!cBreakReached) {
		// Reading next event
		if(!ReadConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
			ErrorHandler("Failed to read console input!");

		// Checking event type and adding it to out vector
		if (eventCount != 0 && inputBuffer.EventType != ignoredInputEvents)
			inputBufferVector.push_back(inputBuffer);

		// Checking if key input is one of out exit characters
		if (eventCount != 0 && inputBuffer.EventType == KEY_EVENT) {
			if (inputBuffer.Event.KeyEvent.bKeyDown) {
				for (char breakChar : cBreak) {
					if (breakChar == inputBuffer.Event.KeyEvent.uChar.AsciiChar) {
						cBreakReached = true;
						break;
					}
				}
			}
		}
	}

	return inputBufferVector;
}
INPUT_RECORD consoleInformations::getOneUserInput() const {
	if (this->cInputHandle == NULL) throw std::runtime_error("getOneUserInput error: cInputHandle was NULL!");

	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;

	// Getting input event
	if (!ReadConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
		ErrorHandler("Failed to read console input!");

	// Checking if this is not ignored event
	while (eventCount != 0 && inputBuffer.EventType == ignoredInputEvents) {
		// Getting next input event
		if (!ReadConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
			ErrorHandler("Failed to read console input!");
	}

	return inputBuffer;
}
INPUT_RECORD consoleInformations::getOneKeyUserInput() const {
	if (this->cInputHandle == NULL) throw std::runtime_error("getOneKeyUserInput error: cInputHandle was NULL!");

	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;
	bool keyEventReached = false;

	while (!keyEventReached) {
		// Getting input event
		if (!ReadConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
			ErrorHandler("Failed to read console input!");

		// Checking if we reched key down event
		if (eventCount != 0 && inputBuffer.EventType == KEY_EVENT && inputBuffer.Event.KeyEvent.bKeyDown) {
			keyEventReached = true;
			break;
		}

	}

	return inputBuffer;
}
bool consoleInformations::isInputAvalible() const {
	if (this->cInputHandle == NULL) throw std::runtime_error("isInputAvalible error: cInputHandle was NULL!");
	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;

	if (!PeekConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
		ErrorHandler("Failed to read console input!");

	return eventCount != 0x000;
}
bool consoleInformations::isKeyInputAvalible() const {
	if (this->cInputHandle == NULL) throw std::runtime_error("isKeyInputAvalible error: cInputHandle was NULL!");
	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;

	if (!PeekConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
		ErrorHandler("Failed to peek console input!");

	return eventCount != 0x000 && inputBuffer.EventType == KEY_EVENT && inputBuffer.Event.KeyEvent.bKeyDown;
}
std::optional<KEY_EVENT_RECORD> consoleInformations::getFirstKeyInput() const {
	if (this->cInputHandle == NULL) throw std::runtime_error("getFirstKeyInput error: cInputHandle was NULL!");

	INPUT_RECORD inputBuffer;
	DWORD eventCount = 0x0000;
	bool keyEventReached = false;

	while (!keyEventReached) {
		// Checking if there is anything in the queue
		if (!PeekConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
			ErrorHandler("Failed to peek console input!");
		if (eventCount == 0) return std::nullopt;

		// Getting input event
		if (!ReadConsoleInput(this->cInputHandle, &inputBuffer, 1, &eventCount))
			ErrorHandler("Failed to read console input!");

		// Checking if we reched key down event or the end of queue
		else if (inputBuffer.EventType == KEY_EVENT && inputBuffer.Event.KeyEvent.bKeyDown) {
			keyEventReached = true;
			break;
		}

	}

	return std::optional<KEY_EVENT_RECORD>(inputBuffer.Event.KeyEvent);
}
