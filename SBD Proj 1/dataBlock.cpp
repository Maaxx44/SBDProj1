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

record dataBlock::getRecord(unsigned int position) {
	if (position >= BLOCK_SIZE) throw std::runtime_error("getRecord error: position(" + std::to_string(position) + ") is out of range(" + std::to_string(BLOCK_SIZE) + ")");
	return this->blockRecords[position];
}

void dataBlock::setRecord(unsigned int position, record newRecord) {
	if (position >= BLOCK_SIZE) throw std::runtime_error("getRecord error: position(" + std::to_string(position) + ") is out of range(" + std::to_string(BLOCK_SIZE) + ")");
	this->blockRecords[position] = newRecord;
}
