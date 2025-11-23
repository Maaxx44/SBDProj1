#pragma once
#include "common.h"
#include "fileTape.h"
#include "queueElement.h"
#include <functional>

enum sortingStage {
	stage1,
	stage2
};

class tapeSorter {
private:
	// Disk files
	fileTape* mainTape;
	fileTape  workTape;

	// Main memory
	record mainMemory[(blocksPerMemory*BLOCK_SIZE)];

	// Sorting data
	unsigned int tapeSize;
	const unsigned int runSize = (blocksPerMemory*BLOCK_SIZE);
	unsigned int numberOfRuns;
	
	// for partial sorting - unaffected by whole sorting
	sortingStage currentSortStage;

public:
	tapeSorter();
	~tapeSorter();

	void addTapeToSort(fileTape* inputTape);
	void clean();

	void sortTapeFull(); // Sorts full tape in one go, without stopping
	void sortNextStage(); // Iterates over every step of sorting

	bool isTapeLoaded() const;

};

