#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "tableMetadata.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	std::vector<tableMetadata> tables;
	
	void initConsole() const;
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	void drawTable(tableMetadata& table) const;

public:
	ui();
	~ui();
};

