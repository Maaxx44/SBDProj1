#pragma once
#include "common.h"
#include "uiTable.h"

enum cursorType {
	tablePointer,
	contentPointer,
	disabled
};

struct cursorInformations {
	std::vector<uiTable*> selectedTables; // vector that tells what tables are selected and in what order (!!!!)
	uiTable& currentTableCursorPoint; // At what table is cursor currently pointing
	unsigned int currentContentCursorPoint; // At what table content is cursor currently pointing
	cursorType cType;


};

