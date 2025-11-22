#include "cursorInformations.h"


cursorInformations::cursorInformations() {
	this->cType = disabled;
	this->currentTableCursorPoint = nullptr;
	this->currentContentCursorPoint = 0;
	this->selectedTables.clear();
}