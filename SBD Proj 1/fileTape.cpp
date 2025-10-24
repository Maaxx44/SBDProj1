#include "fileTape.h"

fileTape::fileTape(): dataLength(0) {}
fileTape::fileTape(std::vector<dataBlock> dataFile, unsigned int dataLength): dataLength(dataLength) {
	this->dataFile = dataFile;
}

record fileTape::getRecord(unsigned int recordIndex) {
	if(recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");

	unsigned int blockIndex = ceil((double)recordIndex / (double)BLOCK_SIZE);
	unsigned int offsetIndex = recordIndex - (blockIndex - 1) * BLOCK_SIZE;

	return this->dataFile[blockIndex].getRecord(offsetIndex);
}
void fileTape::setRecord(unsigned int recordIndex, record newRecord) {
	if (recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");

	unsigned int blockIndex = ceil((double)recordIndex / (double)BLOCK_SIZE);
	unsigned int offsetIndex = recordIndex - (blockIndex - 1) * BLOCK_SIZE;

	this->dataFile[blockIndex].setRecord(offsetIndex, newRecord);
}