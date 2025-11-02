#include "ui.h"

void ui::initConsole() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::resetColor() {
	this->changeColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void ui::changeColor(WORD colorParameters) {
	if (this->cInfo.cHandle != NULL && !SetConsoleTextAttribute(this->cInfo.cHandle, colorParameters))
		ErrorHandler("Failed to set console font attributes!");
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


	this->cInfo.setConsoleSize(150, 40);
	for (int j = 0; j <= 36; j++) {
		for (int i = 0; i <= 150; i++) {
			printf("*");
		}
	}
}
ui::~ui() {}