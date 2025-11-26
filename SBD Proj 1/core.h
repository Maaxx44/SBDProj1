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
	// ---- Functions called by UI ----
	fParUnion UICreateEmptyFile(std::vector<fParUnion> funcParameters);
	fParUnion UICreateRandomFile(std::vector<fParUnion> funcParameters);
	fParUnion UIOpenFile(std::vector<fParUnion> funcParameters);
	fParUnion UIClearFile(std::vector<fParUnion> funcParameters);
	fParUnion UISortFile(std::vector<fParUnion> funcParameters);
	fParUnion UISortStep(std::vector<fParUnion> funcParameters);
	fParUnion UIResetSorting(std::vector<fParUnion> funcParameters);
	fParUnion UIModifyFileLine(std::vector<fParUnion> funcParameters);
	// --------------------------------

	core();
	~core();
};

