#pragma once
#include "common.h"
#include "ui.h"

class core {
private:
	ui userInterface;

	// TODO
	fileTape openedTape;
	tapeSorter sorter;

	// Structure containing funstions list for calling when user select options in UI (maybie attach to uiTable????)

public:




	core();
	~core();
};

