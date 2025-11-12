#include "ui.h"

void ui::initConsole() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	this->cInfo.setConsoleTitle(defaultConsoleTitle);
	this->cInfo.setConsoleSize(defaultConsoleSizeX, defaultConsoleSizeY);
	this->cInfo.setConsoleMode(consoleNoTextMode);
	this->cInfo.setCursorVisibility(false);
}
void ui::resetColor() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) const{
	if (this->cInfo.cHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
}

void ui::drawTableTitle(uiTable& table) const {
	std::string titleString = table.table.getTitle();
	if (!table.titleMod.isTextVisible) return;

	REC titleBox = table.table.getTitleRectangle();
	std::string titleString = table.table.getTitle();
	WORD titleModifiers = table.titleMod.getTextModifier(this->currentFrameTS);

	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", titleString[0]);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", titleString.substr(1, titleBox.W - 2).c_str());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", titleString[14]);
}
void ui::drawTableContentLine(uiTable& table, std::string contentString, unsigned int laneIndex) const {
	if (!table.contentMod[laneIndex].isTextVisible) return;

	WORD contentColor = table.contentMod[laneIndex].getTextModifier(this->currentFrameTS);
	REC contentBox = table.table.getContentRectangle();

	//  Getting modifiers for drawing box around content
	WORD modifiers = NULL;
	if (laneIndex == 0)					modifiers |= COMMON_LVB_GRID_HORIZONTAL;
	if (laneIndex == contentBox.H - 1)	modifiers |= COMMON_LVB_UNDERSCORE;

	// Writing Line
	this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + laneIndex);
	this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", contentString[0]);
	this->changeColor(contentColor | modifiers);
	printf("%s", contentString.substr(1, contentBox.W - 2).c_str());
	this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", contentString[14]);
}
void ui::drawTableContent(uiTable& table) const {
	REC contentBox = table.table.getContentRectangle();
	std::vector<std::string> drawContent = table.table.getContent(table.contentOffset);

	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		drawTableContentLine(table, drawContent[i], i);
	}
}

void ui::drawTable(uiTable& table) const {
	// If table is invisible - don't draw it lul
	if (!table.isTableVisible) return;

	// Draw table title
	drawTableTitle(table);

	// Draw table content
	drawTableContent(table);
}

void ui::initTables() {
	// Creating files table
	this->tFiles = tableMetadata({ .table = uiTable({.X = 2, .Y = 1, .W = 15, .H = 5 }, "Files", { "File 1", "File 2", "File 3", "File 4", "File 5" }) });
	this->tFiles.contentOffset = 2;

	// Creating Options table
	this->tOptions = tableMetadata({ .table = uiTable({.X = 2, .Y = 7, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"}) });

	// TODO - create sub-options tables
}

void ui::draw() {
	// Handle timing
	//  Get current timestamp
	this->currentFrameTS = std::chrono::system_clock::now();

	// Draw tables
	this->drawTable(this->tFiles);
	this->drawTable(this->tOptions);
	for (tableMetadata tSubOption : this->tSubOptions)
		this->drawTable(tSubOption);
}


ui::ui() {
	this->initConsole();
	this->initTables();
}
ui::~ui() {
	//return console to normal functions
	// handled by deconstructor of consoleInformations
}