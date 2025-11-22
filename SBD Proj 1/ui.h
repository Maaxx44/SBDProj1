#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "cursorInformations.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	uiTable tFiles, tOptions;
	cursorInformations cursorInfo;
	std::vector<uiTable> tSubOptions;
	
	void initConsole();
	void initTables();

	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	//Helper functions for drawing tables
	void drawTableTitle(uiTable& table) const;
	void drawTableContentLine(uiTable& table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const;
	void drawTableContent(uiTable& table) const;
	void drawTable(uiTable& table) const;

	void selectTable(uiTable* table); // enables blinking mode in table
	void deselectTable(uiTable* table); // disables blinking mode in table
	void changeSelectedTable(uiTable* newSelectedTable, uiTable* oldDeselectedTable);

	void selectContent(unsigned int contentLine);
	void deselectContent(unsigned int contentLine);
	void changeSelectedContent(unsigned int newSelectedLine, unsigned int oldDeselectedLine);

public:
	bool parseUserInput();
	void draw(); // draws in console
	bool runFrame(); // doesn't actually num frame per-se, but the spirit is there - returns false if program is to exiasdfam[fiuyh8gtw

	ui();
	~ui();
};

