#pragma once
#include "common.h"
#include "dataBlock.h"

class fileTape {
private:
	std::vector<dataBlock> dataFile;
	unsigned int dataLength;

public:
	fileTape();
	fileTape(std::vector<dataBlock> dataFile, unsigned int dataLength);

	record getRecord(unsigned int recordIndex);
	void setRecord(unsigned int recordIndex, record newRecord);

	fileTape static getFileTapeFromDisk(std::ifstream* filePath) {
		if (filePath == nullptr || !filePath->is_open()) throw std::runtime_error("getFileTapeFromDisk error: filePath was NULL");
		
		
		filePath->seekg(0);
		std::vector<dataBlock> dataFile;
		unsigned int dataLength = 0;

		for (std::string fileLine; std::getline(*filePath, fileLine);) {
			// TODO: Parse lines, extract values, create blocks and recodrs and insert them into dataFile
		}



		// TODO
	}
	fileTape static getRandomFileTape(unsigned int numberOfRecords = 0) {
		if (numberOfRecords == 0) numberOfRecords = randomRecordsMin + (rand() / (randomRecordsMax - randomRecordsMin)); // if default parameter or parameter set to 0 - get random number in range

		unsigned int numberOfBlocks = ceil((double)numberOfRecords / (double)BLOCK_SIZE);
		unsigned int numberOfLastBlockRecords = numberOfRecords - (numberOfBlocks - 1) * BLOCK_SIZE;

		// Creating and populating random data blocks
		std::vector<dataBlock> dataFile;
		dataFile.resize(numberOfBlocks);

		for (int i = 0; i < numberOfBlocks - 1; i++) {
			dataFile[i] = dataBlock::getRandomDataBlock();
		}
		dataFile[numberOfBlocks - 1] = dataBlock::getRandomDataBlock(numberOfLastBlockRecords);

		//returning new fileTape created with random blocks
		return fileTape(dataFile, numberOfRecords);
	}
};

