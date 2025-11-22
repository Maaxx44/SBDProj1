#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "cursorInformations.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	uiTable tFiles, tOptions;
	cursorInformations cursInfo;
	std::vector<uiTable> tSubOptions;
	
	void initConsole();
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	//Helper functions for drawing tables
	void drawTableTitle(uiTable& table) const;
	void drawTableContentLine(uiTable& table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const;
	void drawTableContent(uiTable& table) const;
	void drawTable(uiTable& table) const;

	void initTables();

public:
	void parseUserInput();
	void draw(); // draws in console
	void runFrame(); // doesn't actually num frame per-se, but the spirit is there

	ui();
	~ui();
};

