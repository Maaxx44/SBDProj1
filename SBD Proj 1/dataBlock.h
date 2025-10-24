#pragma once
#include "record.h"
#include "globalData.hpp"

struct dataBlock {
private:
	record blockRecords[BLOCK_SIZE] = {};

public:
	dataBlock();
	dataBlock(record blockRecords[BLOCK_SIZE]);

	dataBlock static getRandomDataBlock();
};

