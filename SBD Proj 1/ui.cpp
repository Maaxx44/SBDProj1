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
	for (unsigned int i = 0; i < contentBox.H; i++) {
		this->cInfo.setCursorPosition(contentBox.X, contentBox.Y + i);

		// Upper barrier

		// Lower barrier
	}


	this->cInfo.setCursorPosition(titleBox.X, titleBox.Y + 1);
	this->changeColor(table.contentColor | COMMON_LVB_GRID_HORIZONTAL);
	printf("               ");

}



ui::ui() {
	this->cInfo.setConsoleTitle("SBD Proj 1");
	this->initConsole();
	/*
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_UNDERSCORE);
	printf("      \n");
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_GRID_RVERTICAL);
	printf(" ");
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("what");
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_GRID_LVERTICAL);
	printf(" \n");
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_GRID_HORIZONTAL);
	printf("      \n");
	this->resetColor();
	*/


	//this->cInfo.setConsoleSize(150, 40);
	//for (int j = 0; j <= 36; j++) {
	//	for (int i = 0; i <= 150; i++) {
	//		printf("*");
	//	}
	//}

	uiTable testTable = uiTable({ .X = 2, .Y = 1, .W = 15, .H = 5 }, "TitleTitle", { "File 1", "File 2", "File 3", "File 4", "File 5"});
	std::vector<std::string> testFiles = testTable.getContent(2);

	//std::cout << testTable.getTitle() << std::endl;
	//for (unsigned int i = 0; i < testFiles.size(); i++)
	//	std::cout << testFiles[i] << "  -  length: " << testFiles[i] .size() << std::endl;

	testTable.titleColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	this->drawTable(testTable);

	//testTable.setPositionToText();
	//testFiles = testTable.getFullContent();
	//
	//std::cout << testTable.getTitle() << std::endl;
	//for (unsigned int i = 0; i < testFiles.size(); i++)
	//	std::cout << testFiles[i] << std::endl;
}
ui::~ui() {}