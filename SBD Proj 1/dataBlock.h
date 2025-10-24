#pragma once
#include <string>

#include "record.h"

struct dataBlock {
private:
	record blockRecords[BLOCK_SIZE] = {};

public:
	dataBlock();
	dataBlock(record blockRecords[BLOCK_SIZE]);

	record& getRecord(unsigned int position);

	dataBlock static getRandomDataBlock(unsigned int numberOfRandomRecords = BLOCK_SIZE);
};

