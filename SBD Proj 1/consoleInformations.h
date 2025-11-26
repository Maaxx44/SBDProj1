#pragma once
#include "common.h"
#include "winErrorHandlers.h"

#include <Windows.h>

struct consoleInformations {
public:
	HANDLE cOutputHandle, cInputHandle;
	CONSOLE_SCREEN_BUFFER_INFOEX cScreenBufferInfoEx;
	CONSOLE_CURSOR_INFO cCursorInfo;
	DWORD cMode;

	consoleInformations();
	~consoleInformations();

	// Console manipulation functions
	void refreshAllConsoleInformations();
	void setConsoleTitle(std::string newConsoleTitle);
	void setConsoleSize(unsigned int X, unsigned int Y);
	void setConsoleMode(DWORD newMode);
	void setConsoleDefaultMode();
	void setCursorVisibility(bool isVisible);
	void setCursorPosition(unsigned int X, unsigned int Y) const;

	// Functions getting console size and buiffer size
	COR getCurrentDwSize() const;
	COR getCurrentSrWindow() const;

	std::vector<INPUT_RECORD> getUserInput() const;
	std::vector<INPUT_RECORD> getUserInput(std::vector<char> cBreak) const; // Stops reading input buffer at selected character/s keyboard input
	INPUT_RECORD getOneUserInput() const; // Returns next event
	INPUT_RECORD getOneKeyUserInput() const; // Returns next event with keydown
	bool isInputAvalible() const; // checks if there is input in input buffer
	bool isKeyInputAvalible() const; // checks if there is key input in input buffer
	std::optional<KEY_EVENT_RECORD> getFirstKeyInput() const; // if there is any key event in the queue  - get first and return - else - empty queue
	KEY_EVENT_RECORD getNextKeyInputBlocking() const; // Waits untill there is key input discarding other events

};
typedef consoleInformations cInfo;