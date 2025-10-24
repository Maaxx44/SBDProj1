#include "dataBlock.h"

dataBlock::dataBlock() {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		this->blockRecords[i] = record();
	}
}
dataBlock::dataBlock(record blockRecords[BLOCK_SIZE]) {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		this->blockRecords[i] = blockRecords[i];
	}
}

record& dataBlock::getRecord(unsigned int position) {
	if (position >= BLOCK_SIZE) throw std::runtime_error("getRecord error: position(" + std::to_string(position) + ") is out of range(" + std::to_string(BLOCK_SIZE) + ")");
	return *(&this->blockRecords[position]); // probably an error here
}

dataBlock static getRandomDataBlock(unsigned int numberOfRandomRecords = BLOCK_SIZE) {
	if(numberOfRandomRecords > BLOCK_SIZE) throw std::runtime_error("getRandomDataBlock error: numberOfRandomRecords(" + std::to_string(numberOfRandomRecords) + ") out of range(" + std::to_string(BLOCK_SIZE) + ")");
	
	record blockRecords[BLOCK_SIZE];
	for (int i = 0; i < numberOfRandomRecords; i++) {
		blockRecords[i] = record::getRandomRecord();
	}
	for (int i = numberOfRandomRecords; i < BLOCK_SIZE; i++) {
		blockRecords[i] = record();
	}

	return dataBlock(blockRecords);
}