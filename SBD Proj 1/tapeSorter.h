#pragma once
#include "common.h"
#include "fileTape.h"
#include "queueElement.h"
#include <functional>

enum sortingStage {
	stage1,
	stage2,
	sorted
};

class Compare {
public:
	bool operator() (queueElement A, queueElement B) {
		return A.rData.calculateArea() < B.rData.calculateArea();
	}
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

	// Sorting metadata
	unsigned int readOperations = 0;
	unsigned int writeOperations = 0;
	void saveSotingMetadata();
	
	// for partial sorting - unaffected by whole sorting
	sortingStage currentSortStage;
	std::priority_queue<queueElement, std::vector<queueElement>, Compare> PSMergingQueue;
	unsigned int PSRunIndex = 0;
	unsigned int PSOutputIndex = 0;
	unsigned int PSNextRunInQueue = 0;

public:
	tapeSorter();
	~tapeSorter();


	// Sorting functions
	void sortTapeFull(); // Sorts full tape in one go, without stopping
	void sortNextPart(); // Sorts just barely
	void resetSorting();

	// Metadata functions
	void addTapeToSort(fileTape* inputTape);
	fileTape& getWorkTapeP();
	std::pair<unsigned int, unsigned int> getIOperationsCount();
	unsigned int getRunSize() const;
	unsigned int getRunsCount() const;
	void setIOOperationsCounting(bool newCIOO);
	

	// Misc functions
	bool isTapeLoaded() const;
	void clear();

};

