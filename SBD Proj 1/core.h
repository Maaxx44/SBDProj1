#pragma once
#include "data_manager.h"
#include "ui.h"

class core {
private:
	data_manager currentDataManager;
	ui userInterface;

public:
	core();
	~core();
};

