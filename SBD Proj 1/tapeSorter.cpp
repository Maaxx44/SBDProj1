#include "tapeSorter.h"

tapeSorter::tapeSorter(): mainTape(nullptr), tapeSize(0), numberOfRuns(0) {
	for (unsigned int i = 0; i < this->runSize; i++) {
		this->mainMemory[i].setAngle(0);
		this->mainMemory[i].setRadius(0);
	}
}
tapeSorter::~tapeSorter() { }

void tapeSorter::addTapeToSort(fileTape* inputTape) {
	if (inputTape == nullptr) throw std::runtime_error("addTapeToSort error: inputTape was nullptr!");
	for (unsigned int i = 0; i < this->runSize; i++) {
		this->mainMemory[i].setAngle(0);
		this->mainMemory[i].setRadius(0);
	}

	// Calculating necesarry informations
	this->tapeSize = inputTape->getSize();
	this->numberOfRuns = (unsigned int)std::ceil((double)this->tapeSize / (double)this->runSize);

	// Saving input tape pointer
	this->mainTape = inputTape;

	// Resizing work tape
	this->workTape.clear();
	this->workTape.setSize(this->tapeSize);
}
fileTape& tapeSorter::getWorkTapeP() {
	return this->workTape;
}
void tapeSorter::clean() {
	// Clearing main memory
	for (unsigned int i = 0; i < this->runSize; i++) {
		this->mainMemory[i].setAngle(0);
		this->mainMemory[i].setRadius(0);
	}

	// Clearing work tape
	this->workTape.clear();

	// Removing pointer to main tape
	this->mainTape = nullptr;

	// Clearing informations
	this->tapeSize = 0;
	this->numberOfRuns = 0;
}
bool tapeSorter::isTapeLoaded() const {
	return (this->mainTape != nullptr);
}

void tapeSorter::sortTapeFull() {
	if (this->mainTape == nullptr) throw std::runtime_error("sortTapeFull error: mainTape was nullptr!");
	if (this->mainTape->getSize() <= 1) return;

	// Metadata
	this->workTape.resetReadOperations();
	this->workTape.resetWriteOperations();
	this->mainTape->resetReadOperations();
	this->mainTape->resetWriteOperations();

	// Stage 1
	for (unsigned int runIndex = 0; runIndex < this->numberOfRuns; runIndex++) {
		// load one run into memory
		for(unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - runIndex * runSize); runElementIndex++)
			mainMemory[runElementIndex] = this->mainTape->getRecord(runIndex * runSize + runElementIndex);

		// sort records
		std::sort(mainMemory, mainMemory + min(this->runSize, this->mainTape->getSize() - runIndex * runSize), compareRecordsRef);

		// save record to disk
		for (unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - runIndex * runSize); runElementIndex++) {
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
	for (unsigned int outputIndex = 0; mergingQueue.size() > 0 && outputIndex < this->tapeSize; outputIndex++) {
		/// getting smallest/largest value and removing from queue
		queueElement edgeElement = mergingQueue.top();
		mergingQueue.pop();

		/// saving element to disk
		this->mainTape->setRecord(outputIndex, edgeElement.rData);

		/// pushing new element from the same run to queue (if there are any left from this run)
		if (edgeElement.rElement < runSize - 1 && (edgeElement.rIndex * runSize + edgeElement.rElement + 1) < this->workTape.getSize()) {
			mergingQueue.push(queueElement{
				.rIndex = edgeElement.rIndex, // Same index of the run
				.rElement = edgeElement.rElement + 1, // Get next element of the run
				.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
			});
		}
	}

	// Metadata
  	this->readOperations = this->mainTape->getReadOperations() + this->workTape.getReadOperations();
	this->writeOperations = this->mainTape->getWriteOperations() + this->workTape.getWriteOperations();
}
void tapeSorter::sortNextStage() {
	if (this->mainTape == nullptr) throw std::runtime_error("sortTapeFull error: mainTape was nullptr!");
	if (this->mainTape->getSize() <= 1) return;



	switch (this->currentSortStage) {
	case stage1:
		// Metadata
		this->workTape.resetReadOperations();
		this->workTape.resetWriteOperations();
		this->mainTape->resetReadOperations();
		this->mainTape->resetWriteOperations();

		// Stage 1
		for (unsigned int runIndex = 0; runIndex < this->numberOfRuns; runIndex++) {
			// load one run into memory
			for (unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - runIndex * runSize); runElementIndex++)
				mainMemory[runElementIndex] = this->mainTape->getRecord(runIndex * runSize + runElementIndex);

			// sort records
			std::sort(mainMemory, mainMemory + min(this->runSize, this->mainTape->getSize() - runIndex * runSize), compareRecordsRef);

			// save record to disk
			for (unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - runIndex * runSize); runElementIndex++) {
				this->workTape.setRecord(runIndex * runSize + runElementIndex, mainMemory[runElementIndex]);
			}
		}
		this->currentSortStage = stage2;
		break;
	case stage2:
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
		for (unsigned int outputIndex = 0; mergingQueue.size() > 0 && outputIndex < this->tapeSize; outputIndex++) {
			/// getting smallest/largest value and removing from queue
			queueElement edgeElement = mergingQueue.top();
			mergingQueue.pop();

			/// saving element to disk
			this->mainTape->setRecord(outputIndex, edgeElement.rData);

			/// pushing new element from the same run to queue (if there are any left from this run)
			if (edgeElement.rElement < runSize - 1 && (edgeElement.rIndex * runSize + edgeElement.rElement + 1) < this->workTape.getSize()) {
				mergingQueue.push(queueElement{
					.rIndex = edgeElement.rIndex, // Same index of the run
					.rElement = edgeElement.rElement + 1, // Get next element of the run
					.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
					});
			}
		}
		this->currentSortStage = sorted;

		// Metadata
		this->readOperations = this->mainTape->getReadOperations() + this->workTape.getReadOperations();
		this->writeOperations = this->mainTape->getWriteOperations() + this->workTape.getWriteOperations();

		break;
	}
}
void tapeSorter::sortNextPart() {
	if (this->mainTape == nullptr) throw std::runtime_error("sortTapeFull error: mainTape was nullptr!");
	if (this->mainTape->getSize() <= 1) return;

	switch (this->currentSortStage) {
	case stage1:
		// Stage 1
		if (this->PSRunIndex < this->numberOfRuns) {
			// load one run into memory
			for (unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - PSRunIndex * runSize); runElementIndex++)
				mainMemory[runElementIndex] = this->mainTape->getRecord(PSRunIndex * runSize + runElementIndex);

			// sort records
			std::sort(mainMemory, mainMemory + min(this->runSize, this->mainTape->getSize() - PSRunIndex * runSize), compareRecordsRef);

			// save record to disk
			for (unsigned int runElementIndex = 0; runElementIndex < min(this->runSize, this->mainTape->getSize() - PSRunIndex * runSize); runElementIndex++) {
				this->workTape.setRecord(PSRunIndex * runSize + runElementIndex, mainMemory[runElementIndex]);
			}
			this->PSRunIndex++;
		}
		else {
			// Stage 1.5 (Stage 2 setup)
			//  Clearing up main tape to make room for sorted elements (I think it might not be necesarry if we override all elements with sorted ones, but I guess it is nicer)
			this->mainTape->clear();
			this->mainTape->setSize(this->tapeSize);
			// Iterating over every run and pushing first value to PQ
			for (unsigned int runIndex = 0; runIndex < this->numberOfRuns; runIndex++) {
				this->PSMergingQueue.push(queueElement{
					.rIndex = runIndex,
					.rElement = 0,
					.rData = this->workTape.getRecord(runIndex * runSize)
					});
			}

			this->currentSortStage = stage2;
			this->PSRunIndex = 0;
		}
		break;
	case stage2:
		if (PSMergingQueue.size() > 0 && PSOutputIndex < this->tapeSize) {
			/// getting smallest/largest value and removing from queue
			queueElement edgeElement = PSMergingQueue.top();
			PSMergingQueue.pop();

			/// saving element to disk
			this->mainTape->setRecord(PSOutputIndex, edgeElement.rData);

			/// pushing new element from the same run to queue (if there are any left from this run)
			if (edgeElement.rElement < runSize - 1 && (edgeElement.rIndex * runSize + edgeElement.rElement + 1) < this->workTape.getSize()) {
				PSMergingQueue.push(queueElement{
					.rIndex = edgeElement.rIndex, // Same index of the run
					.rElement = edgeElement.rElement + 1, // Get next element of the run
					.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
					});
			}
			this->PSOutputIndex++;
		}
		else {
			// Metadata
			this->readOperations = this->mainTape->getReadOperations() + this->workTape.getReadOperations();
			this->writeOperations = this->mainTape->getWriteOperations() + this->workTape.getWriteOperations();

			this->currentSortStage = sorted;
			this->PSOutputIndex = 0;
		}
		break;
	}
}

void tapeSorter::resetSorting() {
	// Clearing sorting data
	this->currentSortStage = stage1;
	this->PSMergingQueue = std::priority_queue<queueElement, std::vector<queueElement>, Compare>();
	unsigned int PSRunIndex = 0;
	unsigned int PSOutputIndex = 0;

	// Clearing work data
	this->workTape.clear();
	this->workTape.setSize(this->tapeSize);

	// Metadata
	this->workTape.resetReadOperations();
	this->workTape.resetWriteOperations();
	this->mainTape->resetReadOperations();
	this->mainTape->resetWriteOperations();

	this->readOperations = 0;
	this->writeOperations = 0;
}

std::pair<unsigned int, unsigned int> tapeSorter::getIOperationsCount() const {
	return std::pair<unsigned int, unsigned int>(this->writeOperations, this->readOperations);
}