#include "fileTape.h"

fileTape::fileTape() {}
fileTape::fileTape(std::vector<dataBlock> dataFile) {
	this->dataFile = dataFile;
}

fileTape static getFileTapeFromDisk(FILE filePath) {
	// TODO
}
fileTape static getRandomFileTape(unsigned int numberOfRecords = 0) {
	if (numberOfRecords == 0) numberOfRecords = recordsRandDistr(randomEngine); // if default parameter or parameter set to 0 - get random number in range

	unsigned int numberOfBlocks = ceil(numberOfRecords / BLOCK_SIZE);
	unsigned int numberOfLastBlockRecords = numberOfRecords - (numberOfBlocks - 1) * BLOCK_SIZE;
	
	// Creating and populating random data blocks
	std::vector<dataBlock> dataFile;
	dataFile.resize(numberOfBlocks);

	for (int i = 0; i < numberOfBlocks - 1; i++) {
		dataFile[i] = dataBlock::getRandomDataBlock();
	}
	dataFile[numberOfBlocks - 1] = dataBlock::getRandomDataBlock(numberOfLastBlockRecords);

	//returning new fileTape created with random blocks
	return fileTape(dataFile);
}