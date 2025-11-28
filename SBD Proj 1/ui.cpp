#include "ui-core-merged.h"

// ---- PRIVATE ----
void ui::initConsole() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	this->cInfo.setConsoleTitle(defaultConsoleTitle);
	this->cInfo.setConsoleSize(defaultConsoleSizeX, defaultConsoleSizeY);
	this->cInfo.setConsoleMode(consoleNoTextMode);
	this->cInfo.setCursorVisibility(false);
}
void ui::initCursor() {
	this->cursorInfo.cType = disabled;
	this->cursorInfo.currentContentCursorPoint = 0;
	this->cursorInfo.currentTableCursorPoint = nullptr;
	this->cursorInfo.selectedTables.clear();
}


void ui::resetColor() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) const{
	if (this->cInfo.cOutputHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cOutputHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
}

void ui::drawTableTitle(uiTable* table) const {
	if (table == nullptr)
		throw std::runtime_error("drawTable error: table was nullptr!");

	REC titleBox = table->getTitleRectangle();
	WORD titleModifiers = table->getTitleMod().getTextModifier();
	std::string titleString = std::string(titleBox.W, ' ');
	if (table->getTitleMod().isTextVisible) titleString = table->getTitle(); // If title is visible - get title string


	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", titleString.front());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", titleString.substr(1, titleBox.W - 2).c_str());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", titleString.back());
}
void ui::drawTableContentLine(uiTable* table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const {
	if (table == nullptr)
		throw std::runtime_error("drawTable error: table was nullptr!");

	REC contentBox = table->getContentRectangle();
	WORD contentColor = contentMod.getTextModifier();
	if (!contentMod.isTextVisible) contentString = std::string(contentBox.W, ' ');

	//  Getting modifiers for drawing box around content
	WORD modifiers = NULL;
	if (laneIndex == 0)					modifiers |= COMMON_LVB_GRID_HORIZONTAL;
	if (laneIndex == contentBox.H - 1)	modifiers |= COMMON_LVB_UNDERSCORE;

	// Writing Line
	this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + laneIndex);
	this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", contentString.front());
	this->changeColor(contentColor | modifiers);
	printf("%s", contentString.substr(1, contentBox.W - 2).c_str());
	this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", contentString.back());
}
void ui::drawTableContent(uiTable* table) const {
	if (table == nullptr)
		throw std::runtime_error("drawTable error: table was nullptr!");

	REC contentBox = table->getContentRectangle();
	std::vector<std::string> drawContent = table->getContent();
	std::vector<textModifiers> drawMod = table->getContentMod();

	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		drawTableContentLine(table, drawContent[i], drawMod[i], i);
	}
}
void ui::drawTable(uiTable* table) const {
	if (table == nullptr)
		throw std::runtime_error("drawTable error: table was nullptr!");

	table->updateTiming();

	// If table is invisible - don't draw it lul
	if (!table->getTableVisible()) return;

	// Draw table title
	drawTableTitle(table);

	// Draw table content
	drawTableContent(table);
}

void ui::clearAfterTable(uiTable* table) const {
	if (table == nullptr)
		throw std::runtime_error("drawTable error: table was nullptr!");

	// Gathering data
	REC tablePosition = table->getTableRectangle();
	const std::string emptyLine = std::string(tablePosition.W, ' ');

	// Clearing after table
	this->changeColor(defaultTextColor);
	for (unsigned int line = 0; line < tablePosition.H; line++) {
		this->cInfo.setCursorPosition(tablePosition.X, tablePosition.Y + line);
		printf(emptyLine.c_str());
	}
}

void ui::changeTableSelection(std::initializer_list<std::pair<uiTable*, bool>> tData) {
	for (std::pair<uiTable*, bool> table : tData) {
		if (table.first != nullptr)
			if (table.second)
				table.first->getTitleMod().startBlink();
			else
				table.first->getTitleMod().stopBlink();
	}
}
void ui::changeSelectedTableContentSelection(std::initializer_list<std::pair<unsigned int, bool>> cData) {
	if (this->cursorInfo.currentTableCursorPoint == nullptr)
		throw std::runtime_error("changeSelectedTableContentSelection error: currentTableCursorPoint was nullptr!");
	for (std::pair<unsigned int, bool> content : cData) {
		if (content.second)
			this->cursorInfo.currentTableCursorPoint->getContentLineMod(content.first).startBlink();
		else
			this->cursorInfo.currentTableCursorPoint->getContentLineMod(content.first).stopBlink();
	}
}


// helper functions for getting user input
uiTable ui::createInputTable(COR position, unsigned int width, std::string title) const {
	uiTable inputTable({ .X = position.X, .Y = position.Y, .W = width, .H = 2 }, title, { "" });
	inputTable.setTableVisible(false);

	return inputTable;
}

functionExitCode ui::parseTableInput(WORD keyCode) {
	switch (keyCode) {
	case VK_UP:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().up != nullptr) {
			this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint->getTablePointers().up , true}, {this->cursorInfo.currentTableCursorPoint, false} });
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().up; // change cursor to new selected table
		}
		break;
	case VK_DOWN:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().down != nullptr) {
			this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint->getTablePointers().down , true}, {this->cursorInfo.currentTableCursorPoint, false} });
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().down; // change cursor to new selected table
		}
		break;
	case VK_LEFT:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().left != nullptr) {
			this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint->getTablePointers().left , true}, {this->cursorInfo.currentTableCursorPoint, false} });
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().left; // change cursor to new selected table
		}
		break;
	case VK_RIGHT:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().right != nullptr) {
			this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint->getTablePointers().right , true}, {this->cursorInfo.currentTableCursorPoint, false} });
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().right; // change cursor to new selected table
		}
		break;
	case VK_ESCAPE:
		if (cursorInfo.selectedTables.size() == 0) //end program
			return exitProgram;
		else {
			if (cursorInfo.selectedTables.back() != this->cursorInfo.currentTableCursorPoint) {
				this->changeTableSelection({ {cursorInfo.selectedTables.back() , true}, {this->cursorInfo.currentTableCursorPoint, false} });
				this->cursorInfo.currentTableCursorPoint = cursorInfo.selectedTables.back(); // change cursor to new selected table
				cursorInfo.selectedTables.pop_back(); // just remove last element
			}
		}
		break;
	case VK_RETURN:
		// If there is no table content - dont go
		if (this->cursorInfo.currentTableCursorPoint->getContentSize() == 0) break;

		// First - *select *current table title and disable blinking
		this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint , false} });
		this->cursorInfo.currentTableCursorPoint->getTitleMod().isTextSelected = true;

		// Change cursor settings
		this->cursorInfo.cType = contentPointer;
		this->cursorInfo.currentContentCursorPoint = 0;

		// Set content blinking
		changeSelectedTableContentSelection({ {this->cursorInfo.currentContentCursorPoint, true} });

		// Add this table to selected tables
		cursorInfo.selectedTables.push_back(this->cursorInfo.currentTableCursorPoint);

		break;
	}
	return continueProgram;
}
functionExitCode ui::parseContentInput(WORD keyCode) {
	// Check for arrows, enter and escape and change / select content
	switch (keyCode) {
	case VK_UP:
		if (this->cursorInfo.currentContentCursorPoint == 0) {
			unsigned int newContentCursorPointer = this->cursorInfo.currentTableCursorPoint->getContentSize() - 1;
			changeSelectedTableContentSelection({ {newContentCursorPointer, true}, {this->cursorInfo.currentContentCursorPoint, false } });
			this->cursorInfo.currentContentCursorPoint = newContentCursorPointer;
		}
		else {
			changeSelectedTableContentSelection({ {this->cursorInfo.currentContentCursorPoint - 1, true}, {this->cursorInfo.currentContentCursorPoint, false } });
			this->cursorInfo.currentContentCursorPoint--;
		}
		// Check if out element is in visible range
		if (!this->cursorInfo.currentTableCursorPoint->isContentLineVisible(this->cursorInfo.currentContentCursorPoint))
			this->cursorInfo.currentTableCursorPoint->setContentOffset(this->cursorInfo.currentTableCursorPoint->getOffsetForLine(this->cursorInfo.currentContentCursorPoint));
		break;
	case VK_DOWN:
		if (this->cursorInfo.currentContentCursorPoint == this->cursorInfo.currentTableCursorPoint->getContentSize() - 1) {
			changeSelectedTableContentSelection({ {0, true}, {this->cursorInfo.currentContentCursorPoint, false } });
			this->cursorInfo.currentContentCursorPoint = 0;
		}
		else {
			changeSelectedTableContentSelection({ {this->cursorInfo.currentContentCursorPoint + 1, true}, {this->cursorInfo.currentContentCursorPoint, false } });
			this->cursorInfo.currentContentCursorPoint++;
		}
		// Check if out element is in visible range
		if (!this->cursorInfo.currentTableCursorPoint->isContentLineVisible(this->cursorInfo.currentContentCursorPoint))
			this->cursorInfo.currentTableCursorPoint->setContentOffset(this->cursorInfo.currentTableCursorPoint->getOffsetForLine(this->cursorInfo.currentContentCursorPoint));
		break;
	case VK_ESCAPE:
		// Change cursor type
		this->cursorInfo.cType = tablePointer;

		// Deselect content
		changeSelectedTableContentSelection({ {this->cursorInfo.currentContentCursorPoint, false} });

		// Select last table, disable title highlight and enable blinking
		this->cursorInfo.currentTableCursorPoint = cursorInfo.selectedTables.back();
		cursorInfo.selectedTables.back()->getTitleMod().isTextSelected = false;
		this->changeTableSelection({ {cursorInfo.selectedTables.back() , true} });
		cursorInfo.selectedTables.pop_back();

		break;
	case VK_RETURN:
		if (this->cursorInfo.currentTableCursorPoint->isFunctionCallingEnabled())
			if (this->appCore.callTableFunction(this->cursorInfo.currentTableCursorPoint, this->cursorInfo.currentContentCursorPoint) == exitProgram)
				return exitProgram;
	}
	return continueProgram;
}

functionExitCode ui::parseUserInput() {
	if (cursorInfo.cType == disabled) return continueProgram;
	if (this->cursorInfo.currentTableCursorPoint == nullptr)
		throw std::runtime_error("parseUserInput error: currentTableCursorPoint was nullptr!");


	// While there is input to be read - read it 
	// side note - this is sub-optimal solution. If there is input spam we can have ui lags. But I don't care right now
	std::optional<KEY_EVENT_RECORD> keyRecord = this->cInfo.getFirstKeyInput();
	while (keyRecord != std::nullopt) {
		// Getting pressed key code (virtual code is device independent)
		KEY_EVENT_RECORD inputKey = keyRecord.value();
		WORD keyCode = inputKey.wVirtualKeyCode;

		// Parse input according to cursor Informations
		switch (cursorInfo.cType) {
		case tablePointer:
			if (parseTableInput(keyCode) == exitProgram) return exitProgram;
			break;
		case contentPointer:
			if (parseContentInput(keyCode) == exitProgram) return exitProgram;
			break;
		default:
			throw std::runtime_error("UI cursor state was incorrect.");
		}
		
		// getting next record
		keyRecord = this->cInfo.getFirstKeyInput();
	}

	return continueProgram;
}

void ui::draw() {
	//this->appCore.updateTables();

	// Draw tables
	for (uiTable* pTable : this->pProgramTables)
		this->drawTable(pTable); // TODO - CHECK IF CORRECT

	// Return cursor to normal modifiers
	this->changeColor(defaultTextColor);
}
// -----------------

// ---- PUBLIC ----
/// User input
std::optional<double> ui::getUserInputDouble(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableDoubleDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(&inputTable);

	// Parsing user input
	bool usedComma = false;
	std::string unparsedUserInput = "";
	std::optional<KEY_EVENT_RECORD> keyRecord;
	while (keyRecord = this->cInfo.getNextKeyInputBlocking()) {
		WORD keyCode = keyRecord.value().wVirtualKeyCode;

		// Parse key code
		if (keyCode == VK_BACK) { // Remove last inputted character
			if (!unparsedUserInput.empty()) {
				char removedChar = unparsedUserInput.back(); // Getting last character
				unparsedUserInput.pop_back(); // Removing last character
				if (removedChar == '.') usedComma = false; // If last character was comma - enable comma input again
			}
		}
		else if (keyCode == VK_ESCAPE) { // Exit user input without parsing
			this->clearAfterTable(&inputTable);
			return std::nullopt;
		}
		else if (keyCode >= 0x30 && keyCode <= 0x39 && (GetKeyState(VK_LSHIFT) & 0x1000) == 0) { // Add number if there is space 
			if (unparsedUserInput.size() < tableWidth)
				unparsedUserInput.push_back('0' + keyCode - 0x30);
		}
		else if (!usedComma && (keyCode == VK_OEM_PERIOD || keyCode == VK_OEM_COMMA) && (GetKeyState(VK_LSHIFT) & 0x1000) == 0) { // Verify if no comma has been placed and add comma 
			if (unparsedUserInput.size() < tableWidth) {
				unparsedUserInput.push_back('.');
				usedComma = true;
			}
		}
		else if (keyCode == VK_RETURN) { // Parse user input and return

			// If input is empty - return nullopt
			if (unparsedUserInput.empty())
				return std::nullopt;

			// Clear up user input
			else if (unparsedUserInput == ".") return 0.0;
			else if (unparsedUserInput.front() == '.' && unparsedUserInput.size() > 1) // If there is dot at the beg. add zero before parsing
				unparsedUserInput.insert(0, 1, '0');
			else if (unparsedUserInput.back() == '.' && unparsedUserInput.size() > 1) // If there is dot at the end. add zero before parsing
				unparsedUserInput.push_back('0');

			// Try parsing
			double returnValue = 0.0;
			try {
				returnValue = stod(unparsedUserInput);
			}
			catch (std::exception e) {
				// If parsing failed - return nullopt
				this->clearAfterTable(&inputTable);
				return std::nullopt;
			}

			// Return parsed value
			this->clearAfterTable(&inputTable);
			return std::optional<double>(returnValue);
		}

		// Update user input inside table
		inputTable.setContent({ unparsedUserInput });
		drawTable(&inputTable);
	}


	this->clearAfterTable(&inputTable);
	return std::nullopt;
}
std::optional<unsigned int> ui::getUserInputUInt(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableUIntDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(&inputTable);

	// Parsing user input
	std::string unparsedUserInput = "";
	std::optional<KEY_EVENT_RECORD> keyRecord;
	while (keyRecord = this->cInfo.getNextKeyInputBlocking()) {
		WORD keyCode = keyRecord.value().wVirtualKeyCode;

		// Parse key code
		if (keyCode == VK_BACK) { // Remove last inputted character
			if (!unparsedUserInput.empty())
				unparsedUserInput.pop_back(); // Removing last character
		}
		else if (keyCode == VK_ESCAPE) { // Exit user input without parsing
			this->clearAfterTable(&inputTable);
			return std::nullopt;
		}
		else if (keyCode >= 0x30 && keyCode <= 0x39 && (GetKeyState(VK_LSHIFT) & 0x1000) == 0) { // Add number if there is space 
			if (unparsedUserInput.size() < tableWidth)
				unparsedUserInput.push_back('0' + keyCode - 0x30);
		}
		else if (keyCode == VK_RETURN) { // Parse user input and return
			// If input is empty - return nullopt
			if (unparsedUserInput.empty())
				return std::nullopt;

			// Try parsing
			unsigned int returnValue = 0;
			try {
				// atol() converts to long that is then casted to unsigned int. In this way we capture full range of unsigned int and not write custom string-to-unsigned-int interpreter
				returnValue = (unsigned int)atol(unparsedUserInput.c_str());
			}
			catch (std::exception e) {
				// If parsing failed - return nullopt
				this->clearAfterTable(&inputTable);
				return std::nullopt;
			}

			// Return parsed value
			this->clearAfterTable(&inputTable);
			return std::optional<unsigned int>(returnValue);
		}

		// Update user input inside table
		inputTable.setContent({ unparsedUserInput });
		drawTable(&inputTable);
	}


	this->clearAfterTable(&inputTable);
	return std::nullopt;
}
std::optional<std::string> ui::getUserInputString(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableStringDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(&inputTable);

	// Parsing user input
	std::string unparsedUserInput = "";
	std::optional<KEY_EVENT_RECORD> keyRecord;
	while (keyRecord = this->cInfo.getNextKeyInputBlocking()) {
		WORD keyCode = keyRecord.value().wVirtualKeyCode;

		// Parse key code
		if (keyCode == VK_BACK) { // Remove last inputted character
			if (!unparsedUserInput.empty())
				unparsedUserInput.pop_back(); // Removing last character
		}
		else if (keyCode == VK_ESCAPE) { // Exit user input without parsing
			this->clearAfterTable(&inputTable);
			return std::nullopt;
		}
		else if (keyCode >= 0x30 && keyCode <= 0x39) { // Add number if there is space
			if (unparsedUserInput.size() < tableWidth)
				unparsedUserInput.push_back('0' + keyCode - 0x30);
		}
		else if (keyCode >= 0x41 && keyCode <= 0x5A) { // Add character based on LShift key state
			if (GetKeyState(VK_LSHIFT) & 0x1000) {
				// If uneven number of CM we add uppercase letter
				if (unparsedUserInput.size() < tableWidth)
					unparsedUserInput.push_back('A' + keyCode - 0x41);
			}
			else {
				// Else we add lowercase letter
				if (unparsedUserInput.size() < tableWidth)
					unparsedUserInput.push_back('a' + keyCode - 0x41);
			}
		}
		else if (keyCode == VK_RETURN) { // Parse user input and return
			// If input is empty - return nullopt
			if (unparsedUserInput.empty())
				return std::nullopt;

			// Return value
			this->clearAfterTable(&inputTable);
			return std::optional<std::string>(unparsedUserInput);
		}

		// Update user input inside table
		inputTable.setContent({ unparsedUserInput });
		drawTable(&inputTable);
	}


	this->clearAfterTable(&inputTable);
	return std::nullopt;
}
std::optional<std::filesystem::path> ui::getUserInputFile(std::string customMessage) const {
	return std::nullopt;
}

/// Cursor
void ui::setCursor(uiTable* selectedTable) {
	if (selectedTable == nullptr)
		throw std::runtime_error("setCursor error: selectedTable was nullptr!");

	// If out cursor was pointing to something - deselect it, just in case
	if (this->cursorInfo.currentTableCursorPoint != nullptr)
		this->changeTableSelection({ {this->cursorInfo.currentTableCursorPoint , false} });

	this->cursorInfo.cType = tablePointer;
	this->cursorInfo.currentContentCursorPoint = 0;
	this->cursorInfo.currentTableCursorPoint = selectedTable;
	this->changeTableSelection({ {selectedTable , true} });
}
/// Tables manipulation
void ui::addTable(uiTable* newPTable) {
	this->pProgramTables.push_back(newPTable);
}

functionExitCode ui::runFrame() {
	if (this->parseUserInput() == exitProgram) return exitProgram;
	this->draw();
	return continueProgram;
}

ui::ui(core& appCore): appCore(appCore) {
	this->initConsole();
	this->initCursor();
}
ui::~ui() {
	//return console to normal functions
	// handled by deconstructor of consoleInformations
}
// ----------------