#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "cursorInformations.h"
#include "uiTable.h"

#include "tapeSorter.h"

// FWD declaration
class core;


class ui {
private:
	// UI Data
	cInfo cInfo;
	uiTable tOptions, tFilePreview, tWorkFilePreview, tSortingMetadata;
	cursorInformations cursorInfo;

	// Reference to core for ability of calling Interface <-> Sorter functionality
	core& appCore;

	// Another shady solution
	const textModifiers tableLinesModInterA[2] = { textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_BLUE | FOREGROUND_GREEN), textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY) };
	const textModifiers tableLinesModInterB[2] = { textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_GREEN), textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_GREEN | FOREGROUND_INTENSITY) };

	// DIRTY HACK
	fileTape openedFile;
	tapeSorter sorter;
	
	void initConsole();
	void initTables();

	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	// Helper functions for drawing tables
	void drawTableTitle(uiTable& table) const;
	void drawTableContentLine(uiTable& table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const;
	void drawTableContent(uiTable& table) const;
	void drawTable(uiTable& table) const;

	// Prints empty spaces in place of table to clear inut buffor
	void clearAfterTable(uiTable& table) const;

	// Helper functions for updating tables content
	void updateFilePreviewTable();
	void updateWorkFilePreviewTable();
	void updateSortingDataPreviewTable();
	void updateTables();

	// Helper functions for cursor manipulation
	void selectTable(uiTable* table); // enables blinking mode in table
	void deselectTable(uiTable* table); // disables blinking mode in table
	void changeSelectedTable(uiTable* newSelectedTable, uiTable* oldDeselectedTable);
	void selectContent(unsigned int contentLine);
	void deselectContent(unsigned int contentLine);
	void changeSelectedContent(unsigned int newSelectedLine, unsigned int oldDeselectedLine);

	// helper functions for getting user input
	uiTable createInputTable(COR position, unsigned int width, std::string title) const; // creates input table with given parameters. Will always be 2 height. Returns position of input box
	std::optional<double> getUserInputDouble(std::string customMessage = "Enter new value (double)") const;
	std::optional<unsigned int> getUserInputUInt(std::string customMessage = "Enter new value (unsigned intiger)") const;
	std::optional<std::string> getUserInputString(std::string customMessage = "Enter new string") const;


	// Helper functions for keys input parsing (not string or number input!!!)
	functionExitCode parseTableInput(WORD keyCode);
	functionExitCode parseContentInput(WORD keyCode);

	// DIRTY HACK!
	void executeUserInput(); // Executes functions based on selected table and content


	// Main functions
	functionExitCode parseUserInput();
	void draw(); // draws in console
public:

	functionExitCode runFrame(); // doesn't actually num frame per-se, but the spirit is there

	ui(core& appCore);
	~ui();
};

