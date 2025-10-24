#pragma once
#include "common.h"
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

