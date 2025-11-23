#pragma once
#include "common.h"
#include "fileTape.h"
#include "queueElement.h"
#include <functional>

class tapeSorter {
private:
	// Disk files
	fileTape* mainTape;
	fileTape  workTape;

	// Main memory
	record* mainMemory;

	// Sorting data
	unsigned int tapeSize;
	unsigned int runSize;
	unsigned int numberOfRuns;



public:
	tapeSorter();
	~tapeSorter();

	void addTapeToSort(fileTape* inputTape);
	void clean();

	void sortTapeFull(); // Sorts full tape in one go, without stopping

	bool isTapeLoaded() const;

	void sortFileLegacy(fileTape* inputTape);
};

