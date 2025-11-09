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

void ui::drawTable(tableMetadata& table) const {
	// If table is invisible - don't draw it lul
	if (!table.isTableVisible) return;

	//Assume that console size is big enaugh to fit table
	REC titleBox = table.table.getTitleRectangle();
	REC contentBox = table.table.getContentRectangle();

	// Writing title box
	// Getting color for title
	WORD titleColor = 0x00;
	if (table.isTitleHighlighted && table.isTitleSelected)	titleColor = table.titleSelectedHighlightedColor;
	else if (table.isTitleHighlighted)						titleColor = table.titleHighlightedColor;
	else if (table.isTitleSelected)							titleColor = table.titleSelectedColor;
	else													titleColor = table.titleColor;

	std::string titleString = table.table.getTitle();

	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", titleString[0]);
	this->changeColor(titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", titleString.substr(1, titleBox.W - 2).c_str());
	this->changeColor(titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", titleString[14]);


	// Writing content
	std::vector<std::string> drawContent = table.table.getContent(table.contentOffset);
	std::optional<unsigned int> selectedIndex = table.getSelectedContentIndex();
	std::optional<unsigned int> highlightedIndex = table.getHighlightedContentIndex();
	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		//  Getting content color (ouch those if statements)
		WORD contentColor = 0x00;
		if (selectedIndex != std::nullopt && i == selectedIndex && highlightedIndex != std::nullopt && i == highlightedIndex && table.isContentHighlighted && table.isContentSelected)
			contentColor = table.contentSelectedHighlightedColor;
		else if (highlightedIndex != std::nullopt && i == highlightedIndex && table.isContentHighlighted)
			contentColor = table.contentHighlightedColor;
		else if (selectedIndex != std::nullopt && i == selectedIndex && table.isContentSelected)
			contentColor = table.contentSelectedColor;
		else
			contentColor = table.contentColor;
		//  Getting content text
		std::string contentString = drawContent[i];
		//  Getting modifiers for drawing box around content
		WORD modifiers = NULL;
		if (i == 0)					modifiers |= COMMON_LVB_GRID_HORIZONTAL;
		if (i == contentBox.H - 1)	modifiers |= COMMON_LVB_UNDERSCORE;

		// Writing Line
		this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + i);
		this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_LVERTICAL);
		printf("%c", contentString[0]);
		this->changeColor(contentColor | modifiers);
		printf("%s", contentString.substr(1, titleBox.W - 2).c_str());
		this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_RVERTICAL);
		printf("%c", contentString[14]);

	}
}

void ui::initTables() {
	// Creating files table
	this->tFiles = tableMetadata({ .table = uiTable({.X = 2, .Y = 1, .W = 15, .H = 5 }, "Files", { "File 1", "File 2", "File 3", "File 4", "File 5" }) });

	// Creating Options table
	this->tOptions = tableMetadata({ .table = uiTable({.X = 2, .Y = 7, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"}) });
	this->tOptions.isTitleHighlighted = true;
	this->tOptions.isTitleSelected = true;

	// TODO - create sub-options tables
}

void ui::draw() {
	this->drawTable(this->tFiles);
	this->drawTable(this->tOptions);
	for (tableMetadata tSubOption : this->tSubOptions)
		this->drawTable(tSubOption);
}


ui::ui() {
	this->initConsole();
	this->initTables();

	//this->cInfo.setConsoleSize(150, 40);
	//tableMetadata testTable = { .table = uiTable({.X = 2, .Y = 1, .W = 15, .H = 5 }, "Files", { "File 1", "File 2", "File 3", "File 4", "File 5" }) };
	//testTable.isTitleHighlighted = true;
	//testTable.isContentSelected = true;
	//testTable.contentSelectedIndex = 3;
	//testTable.contentOffset = 3;
	//this->drawTable(testTable);

	//tableMetadata testTable2 = { .table = uiTable({.X = 2, .Y = 7, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"}) };
	//this->drawTable(testTable2);
}
ui::~ui() {
	//return console to normal functions

}