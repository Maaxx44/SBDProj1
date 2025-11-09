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


WORD ui::getTableTitleColor(tableMetadata& table) const {
	switch (table.isTitleHighlighted ? 1 : 0 + table.isTitleSelected ? 2 : 0) {
	case 0: // no modifiers
		return table.titleColor;
	case 1: // title is highlighted
		return table.titleHighlightedColor; // TODO - check timing
	case 2: // title is selected
		return table.titleSelectedColor; // TODO - check timing
	case 3: // title is both highlighted and selected
		return table.titleSelectedHighlightedColor; // TODO - check timing
	}
}
WORD ui::getTableContentLineColor(tableMetadata& table, unsigned int lineIndex, std::optional<unsigned int> selectedLineIndex, std::optional<unsigned int> highlightedLineIndex) const {
	// This is *I think* the mos optimal way of checking modifiers of line color, but it is ugly as fu... heck. Ugly as heck.
	switch ((highlightedLineIndex != std::nullopt && lineIndex == highlightedLineIndex) ? 1 : 0 + (selectedLineIndex != std::nullopt && lineIndex == selectedLineIndex.value()) ? 2 : 0) {
	case 0: // no modifiers
		return table.contentColor;
	case 1: // title is highlighted
		return table.contentHighlightedColor; // TODO - check timing
	case 2: // title is selected
		return table.contentSelectedColor; // TODO - check timing
	case 3: // title is both highlighted and selected
		return table.contentSelectedHighlightedColor; // TODO - check timing
	}
}
void ui::drawTableTitle(tableMetadata& table) const {
	REC titleBox = table.table.getTitleRectangle();
	std::string titleString = table.table.getTitle();
	WORD titleModifiers = getTableTitleColor(table);

	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", titleString[0]);
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", titleString.substr(1, titleBox.W - 2).c_str());
	this->changeColor(titleModifiers | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", titleString[14]);
}
void ui::drawTableContentLine(tableMetadata& table, std::string contentString, unsigned int laneIndex, std::optional<unsigned int> selectedLineIndex, std::optional<unsigned int> highlightedLineIndex) const {
	WORD contentColor = getTableContentLineColor(table, laneIndex, selectedLineIndex, highlightedLineIndex); // can be diffrent for every line
	REC contentBox = table.table.getContentRectangle(); // low on calculations - can be used in function

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
void ui::drawTableContent(tableMetadata& table) const {
	REC contentBox = table.table.getContentRectangle();
	std::vector<std::string> drawContent = table.table.getContent(table.contentOffset);
	std::optional<unsigned int> selectedLineIndex = table.getSelectedContentIndex();
	std::optional<unsigned int> highlightedLineIndex = table.getHighlightedContentIndex();

	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		drawTableContentLine(table, drawContent[i], i, selectedLineIndex, highlightedLineIndex);
	}
}

void ui::drawTable(tableMetadata& table) const {
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
	this->tFiles.isTitleHighlighted = true;
	this->tFiles.contentSelectedIndex = 3;
	this->tFiles.isContentSelected = true;
	this->tFiles.contentOffset = 2;

	// Creating Options table
	this->tOptions = tableMetadata({ .table = uiTable({.X = 2, .Y = 7, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"}) });
	this->tOptions.isContentSelected = true;

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