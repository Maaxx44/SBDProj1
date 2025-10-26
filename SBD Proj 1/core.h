#pragma once
#include "common.h"
#include "dataManager.h"
#include "ui.h"

class core {
private:
	dataManager currentDataManager;
	ui userInterface;

public:
	core();
	~core();
};

