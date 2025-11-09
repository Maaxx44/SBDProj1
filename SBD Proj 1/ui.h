#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "tableMetadata.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	tableMetadata tFiles, tOptions;
	std::vector<tableMetadata> tSubOptions;
	
	void initConsole();
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	void drawTable(tableMetadata& table) const;

	void initTables();

public:
	// TODO - add time delta to make interface flashy
	void draw();

	ui();
	~ui();
};

