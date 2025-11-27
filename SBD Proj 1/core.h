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
	functionExitCode UICreateEmptyFile(std::vector<fParUnion> funcParameters);
	functionExitCode UICreateRandomFile(std::vector<fParUnion> funcParameters);
	functionExitCode UIOpenFile(std::vector<fParUnion> funcParameters);
	functionExitCode UIClearFile(std::vector<fParUnion> funcParameters);
	functionExitCode UISortFile(std::vector<fParUnion> funcParameters);
	functionExitCode UISortStep(std::vector<fParUnion> funcParameters);
	functionExitCode UIResetSorting(std::vector<fParUnion> funcParameters);
	functionExitCode UIModifyFileLine(std::vector<fParUnion> funcParameters);
	// --------------------------------

	core();
	~core();
};

