#pragma once
#include "common.h"
#include "dataBlock.h"

class fileTape {
private:
	std::vector<dataBlock> dataFile;
	unsigned int dataLength;

	unsigned int readCount;
	unsigned int writeCount;

	unsigned int getBlockOffset(unsigned int recordIndex) const;
	unsigned int getOffsetIndex(unsigned int recordIndex) const;

public:
	fileTape();
	fileTape(std::vector<dataBlock> dataFile, unsigned int dataLength);

	dataBlock getBlock(unsigned int blockIndex);
	record getRecord(unsigned int recordIndex);
	unsigned int getReadOperations() const;
	unsigned int getWriteOperations() const;
	unsigned int getSize() const;

	void resetReadOperations();
	void resetWriteOperations();
	void setReadOperations(unsigned int operationsCount);
	void setWriteOperations(unsigned int operationsCount);
	void setRecord(unsigned int recordIndex, record newRecord);
	void addRecord(record newRecord);
	void setSize(unsigned int newSize);
	void clear();

	fileTape static getFileTapeFromDisk(std::ifstream* filePath) {
		if (filePath == nullptr || !filePath->is_open()) throw std::runtime_error("getFileTapeFromDisk error: filePath was NULL");
		
		
		filePath->seekg(0);
		std::vector<dataBlock> dataFile;
		record			tempBlock[BLOCK_SIZE];
		unsigned int	totalDataLength = 0,
						recordIndex = 0;
		double			tempAngle = 0.0,
						tempRadius = 0.0;
		char			c = ' ';

		for (std::string fileLine; std::getline(*filePath, fileLine);) {
			std::stringstream ss(fileLine);

			while (!ss.eof()) {
				while (c != '[' && !ss.eof()) { ss >> c; }	// seeking [ bracket
				if (ss.eof()) continue;							// Skip this line
				ss >> tempRadius;							// parse first number
				if (ss.eof()) continue;							// Skip this line
				while (c != ';' && !ss.eof()) { ss >> c; }	// seeking ;
				if (ss.eof()) continue;							// Skip this line
				ss >> tempAngle;							// parse second number
				if (ss.eof()) continue;							// Skip this line
				while (c != ']' && !ss.eof()) { ss >> c; }	// seeking ] bracket
				if (ss.eof()) continue;							// Skip this line

				// numbers parsed successfully
				tempBlock[recordIndex] = record(tempAngle, tempRadius);
				totalDataLength++;
				recordIndex++;

				// if we reached the end of a block - add it to file and clear tempBlock
				if (recordIndex >= BLOCK_SIZE) {
					dataFile.push_back(dataBlock(tempBlock));
					for (unsigned int i = 0; i < BLOCK_SIZE; i++) tempBlock[i] = record(); //clean temp block

					recordIndex = 0;
				}
			}
		}

		// Add incomplete blocks at the end
		if (recordIndex != 0) {
			dataFile.push_back(dataBlock(tempBlock));
		}

		return fileTape(dataFile, totalDataLength);
	}
	fileTape static getRandomFileTape(unsigned int numberOfRecords = 0) {
		if (numberOfRecords == 0) numberOfRecords = randomRecordsMin + (rand() / (randomRecordsMax - randomRecordsMin)); // if default parameter or parameter set to 0 - get random number in range

		unsigned int numberOfBlocks = (unsigned int)ceil((double)numberOfRecords / (double)BLOCK_SIZE);
		unsigned int numberOfLastBlockRecords = numberOfRecords - (numberOfBlocks - 1) * BLOCK_SIZE;

		// Creating and populating random data blocks
		std::vector<dataBlock> dataFile;
		dataFile.resize(numberOfBlocks);

		for (unsigned int i = 0; i < numberOfBlocks - 1; i++) {
			dataFile[i] = dataBlock::getRandomDataBlock();
		}
		dataFile[numberOfBlocks - 1] = dataBlock::getRandomDataBlock(numberOfLastBlockRecords);

		//returning new fileTape created with random blocks
		return fileTape(dataFile, numberOfRecords);
	}
};

