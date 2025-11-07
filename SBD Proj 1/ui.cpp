#include "ui.h"

void ui::initConsole() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	this->cInfo.setConsoleTitle("SBD Proj 1");
	this->cInfo.setConsoleSize(150, 50);
}
void ui::resetColor() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) const{
	if (this->cInfo.cHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
}

void ui::drawTable(tableMetadata& table) const {
	//Assume that console size is big enaugh to fit table
	REC titleBox = table.table.getTitleRectangle();
	REC contentBox = table.table.getContentRectangle();

	// Writing title box
	// Getting color for title
	WORD titleColor = table.table.titleColor;
	std::string titleString = table.table.getTitle();
	if (table.isTitleHighlighted) titleColor = table.table.highlightColor;
	else if (table.isTitleSelected) titleColor = table.table.selectColor;

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
	for (unsigned int i = 0; i < (unsigned int)contentBox.H; i++) {
		// Getting color for this line
		WORD contentColor = table.table.contentColor;
		if (selectedIndex != std::nullopt && i == selectedIndex) contentColor = table.table.selectColor;
		std::string contentString = drawContent[i];

		this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + i);

		// Upper and lower barier
		WORD modifiers = NULL;
		if (i == 0)					modifiers |= COMMON_LVB_GRID_HORIZONTAL;
		if (i == contentBox.H - 1)	modifiers |= COMMON_LVB_UNDERSCORE;

		// Writing Line
		this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_LVERTICAL);
		printf("%c", contentString[0]);
		this->changeColor(contentColor | modifiers);
		printf("%s", contentString.substr(1, titleBox.W - 2).c_str());
		this->changeColor(contentColor | modifiers | COMMON_LVB_GRID_RVERTICAL);
		printf("%c", contentString[14]);

	}
}

void ui::initTables() {

}




ui::ui() {
	this->initConsole();

	//this->cInfo.setConsoleSize(150, 40);
	tableMetadata testTable = { .table = uiTable({.X = 2, .Y = 1, .W = 15, .H = 5 }, "Files", { "File 1", "File 2", "File 3", "File 4", "File 5" }) };
	//testTable.isTitleHighlighted = true;
	//testTable.isContentSelected = true;
	//testTable.contentSelectedIndex = 3;
	//testTable.contentOffset = 3;
	this->drawTable(testTable);

	tableMetadata testTable2 = { .table = uiTable({.X = 2, .Y = 7, .W = 15, .H = 5 }, "Options", {"Add File", "Edit File", "Sort File", "Quit"}) };
	this->drawTable(testTable2);



	//testTable.setPositionToText();
	//testFiles = testTable.getFullContent();
	//
	//std::cout << testTable.getTitle() << std::endl;
	//for (unsigned int i = 0; i < testFiles.size(); i++)
	//	std::cout << testFiles[i] << std::endl;
}
ui::~ui() {}