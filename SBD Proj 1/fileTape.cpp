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

record fileTape::getRecord(unsigned int recordIndex, bool ignoreCIOO) {
	if(recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");

#if RWOpt
	if (!ignoreCIOO && this->countIOOperations && this->cachedBlock != &this->dataFile[getBlockOffset(recordIndex)]) {
		this->cachedBlock = &this->dataFile[getBlockOffset(recordIndex)];
		this->readCount++;
	}
#else
	if (!ignoreCIOO && this->countIOOperations) this->readCount++;
#endif
	return this->dataFile[getBlockOffset(recordIndex)].getRecord(getOffsetIndex(recordIndex));
}
dataBlock fileTape::getBlock(unsigned int blockIndex, bool ignoreCIOO) {
	if(blockIndex >= this->dataFile.size()) throw std::runtime_error("getBlock error: blockIndex(" + std::to_string(blockIndex) + ") out of range(" + std::to_string(this->dataFile.size()) + ")");

#if RWOpt
	if (!ignoreCIOO && this->countIOOperations && this->cachedBlock != &this->dataFile[blockIndex]) {
		this->cachedBlock = &this->dataFile[blockIndex];
		this->readCount++;
	}
#else
	if (!ignoreCIOO && this->countIOOperations) this->readCount++;
#endif
	return this->dataFile[blockIndex];
}
void fileTape::setRecord(unsigned int recordIndex, record newRecord) {
	if (recordIndex >= this->dataLength) throw std::runtime_error("getRecord error: recordIndex(" + std::to_string(recordIndex) + ") out of range(" + std::to_string(this->dataLength) + ")");
	this->dataFile[getBlockOffset(recordIndex)].setRecord(getOffsetIndex(recordIndex), newRecord);

#if RWOpt
	if (this->countIOOperations && this->cachedBlock != &this->dataFile[getBlockOffset(recordIndex)]) {
		this->cachedBlock = &this->dataFile[getBlockOffset(recordIndex)];
		this->writeCount++;
	}
#else
	if (this->countIOOperations) this->writeCount++;
#endif
}
void fileTape::addRecord(record newRecord) {
	// if tape is empty OR last data block is full - create new block and add it as the last one
	if (this->dataLength == 0 || this->dataLength % BLOCK_SIZE == 0) {
		record blockRecords[BLOCK_SIZE] = { newRecord };
		this->dataFile.push_back(dataBlock(blockRecords));
		if (this->countIOOperations) this->writeCount++;

#if RWOpt
		if (this->countIOOperations) this->cachedBlock = &this->dataFile.back();
#endif
	}
	else {
		this->dataFile[getBlockOffset(this->dataLength + 1)].setRecord(getOffsetIndex(this->dataLength + 1), newRecord);

#if RWOpt
		if (this->countIOOperations && this->cachedBlock != &this->dataFile[getBlockOffset(this->dataLength + 1)]) {
			this->cachedBlock = &this->dataFile[getBlockOffset(this->dataLength + 1)];
			this->writeCount++;
		} 
#else
		if (this->countIOOperations) this->writeCount++;
#endif
	}
	this->dataLength++;
}

std::pair<unsigned int, unsigned int> fileTape::getIOOperations() const {
	return { this->readCount , this->writeCount };
}
void fileTape::setIOOCounting(bool newCIOO) {
	this->countIOOperations = newCIOO;
}
void fileTape::resetIOOCounter() {
	this->readCount = 0;
	this->writeCount = 0;
}

unsigned int fileTape::getSize() const {
	return this->dataLength;
}
void fileTape::setSize(unsigned int newSize) {
	this->dataFile.resize(ceil((double)newSize / BLOCK_SIZE), dataBlock());
	this->dataLength = newSize;
}
void fileTape::clear() {
	this->dataFile.clear();
	this->dataLength = 0;
}

bool fileTape::checkIfSorted() {
	for (unsigned int index = 0; index < this->dataLength - 1; index++) {
		if (this->getRecord(index).calculateArea() < this->getRecord(index + 1).calculateArea()) {
			return false;
		}
	}
	return true;
}
void fileTape::dumpToFile(std::ofstream* filePath) {
	for (unsigned int recordIndex = 0; recordIndex < this->dataLength; recordIndex++) {
		record nextRecord = this->getRecord(recordIndex);
		*filePath << '[' << nextRecord.getAngle() << ", " << nextRecord.getRadius() << "]\n";
	}
}
