#pragma once
#include "dataBlock.h"

#include <vector>
#include <fstream>

class fileTape {
private:
	std::vector<dataBlock> dataFile;

public:
	fileTape();
	fileTape(std::vector<dataBlock> dataFile);

	fileTape static getFileTapeFromDisk(FILE filePath);
	fileTape static getRandomFileTape(unsigned int numberOfRecords = 0);

};

