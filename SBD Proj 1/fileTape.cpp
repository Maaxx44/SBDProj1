#include "fileTape.h"

unsigned int fileTape::getBlockOffset(unsigned int recordIndex) const {
	return (unsigned int)ceil((double)recordIndex / (double)BLOCK_SIZE);
}
unsigned int fileTape::getOffsetIndex(unsigned int recordIndex) const {
	return recordIndex - (getBlockOffset(recordIndex) - 1) * BLOCK_SIZE;
}

fileTape::fileTape(): dataLength(0) {}
fileTape::fileTape(std::vector<dataBlock> dataFile, unsigned int dataLength): dataLength(dataLength) {
	this->dataFile = dataFile;
}

record fileTape::getRecord(unsigned int recordIndex) {
	if(recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");
	return this->dataFile[getBlockOffset(recordIndex)].getRecord(getOffsetIndex(recordIndex));

	//unsigned int blockIndex = (unsigned int)ceil((double)recordIndex / (double)BLOCK_SIZE);
	//unsigned int offsetIndex = recordIndex - (blockIndex - 1) * BLOCK_SIZE;
	//return this->dataFile[blockIndex].getRecord(offsetIndex);
}
void fileTape::setRecord(unsigned int recordIndex, record newRecord) {
	if (recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");
	this->dataFile[getBlockOffset(recordIndex)].setRecord(getOffsetIndex(recordIndex), newRecord);
}
void fileTape::addRecord(record newRecord) {
	// if tape is empty OR last data block is full - create new block and add it as the last one
	if (this->dataLength == 0 || this->dataLength % BLOCK_SIZE == 0) {
		record blockRecords[BLOCK_SIZE] = { newRecord }; // I hope this works
		this->dataFile.push_back(dataBlock(blockRecords));
		dataLength++;
	}
	else {
		this->dataFile[getBlockOffset(this->dataLength + 1)].setRecord(getOffsetIndex(this->dataLength + 1), newRecord);
		this->dataLength++;
	}
}
unsigned int fileTape::getSize() const {
	return this->dataLength;
}
