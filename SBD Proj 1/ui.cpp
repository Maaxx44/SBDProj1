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
	this->tWorkFilePreview = uiTable({ .X = 76, .Y = 1, .W = 45, .H = 35 }, "Work File Preview", {}); // work file preview will not be editable
	this->tSortingMetadata = uiTable({ .X = 2, .Y = 15, .W = 25, .H = 21 }, "Sorting Data", {});

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
double ui::parseNumberInput() {
	// dirty hack! CHANGE LATER
	this->cInfo.setCursorPosition(0, 0);
	std::cout << "Enter number: ";
	double returnValue = 0;
	std::cin >> returnValue;

	this->cInfo.setCursorPosition(0, 0);
	for (unsigned int i = 0; i < this->cInfo.getCurrentDwSize().X; i++)
		std::cout << " ";

	return returnValue;
}
double ui::parseNumberInput(std::string customMessage) {
	// dirty hack! CHANGE LATER
	this->cInfo.setCursorPosition(0, 0);
	std::cout << customMessage;
	double returnValue = 0;
	std::cin >> returnValue;

	this->cInfo.setCursorPosition(0, 0);
	for (unsigned int i = 0; i < this->cInfo.getCurrentDwSize().X; i++)
		std::cout << " ";

	return returnValue;
}


void ui::executeUserInput() {
	if (this->cursorInfo.currentTableCursorPoint == &this->tOptions) { 
		switch (this->cursorInfo.currentContentCursorPoint) {
			//TODO - SET TO FINAL FUNCTIONS !!!!
		case 0: // "Empty File"
			this->openedFile.clear();
			this->openedFile.setSize((unsigned int)parseNumberInput("Enter no. records: "));
			this->sorter.clean();
			this->sorter.addTapeToSort(&this->openedFile);
			this->updateFilePreview();
			this->updateWorkFilePreview();
			break;
		case 1: // "Random File"
			this->openedFile = fileTape::getRandomFileTape((unsigned int)parseNumberInput("Enter no. records: "));
			this->sorter.clean();
			this->sorter.addTapeToSort(&this->openedFile);
			this->updateFilePreview();
			this->updateWorkFilePreview();
			break;
		case 2: // "Open File"
			break;
		case 3: // "Clear File"
			this->openedFile.clear();
			this->sorter.clean();
			this->updateFilePreview();
			this->updateWorkFilePreview();
			break;
		case 4: // "Sort file"
			this->sorter.clean();
			this->sorter.addTapeToSort(&this->openedFile);
			this->sorter.sortTapeFull();
			this->updateFilePreview();
			this->updateWorkFilePreview();
			this->updateSortingDataPreview();
				break;
		case 5: // "Sort step"
			this->sorter.sortNextPart();
			this->updateFilePreview();
			this->updateWorkFilePreview();
			this->updateSortingDataPreview();
				break;
		case 6: // "Reset sorting"
			this->sorter.resetSorting();
			this->updateFilePreview();
			this->updateWorkFilePreview();
			this->updateSortingDataPreview();
			break;
		}
	}
	else if (this->cursorInfo.currentTableCursorPoint == &this->tFilePreview) {
		this->cursorInfo.currentContentCursorPoint;
		double userAngle = std::fmod(parseNumberInput("Enter angle:"), 360.0);
		double userRadius = parseNumberInput("Enter radius:");
		if (userRadius <= 0.0) userRadius = 5;

		this->openedFile.setRecord(this->cursorInfo.currentContentCursorPoint, record(userAngle, userRadius));
		this->updateFilePreview();
		this->tFilePreview.getContentLineMod(this->cursorInfo.currentContentCursorPoint).startFlash();
		this->selectContent(this->cursorInfo.currentContentCursorPoint);
	}
}
void ui::updateFilePreview() {
	// aprsing numbers into strings
	std::vector<std::string> parsedFile;
	for (unsigned int i = 0; i < this->openedFile.getSize(); i++) {
		record nextRecord = this->openedFile.getRecord(i);
		parsedFile.push_back("A: " + std::to_string(nextRecord.getAngle()) + ", R: " + std::to_string(nextRecord.getRadius()) + " = " + std::to_string(nextRecord.calculateArea()));
	}
	this->tFilePreview.setContent(parsedFile);
}
void ui::updateWorkFilePreview() {
	std::vector<std::string> parsedFile;
	for (unsigned int i = 0; i < this->sorter.getWorkTapeP().getSize(); i++) {
		record nextRecord = this->sorter.getWorkTapeP().getRecord(i);
		parsedFile.push_back("A: " + std::to_string(nextRecord.getAngle()) + ", R: " + std::to_string(nextRecord.getRadius()) + " = " + std::to_string(nextRecord.calculateArea()));
	}
	this->tWorkFilePreview.setContent(parsedFile);
}
void ui::updateSortingDataPreview() {
	std::pair<unsigned int, unsigned int> IOOperations = this->sorter.getIOperationsCount();
	std::vector<std::string> sortingMetadata;
	sortingMetadata.push_back("Reads: " + std::to_string(IOOperations.first));
	sortingMetadata.push_back("Writes: " + std::to_string(IOOperations.second));
	this->tSortingMetadata.setContent(sortingMetadata);
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

ui::ui() {
	this->initConsole();
	this->initTables();

}
ui::~ui() {
	//return console to normal functions
	// handled by deconstructor of consoleInformations
}