#pragma once
#include "common.h"
#include "consoleInformations.h"
#include "cursorInformations.h"
#include "uiTable.h"
#include "tapeSorter.h"

class core;

class ui {
private:
	// UI Data
	cInfo cInfo;
	cursorInformations cursorInfo;
	std::vector<uiTable*> pProgramTables;

	// Reference to core for ability of calling Interface <-> Sorter functionality
	core& appCore;

	// Initialization functions
	void initConsole();
	void initCursor();

	// Text manipulation functions
	void changeColor(WORD colorParameters) const;
	void resetColor() const;

	// Table drawing functions
	void drawTableTitle(uiTable* table) const;
	void drawTableContentLine(uiTable* table, std::string contentString, textModifiers& contentMod, unsigned int laneIndex) const;
	void drawTableContent(uiTable* table) const;
	void drawTable(uiTable* table) const;
	void clearAfterTable(uiTable* table) const; // Prints empty spaces in place of table to clear inut buffor

	// User alpha-numerical input functions
	uiTable createInputTable(COR position, unsigned int width, std::string title) const; // creates input table with given parameters. Will always be 2 height. Returns position of input box

	// UI navigation functions
	functionExitCode parseTableInput(WORD keyCode);
	functionExitCode parseContentInput(WORD keyCode);

	// Main functions
	functionExitCode parseUserInput();
	void draw();

public:
	// User AN input functions - public, so they can get called from core class
	std::optional<double> getUserInputDouble(std::string customMessage = "Enter new value (double)") const;
	std::optional<unsigned int> getUserInputUInt(std::string customMessage = "Enter new value (unsigned intiger)") const;
	std::optional<std::string> getUserInputString(std::string customMessage = "Enter new string") const;

	// Cursor manipulation functions
	void selectTable(uiTable* table); // enables blinking mode in table
	void deselectTable(uiTable* table); // disables blinking mode in table
	void changeSelectedTable(uiTable* newSelectedTable, uiTable* oldDeselectedTable);
	void selectContent(unsigned int contentLine); // enables blinking mode in tables content line
	void deselectContent(unsigned int contentLine); // disables blinking mode in tables content line
	void changeSelectedContent(unsigned int newSelectedLine, unsigned int oldDeselectedLine);
	void setCursor(uiTable* selectedTable);

	void addTable(uiTable* newPTable);

	// Main UI function - all functions lead to this one
	functionExitCode runFrame();

	ui(core& appCore);
	~ui();
};

class core {
private:
	ui userInterface;

	// TODO
	fileTape openedTape;
	tapeSorter sorter;

	// TODO - TEMP
	uiTable tOptions, tFilePreview, tWorkFilePreview, tSortingMetadata;
	const textModifiers tableLinesModInterA[2] = { textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_BLUE | FOREGROUND_GREEN), textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY) };
	const textModifiers tableLinesModInterB[2] = { textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_GREEN), textModifiers(true, false, true, defaultTextColor, defaultHighLightColor, FOREGROUND_GREEN | FOREGROUND_INTENSITY) };

	// TODO
	functionExitCode UICreateEmptyFile();
	functionExitCode UICreateRandomFile();
	functionExitCode UIOpenFile();
	functionExitCode UIClearFile();
	functionExitCode UISortFile();
	functionExitCode UISortStep();
	functionExitCode UIResetSorting();
	functionExitCode UIModifyFileLine(unsigned int selectedContent);

	// Table content updating functions
	void updateFilePreviewTable();
	void updateWorkFilePreviewTable();
	void updateSortingDataPreviewTable();

	void initTables();

public:
	// ---- Functions called by UI ----
	functionExitCode callTableFunction(uiTable* selectedTable, unsigned int selectedContent);
	void updateTables();
	// --------------------------------

	core();
	~core();
};