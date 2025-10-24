#pragma once
#include "dataBlock.h"

#include <vector>

class fileTape {
private:
	std::vector<dataBlock> dataFile;

public:
	fileTape();
	~fileTape();


	fileTape static getFileTapeFromDisk(std::string filePath);
	fileTape static getRandomFileTape(unsigned int numberOfRecords);
	fileTape static getRandomFileTape();

};

