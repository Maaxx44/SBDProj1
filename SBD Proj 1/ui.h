#pragma once
#include "common.h"
#include "consoleInformations.h"

class ui {
private:
	cInfo cInfo;
	
	void initConsole();
	void changeColor();
	void resetColor();

	void writeBorders();
	void writeData();

public:
	ui();
	~ui();
};

