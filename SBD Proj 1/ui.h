#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	std::vector<uiTable> tables;
	
	void initConsole() const;
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	void drawTable(uiTable& table) const;

public:
	ui();
	~ui();
};

