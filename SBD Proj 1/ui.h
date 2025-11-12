#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "uiTable.h"

class ui {
private:
	cInfo cInfo;
	uiTable tFiles, tOptions;
	std::vector<uiTable> tSubOptions;
	std::chrono::time_point<std::chrono::system_clock> currentFrameTS;
	std::chrono::time_point<std::chrono::system_clock> lastFrameTS;
	
	void initConsole();
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	//Helpoer functions for drawing tables
	void drawTableTitle(uiTable& table) const;
	void drawTableContentLine(uiTable& table, std::string contentString, unsigned int laneIndex) const;
	void drawTableContent(uiTable& table) const;


	void drawTable(uiTable& table) const;

	void initTables();

public:
	// TODO - add time delta to make interface flashy
	void draw();

	ui();
	~ui();
};

