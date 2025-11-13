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

void ui::initTables() {
	// Creating files table
	this->tFiles = uiTable({.X = 2, .Y = 1, .W = 15, .H = 7 }, "Files", { "File 1", "File 2", "File 3", "File 4", "File 5" });
	this->tFiles.getContentLineMod(0).isTextSelected = true;
	this->tFiles.getContentLineMod(2).isTextSelected = true;
	this->tFiles.getContentLineMod(4).isTextHighlighted = true;
	this->tFiles.getContentLineMod(1).startBlink();

	this->tFiles.setContentOffset(1);

	// Creating Options table
	this->tOptions = uiTable({.X = 2, .Y = 9, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"});

	// TODO - create sub-options tables
}

void ui::draw() {
	// Draw tables
	this->drawTable(this->tFiles);
	this->drawTable(this->tOptions);
	for (uiTable& tSubOption : this->tSubOptions)
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