#pragma once
#include "common.h"
#include "consoleInformations.h"

class ui {
private:
	cInfo cInfo;
	
	void initConsole();
	void changeColor();
	void resetColor();

public:
	ui();
	~ui();
};

