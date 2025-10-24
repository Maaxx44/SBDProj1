#pragma once
#include <vector>
#include <fstream>

#include "dataBlock.h"

class fileTape {
private:
	std::vector<dataBlock> dataFile;

public:
	fileTape();
	fileTape(std::vector<dataBlock> dataFile);

	fileTape static getFileTapeFromDisk(FILE filePath);
	fileTape static getRandomFileTape(unsigned int numberOfRecords = 0);

};

