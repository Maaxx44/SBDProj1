#include "fileTape.h"

unsigned int fileTape::getBlockOffset(unsigned int recordIndex) const {
	return (unsigned int)floor((double)recordIndex / (double)BLOCK_SIZE);
}
unsigned int fileTape::getOffsetIndex(unsigned int recordIndex) const {
	return recordIndex - (getBlockOffset(recordIndex)) * BLOCK_SIZE;
}

fileTape::fileTape(): dataLength(0), readCount(0), writeCount(0) {}
fileTape::fileTape(std::vector<dataBlock> dataFile, unsigned int dataLength): dataLength(dataLength), readCount(0), writeCount(0) {
	this->dataFile = dataFile;
}

record fileTape::getRecord(unsigned int recordIndex) {
	if(recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");
	this->readCount++;
	return this->dataFile[getBlockOffset(recordIndex)].getRecord(getOffsetIndex(recordIndex));
}
dataBlock fileTape::getBlock(unsigned int blockIndex) {
	if(blockIndex >= this->dataFile.size()) throw std::runtime_error("getBlock error: blockIndex(" + std::to_string(blockIndex) + ") out of range(" + std::to_string(this->dataFile.size()) + ")");
	return this->dataFile[blockIndex];
}

void fileTape::setRecord(unsigned int recordIndex, record newRecord) {
	if (recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");
	this->writeCount++;
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
void fileTape::setSize(unsigned int newSize) {
	this->dataFile.resize(newSize, dataBlock());
	this->dataLength = newSize;
}
void fileTape::clear() {
	this->dataFile.clear();
	this->dataLength = 0;
}

unsigned int fileTape::getReadOperations() const {
	return this->readCount;
}
unsigned int fileTape::getWriteOperations() const {
	return this->writeCount;
}

void fileTape::resetReadOperations() {
	this->readCount = 0;
}
void fileTape::resetWriteOperations() {
	this->writeCount = 0;
}