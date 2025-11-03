#include "ui.h"

void ui::initConsole() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::resetColor() const {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) const{
	if (this->cInfo.cHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
}

void ui::drawTable(uiTable& table) const {
	//Assume that console size is big enaugh to fit table
	REC titleBox = table.getTitleRectangle();
	REC contentBox = table.getContentRectangle();

	// Writing title box
	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y);
	this->changeColor(table.titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_LVERTICAL);
	printf("%c", table.getTitle()[0]);
	this->changeColor(table.titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL);
	printf("%s", table.getTitle().substr(1, titleBox.W - 2).c_str());
	this->changeColor(table.titleColor | COMMON_LVB_UNDERSCORE | COMMON_LVB_GRID_HORIZONTAL | COMMON_LVB_GRID_RVERTICAL);
	printf("%c", table.getTitle()[14]);

	// Writing content
	std::vector<std::string> drawContent = table.getContent(0);
	for (unsigned int i = 0; i < contentBox.H; i++) {
		this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + i);

		// Upper and lower barier
		WORD modifiers = NULL;
		if (i == 0)					modifiers |= COMMON_LVB_GRID_HORIZONTAL;
		if (i == contentBox.H - 1)	modifiers |= COMMON_LVB_UNDERSCORE;

		// Writing Line
		this->changeColor(table.contentColor | modifiers | COMMON_LVB_GRID_LVERTICAL);
		printf("%c", drawContent[i][0]);
		this->changeColor(table.contentColor | modifiers);
		printf("%s", drawContent[i].substr(1, titleBox.W - 2).c_str());
		this->changeColor(table.contentColor | modifiers | COMMON_LVB_GRID_RVERTICAL);
		printf("%c", drawContent[i][14]);

	}
}



ui::ui() {
	this->cInfo.setConsoleTitle("SBD Proj 1");
	this->initConsole();

	//this->cInfo.setConsoleSize(150, 40);

	uiTable testTable = uiTable({ .X = 2, .Y = 1, .W = 15, .H = 5 }, "TitleTitle", { "File 1", "File 2", "File 3", "File 4", "File 5"});
	std::vector<std::string> testFiles = testTable.getContent(2);

	//std::cout << testTable.getTitle() << std::endl;
	//for (unsigned int i = 0; i < testFiles.size(); i++)
	//	std::cout << testFiles[i] << "  -  length: " << testFiles[i] .size() << std::endl;

	testTable.titleColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	testTable.contentColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	this->drawTable(testTable);

	testTable.resizeTable({ .X = 17, .Y = 1, .W = 15, .H = 5 });
	testTable.titleColor = FOREGROUND_RED;
	testTable.contentColor =  FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	this->drawTable(testTable);

	//testTable.setPositionToText();
	//testFiles = testTable.getFullContent();
	//
	//std::cout << testTable.getTitle() << std::endl;
	//for (unsigned int i = 0; i < testFiles.size(); i++)
	//	std::cout << testFiles[i] << std::endl;
}
ui::~ui() {}