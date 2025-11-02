#pragma once
#include "common.h"
#include "consoleInformations.h"

class ui {
private:
	cInfo cInfo;
	
	void initConsole();
	void changeColor(WORD colorParameters);
	void resetColor();

	void createTable(std::vector<std::string> titleLines, std::vector<std::string> contentLines) {

	}



	void writeBorders();
	void writeData();

public:
	ui();
	~ui();
};

