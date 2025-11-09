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
	std::chrono::time_point<std::chrono::system_clock> currentFrameTS;
	
	void initConsole();
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	//Helpoer functions for drawing tables
	WORD getTableTitleColor(tableMetadata& table) const;
	WORD getTableContentLineColor(tableMetadata& table, unsigned int lineIndex, std::optional<unsigned int> selectedLineIndex, std::optional<unsigned int> highlightedLineIndex) const; // use precalcutated indexes to ease up on calculations
	void drawTableTitle(tableMetadata& table) const;
	void drawTableContentLine(tableMetadata& table, std::string contentString, unsigned int laneIndex, std::optional<unsigned int> selectedLineIndex, std::optional<unsigned int> highlightedLineIndex) const;
	void drawTableContent(tableMetadata& table) const;


	void drawTable(tableMetadata& table) const;

	void initTables();

public:
	// TODO - add time delta to make interface flashy
	void draw();

	ui();
	~ui();
};

