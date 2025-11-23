#include "tapeSorter.h"

tapeSorter::tapeSorter(): mainTape(nullptr), mainMemory(nullptr), tapeSize(0), runSize(0), numberOfRuns(0) {}
tapeSorter::~tapeSorter() {
	if (mainMemory != nullptr) delete[] this->mainMemory;
}

void tapeSorter::addTapeToSort(fileTape* inputTape) {
	if (inputTape == nullptr) throw std::runtime_error("addTapeToSort error: inputTape was nullptr!");
	if (mainMemory != nullptr) delete[] this->mainMemory;

	// Calculating necesarry informations
	this->tapeSize = inputTape->getSize();
	this->runSize  = blocksPerMemory * BLOCK_SIZE;
	this->numberOfRuns = (unsigned int)std::ceil((double)this->tapeSize / (double)this->runSize);

	// Creating main memory (aka. RAM)
	this->mainMemory = new record[runSize];

	// Saving input tape pointer
	this->mainTape = inputTape;

	// Resizing work tape
	this->workTape.clear();
	this->workTape.setSize(this->tapeSize);
}
void tapeSorter::clean() {
	// Clearing main memory
	if (mainMemory != nullptr) delete[] this->mainMemory;

	// Clearing work tape
	this->workTape.clear();

	// Removing pointer to main tape
	this->mainTape = nullptr;

	// Clearing informations
	this->tapeSize = 0;
	this->runSize = 0;
	this->numberOfRuns = 0;
}

class Compare {
public:
	bool operator() (queueElement A, queueElement B) {
		return A.rData.calculateArea() < B.rData.calculateArea();
	}
};

void tapeSorter::sortTapeFull() {
	if (this->mainTape == nullptr) throw std::runtime_error("sortTapeFull error: mainTape was nullptr!");

	// Stage 1
	for (unsigned int runIndex = 0; runIndex < this->numberOfRuns; runIndex++) {
		// load one run into memory
		for(unsigned int runElementIndex = 0; runElementIndex < this->runSize; runElementIndex++)
			mainMemory[runElementIndex] = this->mainTape->getRecord(runIndex * runSize + runElementIndex);

		// sort records
		std::sort(mainMemory, mainMemory + runSize, compareRecordsRef);

		// save record to disk
		for (unsigned int runElementIndex = 0; runElementIndex < this->runSize; runElementIndex++) {
			this->workTape.setRecord(runIndex * runSize + runElementIndex, mainMemory[runElementIndex]);
		}

		// LEGACY VERSION
		//for (unsigned int blockIndex = 0; blockIndex < blocksPerMemory; blockIndex++)
		//	for (unsigned int recordIndex = 0; recordIndex < BLOCK_SIZE; recordIndex++)
		//		mainMemory[recordIndex + blockIndex * BLOCK_SIZE] = this->mainTape->getRecord(recordIndex + blockIndex * BLOCK_SIZE);
		//
		// sort records
		//std::sort(mainMemory, mainMemory + runSize, compareRecordsRef);
		//
		// save record to disk
		//for (unsigned int mainMemoryRecordIndex = 0; mainMemoryRecordIndex < runSize; mainMemoryRecordIndex++) {
		//	tempTape.setRecord(runIndex * runSize + mainMemoryRecordIndex, mainMemory[mainMemoryRecordIndex]); // TODO - CHECK IF MATH IS CORRECT
		//}
	}

	// Stage 1.5 (Stage 2 setup)
	//  Clearing up main tape to make room for sorted elements (I think it might not be necesarry if we override all elements with sorted ones, but I guess it is nicer)
	this->mainTape->clear();
	this->mainTape->setSize(this->tapeSize);
	//  Creating prio. queue for mering runs
	std::priority_queue<queueElement, std::vector<queueElement>, Compare> mergingQueue;
	// Iterating over every run and pushing first value to PQ
	for (unsigned int runIndex = 0; runIndex < this->numberOfRuns; runIndex++) {
		mergingQueue.push(queueElement{
			.rIndex = runIndex,
			.rElement = 0,
			.rData = this->workTape.getRecord(runIndex * runSize)
		});
	}

	// Stage 2
	for (unsigned int outputIndex = 0; mergingQueue.size() > 0; outputIndex++) {
		/// getting smallest/largest value and removing from queue
		queueElement edgeElement = mergingQueue.top();
		mergingQueue.pop();

		/// saving element to disk
		this->mainTape->setRecord(outputIndex, edgeElement.rData);

		/// pushing new element from the same run to queue (if there are any left from this run)
		if (edgeElement.rElement < runSize - 1) {
			mergingQueue.push(queueElement{
				.rIndex = edgeElement.rIndex, // Same index of the run
				.rElement = edgeElement.rElement + 1, // Get next element of the run
				.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
			});
		}
	}
}

void tapeSorter::sortFileLegacy(fileTape* inputTape) {
	// Stage 0 (Stage 1 setup)
	/// Create runs (reserve space on disk) and get variables
	const unsigned int inputSize = inputTape->getSize();
	const unsigned int runSize = blocksPerMemory * BLOCK_SIZE;
	unsigned int runsCount = (unsigned int)std::ceil((double)inputSize / (double)runSize) + 1; // one more run for the merging run
	record* mainMemory = new record[runSize];
	fileTape tempTape;
	tempTape.setSize(inputTape->getSize());



	// Stage 1
	/// sort parts into runs
	for (unsigned int runIndex = 0; runIndex < runsCount - 1; runIndex++) {
		/// load records into memory
		for (unsigned int blockIndex = 0; blockIndex < blocksPerMemory; blockIndex++)
			for (unsigned int recordIndex = 0; recordIndex < BLOCK_SIZE; recordIndex++)
				mainMemory[recordIndex + blockIndex * BLOCK_SIZE] = inputTape->getRecord(recordIndex + blockIndex * BLOCK_SIZE);

		/// sort records
		std::sort(mainMemory, mainMemory + runSize, compareRecordsRef);

		/// save record to disk
		for (unsigned int mainMemoryRecordIndex = 0; mainMemoryRecordIndex < runSize; mainMemoryRecordIndex++) {
			tempTape.setRecord(runIndex * runSize + mainMemoryRecordIndex, mainMemory[mainMemoryRecordIndex]); // TODO - CHECK IF MATH IS CORRECT
		}
	}



	// Stage 1.5 (Stage 2 setup)
	/// clearing oryginal tape to make space for sorted data
	unsigned int inputTapeSize = inputTape->getSize();
	std::priority_queue<queueElement, std::vector<queueElement>, queueElement> mergingQueue;
	inputTape->clear();
	inputTape->setSize(inputTapeSize);

	/// Iterating over every run and pushing first value to PQ
	for (unsigned int runIndex = 0; runIndex < runsCount - 1; runIndex++) {
		mergingQueue.push(queueElement{ .rIndex = runIndex, .rElement = 0, .rData = tempTape.getRecord(runIndex * runSize) });
	}


	// Stage 2
	for (unsigned int outputIndex = 0; mergingQueue.size() > 0; outputIndex++) {
		/// getting smallest/largest value and removing from queue
		queueElement edgeElement = mergingQueue.top();
		mergingQueue.pop();

		/// saving element to disk
		inputTape->setRecord(outputIndex, edgeElement.rData);

		/// pushing new element from the same run to queue (if there are any left)
		if (edgeElement.rElement < runSize - 1) {
			queueElement nextElement{
				.rIndex = edgeElement.rIndex, // Same index of the run
				.rElement = edgeElement.rElement + 1, // Get next element of the run
				.rData = tempTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
			};
			mergingQueue.push(nextElement);
		}
	}
}