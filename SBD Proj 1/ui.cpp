#include "ui.h"

void ui::initConsole() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	this->cInfo.setConsoleTitle(defaultConsoleTitle);
	this->cInfo.setConsoleSize(defaultConsoleSizeX, defaultConsoleSizeY);
	this->cInfo.setConsoleMode(consoleNoTextMode);
	this->cInfo.setCursorVisibility(false);
}
void ui::initTables() {
	// Creating tables
	this->tOptions = uiTable({ .X = 2, .Y = 1, .W = 25, .H = 13 }, "Options", { "Create empty file", "Create random file", "Open file", "Clear file", "Sort file", "Make sorting step", "Reset sorting"});
	this->tFilePreview = uiTable({ .X = 29, .Y = 1, .W = 45, .H = 35 }, "File Preview", {});
	this->tWorkFilePreview = uiTable({ .X = 76, .Y = 1, .W = 45, .H = 35 }, "Work File Preview", {}); // not editable
	this->tSortingMetadata = uiTable({ .X = 2, .Y = 15, .W = 25, .H = 21 }, "Sorting Data", {}); // not editable

	// Changing title select color to corresponding one
	this->tFilePreview.getTitleMod().cSelected = tableLinesModInterA[1].cSelected;
	this->tWorkFilePreview.getTitleMod().cSelected = tableLinesModInterB[1].cSelected;

	// Creating links
	this->tOptions.setTablePointers(uiTablePointer(&this->tSortingMetadata, &this->tSortingMetadata, &this->tWorkFilePreview, &this->tFilePreview));
	this->tFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tOptions, &this->tWorkFilePreview));
	this->tWorkFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tFilePreview, &this->tOptions));
	this->tSortingMetadata.setTablePointers(uiTablePointer(&this->tOptions, &this->tOptions, &this->tWorkFilePreview, &this->tFilePreview));

	// Initializing cursor with it pointing to options table
	this->cursorInfo.cType = tablePointer;
	this->cursorInfo.currentContentCursorPoint = 0;
	this->cursorInfo.currentTableCursorPoint = &this->tOptions;
	this->cursorInfo.selectedTables.clear();
	this->selectTable(&this->tOptions);
}

void ui::resetColor() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) const{
	if (this->cInfo.cOutputHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cOutputHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
}

void ui::drawTableTitle(uiTable& table) const {
	REC titleBox = table.getTitleRectangle();
	WORD titleModifiers = table.getTitleMod().getTextModifier();
	std::string titleString = std::string(titleBox.W, ' ');
	if (table.getTitleMod().isTextVisible) titleString = table.getTitle(); // If title is visible - get title string


	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", titleString.front());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", titleString.substr(1, titleBox.W - 2).c_str());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", titleString.back());
}
void ui::drawTableContentLine(uiTable& table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const {
	REC contentBox = table.getContentRectangle();
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
void ui::drawTableContent(uiTable& table) const {
	REC contentBox = table.getContentRectangle();
	std::vector<std::string> drawContent = table.getContent();
	std::vector<textModifiers> drawMod = table.getContentMod();

	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		drawTableContentLine(table, drawContent[i], drawMod[i], i);
	}
}
void ui::drawTable(uiTable& table) const {
	table.updateTiming();

	// If table is invisible - don't draw it lul
	if (!table.getTableVisible()) return;

	// Draw table title
	drawTableTitle(table);

	// Draw table content
	drawTableContent(table);
}

void ui::clearAfterTable(uiTable& table) const {
	// Gathering data
	REC tablePosition = table.getTableRectangle();
	const std::string emptyLine = std::string(tablePosition.W, ' ');

	// Clearing after table
	this->changeColor(defaultTextColor);
	for (unsigned int line = 0; line < tablePosition.H; line++) {
		this->cInfo.setCursorPosition(tablePosition.X, tablePosition.Y + line);
		printf(emptyLine.c_str());
	}
}


void ui::selectTable(uiTable* table) {
	table->getTitleMod().startBlink();
}
void ui::deselectTable(uiTable* table) {
	table->getTitleMod().stopBlink();
}
void ui::changeSelectedTable(uiTable* newSelectedTable, uiTable* oldDeselectedTable) {
	selectTable(newSelectedTable);
	deselectTable(oldDeselectedTable);
}
void ui::selectContent(unsigned int contentLine) {
	if (this->cursorInfo.currentTableCursorPoint == nullptr) throw std::runtime_error("currentTableCursorPointer is null!");
	this->cursorInfo.currentTableCursorPoint->getContentLineMod(contentLine).startBlink();
}
void ui::deselectContent(unsigned int contentLine) {
	if (this->cursorInfo.currentTableCursorPoint == nullptr) throw std::runtime_error("currentTableCursorPointer is null!");
	this->cursorInfo.currentTableCursorPoint->getContentLineMod(contentLine).stopBlink();
}
void ui::changeSelectedContent(unsigned int newSelectedLine, unsigned int oldDeselectedLine) {
	if (this->cursorInfo.currentTableCursorPoint == nullptr) throw std::runtime_error("currentTableCursorPointer is null!");
	selectContent(newSelectedLine);
	deselectContent(oldDeselectedLine);
}

// helper functions for getting user input
uiTable ui::createInputTable(COR position, unsigned int width, std::string title) const {
	uiTable inputTable({ .X = position.X, .Y = position.Y, .W = width, .H = 2 }, title, { "" });
	inputTable.setTableVisible(false);

	return inputTable;
}
std::optional<double> ui::getUserInputDouble(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableDoubleDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(inputTable);

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
			this->clearAfterTable(inputTable);
			return std::nullopt;
		}
		else if (keyCode >= 0x30 && keyCode <= 0x39) { // Add number if there is space 
			if(unparsedUserInput.size() < tableWidth)
				unparsedUserInput.push_back('0' + keyCode - 0x30);
		}
		else if (!usedComma && (keyCode == VK_OEM_PERIOD || keyCode == VK_OEM_COMMA)) { // Verify if no comma has been placed and add comma 
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
				this->clearAfterTable(inputTable);
				return std::nullopt;
			}

			// Return parsed value
			this->clearAfterTable(inputTable);
			return std::optional<double>(returnValue);
		}

		// Update user input inside table
		inputTable.setContent({ unparsedUserInput });
		drawTable(inputTable);
	}


	this->clearAfterTable(inputTable);
	return std::nullopt;
}
std::optional<unsigned int> ui::getUserInputUInt(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableUIntDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(inputTable);

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
			this->clearAfterTable(inputTable);
			return std::nullopt;
		}
		else if (keyCode >= 0x30 && keyCode <= 0x39) { // Add number if there is space 
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
				this->clearAfterTable(inputTable);
				return std::nullopt;
			}

			// Return parsed value
			this->clearAfterTable(inputTable);
			return std::optional<unsigned int>(returnValue);
		}

		// Update user input inside table
		inputTable.setContent({ unparsedUserInput });
		drawTable(inputTable);
	}


	this->clearAfterTable(inputTable);
	return std::nullopt;
}
std::optional<std::string> ui::getUserInputString(std::string customMessage) const {
	unsigned int tableWidth = max(inputTableStringDefaultWidth, (unsigned int)customMessage.size());

	// Creating input table
	uiTable inputTable = this->createInputTable({ inputTableDefaultX, inputTableDefaultY }, tableWidth, customMessage);
	inputTable.setTableVisible(true);
	drawTable(inputTable);

	// Parsing user input


	return "";
}


functionExitCode ui::parseTableInput(WORD keyCode) {
	switch (keyCode) {
	case VK_UP:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().up != nullptr) {
			this->changeSelectedTable(this->cursorInfo.currentTableCursorPoint->getTablePointers().up, this->cursorInfo.currentTableCursorPoint); // change blinking to new selected table
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().up; // change cursor to new selected table
		}
		break;
	case VK_DOWN:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().down != nullptr) {
			this->changeSelectedTable(this->cursorInfo.currentTableCursorPoint->getTablePointers().down, this->cursorInfo.currentTableCursorPoint); // change blinking to new selected table
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().down; // change cursor to new selected table
		}
		break;
	case VK_LEFT:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().left != nullptr) {
			this->changeSelectedTable(this->cursorInfo.currentTableCursorPoint->getTablePointers().left, this->cursorInfo.currentTableCursorPoint); // change blinking to new selected table
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().left; // change cursor to new selected table
		}
		break;
	case VK_RIGHT:
		if (this->cursorInfo.currentTableCursorPoint != nullptr && this->cursorInfo.currentTableCursorPoint->getTablePointers().right != nullptr) {
			this->changeSelectedTable(this->cursorInfo.currentTableCursorPoint->getTablePointers().right, this->cursorInfo.currentTableCursorPoint); // change blinking to new selected table
			this->cursorInfo.currentTableCursorPoint = this->cursorInfo.currentTableCursorPoint->getTablePointers().right; // change cursor to new selected table
		}
		break;
	case VK_ESCAPE:
		if (cursorInfo.selectedTables.size() == 0) //end program
			return exitProgram;
		else {
			if (cursorInfo.selectedTables.back() != this->cursorInfo.currentTableCursorPoint) {
				this->changeSelectedTable(cursorInfo.selectedTables.back(), this->cursorInfo.currentTableCursorPoint); // change blinking to new selected table
				this->cursorInfo.currentTableCursorPoint = cursorInfo.selectedTables.back(); // change cursor to new selected table
				cursorInfo.selectedTables.pop_back(); // just remove last element
			}
		}
		break;
	case VK_RETURN:
		// If there is no table content - dont go
		if (this->cursorInfo.currentTableCursorPoint->getContentSize() == 0) break;

		// First - *select *current table title and disable blinking
		this->deselectTable(this->cursorInfo.currentTableCursorPoint);
		this->cursorInfo.currentTableCursorPoint->getTitleMod().isTextSelected = true;

		// Change cursor settings
		this->cursorInfo.cType = contentPointer;
		this->cursorInfo.currentContentCursorPoint = 0;

		// Set content blinking
		selectContent(this->cursorInfo.currentContentCursorPoint);

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
			this->changeSelectedContent(newContentCursorPointer, this->cursorInfo.currentContentCursorPoint);
			this->cursorInfo.currentContentCursorPoint = newContentCursorPointer;
		}
		else {
			this->changeSelectedContent(this->cursorInfo.currentContentCursorPoint - 1, this->cursorInfo.currentContentCursorPoint);
			this->cursorInfo.currentContentCursorPoint--;
		}
		// Check if out element is in visible range
		if (!this->cursorInfo.currentTableCursorPoint->isContentLineVisible(this->cursorInfo.currentContentCursorPoint))
			this->cursorInfo.currentTableCursorPoint->setContentOffset(this->cursorInfo.currentTableCursorPoint->getOffsetForLine(this->cursorInfo.currentContentCursorPoint));
		break;
	case VK_DOWN:
		if (this->cursorInfo.currentContentCursorPoint == this->cursorInfo.currentTableCursorPoint->getContentSize() - 1) {
			this->changeSelectedContent(0, this->cursorInfo.currentContentCursorPoint);
			this->cursorInfo.currentContentCursorPoint = 0;
		}
		else {
			this->changeSelectedContent(this->cursorInfo.currentContentCursorPoint + 1, this->cursorInfo.currentContentCursorPoint);
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
		this->deselectContent(this->cursorInfo.currentContentCursorPoint);

		// Select last table, disable title highlight and enable blinking
		this->cursorInfo.currentTableCursorPoint = cursorInfo.selectedTables.back();
		cursorInfo.selectedTables.back()->getTitleMod().isTextSelected = false;
		this->selectTable(cursorInfo.selectedTables.back());
		cursorInfo.selectedTables.pop_back();

		break;
	case VK_RETURN:
		executeUserInput();
		break;
	}
	return continueProgram;
}

void ui::executeUserInput() {
	if (this->cursorInfo.currentTableCursorPoint == &this->tOptions) { 
		// Initializing possible user input - to please the compiler
		std::optional<unsigned int> userInput(std::nullopt);


		switch (this->cursorInfo.currentContentCursorPoint) {
			//TODO - SET TO FINAL FUNCTIONS !!!!
		case 0: // "Empty File"
			userInput = getUserInputUInt(" Enter number of records:");
			if (userInput != std::nullopt) {
				// User did not cancel operation - create new empty file
				this->openedFile.clear();
				this->openedFile.setSize(userInput.value());
				this->sorter.addTapeToSort(&this->openedFile);
			}
			break;
		case 1: // "Random File"
			userInput = getUserInputUInt(" Enter number of records:");
			if (userInput != std::nullopt) {
				// User did not cancel operation - create new random file
				this->openedFile.clear();
				this->openedFile = fileTape::getRandomFileTape(userInput.value());
				this->sorter.addTapeToSort(&this->openedFile);
			}
			break;
		case 2: // "Open File"
			// TODO
			break;
		case 3: // "Clear File"
			this->openedFile.clear();
			this->sorter.clear();
			break;
		case 4: // "Sort file"
			this->sorter.addTapeToSort(&this->openedFile);
			this->sorter.sortTapeFull();
				break;
		case 5: // "Sort step"
			if(!this->sorter.isTapeLoaded())
				this->sorter.addTapeToSort(&this->openedFile);
			this->sorter.sortNextPart();
				break;
		case 6: // "Reset sorting"
			this->sorter.resetSorting();
			break;
		}
		this->updateTables();
	}
	else if (this->cursorInfo.currentTableCursorPoint == &this->tFilePreview) {
		this->cursorInfo.currentContentCursorPoint;

		double userAngle = 0.0, userRadius = 0.0;
		std::optional<double> userInput;

		bool correctValueEntered = false;
		std::string message = " Enter angle:";

		// Getting angle
		while (!correctValueEntered) {
			userInput = getUserInputDouble(message);

			if (userInput == std::nullopt)
				return;
			else if (userInput > 360.0)
				message = " Angle cannot be more than 360. Enter correct angle:";
			else {
				correctValueEntered = true;
				userAngle = userInput.value();
			}
		}

		// Getting radius
		correctValueEntered = false;
		message = " Enter radius:";
		while (!correctValueEntered) {
			userInput = getUserInputDouble(message);

			if (userInput == std::nullopt)
				return;
			else {
				correctValueEntered = true;
				userRadius = userInput.value();
			}
		}

		// Setting values
		this->openedFile.setRecord(this->cursorInfo.currentContentCursorPoint, record(userAngle, userRadius));
		this->updateTables();
		this->tFilePreview.getContentLineMod(this->cursorInfo.currentContentCursorPoint).startFlash();
		this->selectContent(this->cursorInfo.currentContentCursorPoint);
	}
}
void ui::updateFilePreviewTable() {
	// Required fields for table
	std::vector<std::string> parsedFile;
	std::vector<textModifiers> parsedFileMod;

	// For every content in opened file - get and parse its values into string
	for (unsigned int i = 0; i < this->openedFile.getSize(); i++) {
		record nextRecord = this->openedFile.getRecord(i);

		// Get values and parse them into stringstream (we cau use setw)
		std::stringstream ss;
		ss << " A: " << std::setw(10) << std::to_string(nextRecord.getAngle()) << ", R: " << std::setw(10) << std::to_string(nextRecord.getRadius()) << " = " << std::setw(11) << std::to_string(nextRecord.calculateArea());

		// Push back values into vectors
		parsedFile.push_back(ss.str());
		parsedFileMod.push_back(tableLinesModInterA[i % 2]);
	}

	// Setting tables content
	this->tFilePreview.setContent(parsedFile);
	this->tFilePreview.setContentMod(parsedFileMod);

	// Adjusting file readn and write counter, because we use those operations to display data
	this->openedFile.setReadOperations(this->openedFile.getReadOperations() - this->openedFile.getSize());

}
void ui::updateWorkFilePreviewTable() {
	std::vector<std::string> parsedFile;
	std::vector<textModifiers> parsedFileMod;
	for (unsigned int i = 0; i < this->sorter.getWorkTapeP().getSize(); i++) {
		record nextRecord = this->sorter.getWorkTapeP().getRecord(i);
		this->sorter.getWorkTapeP().setReadOperations(this->sorter.getWorkTapeP().getReadOperations() - 1); // adjusting for dispalying
		std::stringstream ss;
		ss << " A: " << std::setw(10) << std::to_string(nextRecord.getAngle()) << ", R: " << std::setw(10) << std::to_string(nextRecord.getRadius()) << " = " << std::setw(11) << std::to_string(nextRecord.calculateArea());
		parsedFile.push_back(ss.str());
		parsedFileMod.push_back(tableLinesModInterB[i % 2]);
	}
	this->tWorkFilePreview.setContent(parsedFile);
	this->tWorkFilePreview.setContentMod(parsedFileMod);
}
void ui::updateSortingDataPreviewTable() {
	std::pair<unsigned int, unsigned int> IOOperations = this->sorter.getIOperationsCount();
	unsigned int runsCount = this->sorter.getRunsCount();
	unsigned int runSize = this->sorter.getRunSize();
	std::vector<std::string> sortingMetadata;
	sortingMetadata.push_back("Reads: " + std::to_string(IOOperations.first));
	sortingMetadata.push_back("Writes: " + std::to_string(IOOperations.second));
	sortingMetadata.push_back("Run size: " + std::to_string(runSize));
	sortingMetadata.push_back("Runs: " + std::to_string(runsCount));
	this->tSortingMetadata.setContent(sortingMetadata);
}
void ui::updateTables() {
	this->updateFilePreviewTable();
	this->updateWorkFilePreviewTable();
	this->updateSortingDataPreviewTable();
}

functionExitCode ui::parseUserInput() {
	if (cursorInfo.cType == disabled) return continueProgram;
	if (this->cursorInfo.currentTableCursorPoint == nullptr)
		this->cursorInfo.currentTableCursorPoint = &this->tOptions;


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
	// Draw tables
	this->drawTable(this->tOptions);
	this->drawTable(this->tFilePreview);
	this->drawTable(this->tWorkFilePreview);
	this->drawTable(this->tSortingMetadata);

	// Return cursor to normal modifiers
	this->changeColor(defaultTextColor);
}
functionExitCode ui::runFrame() {
	if (this->parseUserInput() == exitProgram) return exitProgram;
	this->draw();
	return continueProgram;
}

ui::ui(core&appCore): appCore(appCore) {
	this->initConsole();
	this->initTables();
	this->updateTables();
}
ui::~ui() {
	//return console to normal functions
	// handled by deconstructor of consoleInformations
}