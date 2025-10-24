#pragma once
#include <string>

#include "common.h"
#include "record.h"

struct dataBlock {
private:
	record blockRecords[BLOCK_SIZE] = {};

public:
	dataBlock();
	dataBlock(record blockRecords[BLOCK_SIZE]);

	record getRecord(unsigned int position);
	void setRecord(unsigned int position, record newRecord);

	dataBlock static getRandomDataBlock(unsigned int numberOfRandomRecords = BLOCK_SIZE) {
		if (numberOfRandomRecords > BLOCK_SIZE) throw std::runtime_error("getRandomDataBlock error: numberOfRandomRecords(" + std::to_string(numberOfRandomRecords) + ") out of range(" + std::to_string(BLOCK_SIZE) + ")");

		record blockRecords[BLOCK_SIZE];
		for (int i = 0; i < numberOfRandomRecords; i++) {
			blockRecords[i] = record::getRandomRecord();
		}
		for (int i = numberOfRandomRecords; i < BLOCK_SIZE; i++) {
			blockRecords[i] = record();
		}

		return dataBlock(blockRecords);
	}
};

