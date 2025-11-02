#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	std::vector<uiTable> tables;
	
	void initConsole();
	void changeColor(WORD colorParameters);
	void resetColor();

	void drawFullTables();
	void drawTablesContent();

public:
	ui();
	~ui();
};

