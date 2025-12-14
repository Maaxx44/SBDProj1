#include "tapeSorter.h"

tapeSorter::tapeSorter(): mainTape(nullptr), tapeSize(0), numberOfRuns(0), currentSortStage(stage1) {
	for (unsigned int i = 0; i < this->runSize; i++) {
		this->mainMemory[i].setAngle(0);
		this->mainMemory[i].setRadius(0);
	}
}
tapeSorter::~tapeSorter() { }

void tapeSorter::saveSotingMetadata() {
	std::pair<unsigned int, unsigned int> mainTapeIOO = { 0, 0 }, workTapeIOO = { 0, 0 };
	if(this->mainTape != nullptr) mainTapeIOO = this->mainTape->getIOOperations();
	workTapeIOO = this->workTape.getIOOperations();
	this->readOperations = mainTapeIOO.first + workTapeIOO.first;
	this->writeOperations = mainTapeIOO.second + workTapeIOO.second;
}

void tapeSorter::addTapeToSort(fileTape* inputTape) {
	if (inputTape == nullptr) throw std::runtime_error("addTapeToSort error: inputTape was nullptr!");

	// Clean existing data
	this->clear();

	// Calculating necesarry informations
	this->tapeSize = inputTape->getSize();
	this->numberOfRuns = (unsigned int)std::ceil((double)this->tapeSize / (double)this->runSize);

	// Saving input tape pointer
	this->mainTape = inputTape;

	// Resizing work tape
	this->workTape.setSize(this->tapeSize);
}
fileTape& tapeSorter::getWorkTapeP() {
	return this->workTape;
}
void tapeSorter::clear() {
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
unsigned int tapeSorter::getRunSize() const {
	return this->runSize;
}
unsigned int tapeSorter::getRunsCount() const {
	return this->numberOfRuns;
}
void tapeSorter::setIOOperationsCounting(bool newCIOO) {
	if(this->mainTape != nullptr)
		this->mainTape->setIOOCounting(newCIOO);
	this->workTape.setIOOCounting(newCIOO);
}
unsigned int tapeSorter::getPhazesCount() {
	return this->phazesCount;
}

void tapeSorter::sortTapeFull() {
	if (this->mainTape == nullptr) throw std::runtime_error("sortTapeFull error: mainTape was nullptr!");
	if (this->mainTape->getSize() <= 1) return;

	// Metadata
	this->workTape.resetIOOCounter();
	this->mainTape->resetIOOCounter();
	this->phazesCount = 0;

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
	}

	// Stage 2
	// Main sorting pointers
	fileTape* outputTape = this->mainTape;
	fileTape* inputTape = &this->workTape;
	std::priority_queue<queueElement, std::vector<queueElement>, Compare> mergingQueue;

	unsigned int currentRunsCount = 0;
	unsigned int currentPhazesCount = 0;
	unsigned int currentRunSize = this->runSize;
	unsigned int currentRunsInputIndex = 0;
	unsigned int outputTapeInsertIndex = 0;

	do {
		outputTapeInsertIndex = 0;
		currentRunsInputIndex = 0;
		currentRunsCount = 0;
		currentPhazesCount++;

		// Possibly remove later
		do {
			// 1: Adding runs to merging queue
			for (unsigned int runIndex = 0; runIndex < blocksPerMemory - 1 && (currentRunsInputIndex * currentRunSize) < this->tapeSize; runIndex++, currentRunsInputIndex++) {
				currentRunsCount++;
				mergingQueue.push(queueElement{
						.rIndex = currentRunsInputIndex,
						.rElement = 0,
						.rData = inputTape->getRecord(currentRunsInputIndex * currentRunSize)
					});
			}

			// 2: merge with merging queue
			while (mergingQueue.size() > 0) {
				// Adding edge element to output file
				queueElement edgeElement = mergingQueue.top();
				mergingQueue.pop();

				outputTape->setRecord(outputTapeInsertIndex, edgeElement.rData);
				outputTapeInsertIndex++;

				// Adding element to merging queue
				if (edgeElement.rElement < currentRunSize - 1 && edgeElement.rElement + 1 + edgeElement.rIndex * currentRunSize < this->tapeSize) {
					mergingQueue.push(queueElement{
						.rIndex = edgeElement.rIndex,
						.rElement = edgeElement.rElement + 1,
						.rData = inputTape->getRecord(edgeElement.rIndex * currentRunSize + edgeElement.rElement + 1, (edgeElement.rElement + 1) % BLOCK_SIZE != 0)
						});
				}
			}
		} while (outputTapeInsertIndex < this->tapeSize);

		fileTape* tempTape = inputTape;
		inputTape = outputTape;
		outputTape = tempTape;

		currentRunSize *= (blocksPerMemory - 1);
	} while (currentRunsCount > 1);
	// We are left with one run what *should* be sorted

	// If the last run is in the work tape we copy it to outr main tape
	// We check for outputTape, because we swap pointers on line 147!!!
	if (outputTape == this->mainTape) {
		for (unsigned int index = 0; index < this->tapeSize; index++) {
			outputTape->setRecord(index, inputTape->getRecord(index, true));
		}
	}

	// Metadata
	this->phazesCount = currentPhazesCount - 1; // adjusting for overcounting
	this->saveSotingMetadata();

	// Sanity check
	if (!this->mainTape->checkIfSorted()) throw std::runtime_error("YOU FUCKED UP SORTING AGAIN!!!");


	// Legacy code
	/*
	if (this->numberOfRuns <= blocksPerMemory) {
		this->mainTape->clear();
		this->mainTape->setSize(this->tapeSize);
		//  Creating prio. queue for mering runs
		std::priority_queue<queueElement, std::vector<queueElement>, Compare> mergingQueue;
		// Iterating over blocksPerMemory runs and pushing first value to PQ
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
	#if RWOpt
						.rIndex = edgeElement.rIndex, // Same index of the run
						.rElement = edgeElement.rElement + 1, // Get next element of the run
						.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1, (edgeElement.rElement + 1) % BLOCK_SIZE != 0) // get next record of the run, dont count towards IOO unless 
	#else
						.rIndex = edgeElement.rIndex, // Same index of the run
						.rElement = edgeElement.rElement + 1, // Get next element of the run
						.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
	#endif
					});
			}
		}
		this->phazesCount = 1;
	}
	else {
		// Main sorting pointers
		fileTape* outputTape = &this->workTape;
		fileTape* inputTape = this->mainTape;
		std::priority_queue<queueElement, std::vector<queueElement>, Compare> mergingQueue;

		unsigned int currentRunsCount = 0;
		unsigned int currentPhazesCount = 0;
		unsigned int currentRunSize = this->runSize;
		unsigned int currentRunsInputIndex = 0;
		unsigned int outputTapeInsertIndex = 0;

		do {
			outputTapeInsertIndex = 0;
			currentRunsInputIndex = 0;
			currentRunsCount = 0;
			currentPhazesCount++;

			do {
				// Adding runs to merging queue
				for (; currentRunsInputIndex < blocksPerMemory - 1 && (currentRunsInputIndex * currentRunSize) < this->tapeSize; currentRunsInputIndex++) {
					currentRunsCount++;
					mergingQueue.push(queueElement{
							.rIndex = currentRunsInputIndex,
							.rElement = 0,
							.rData = inputTape->getRecord(currentRunsInputIndex * currentRunSize)
						});
				}

				// 2: merge with merging queue
				while (mergingQueue.size() > 0) {
					// Adding edge element to output file
					queueElement edgeElement = mergingQueue.top();
					mergingQueue.pop();

					outputTape->setRecord(outputTapeInsertIndex, edgeElement.rData);
					outputTapeInsertIndex++;

					// Adding element to merging queue
					if (edgeElement.rElement < currentRunSize - 1 && edgeElement.rElement + 1 + edgeElement.rIndex * currentRunSize < this->tapeSize) {
						mergingQueue.push(queueElement{
							.rIndex = edgeElement.rIndex,
							.rElement = edgeElement.rElement + 1,
							.rData = inputTape->getRecord(edgeElement.rIndex * currentRunSize + edgeElement.rElement + 1)
						});
					}
				}
			} while (outputTapeInsertIndex != this->tapeSize);
			currentRunSize *= (blocksPerMemory - 1);
		} while (currentRunsCount > 1);
		// We are left with one runt what *should* be sorted
		
		this->phazesCount = currentPhazesCount;

		// Legacy Code
		// Stage 1.5 (Stage 2 setup)
		//  Clearing up main tape to make room for sorted elements (I think it might not be necesarry if we override all elements with sorted ones, but I guess it is nicer)
		this->mainTape->clear();
		this->mainTape->setSize(this->tapeSize);
		//  Creating prio. queue for mering runs
		// Iterating over blocksPerMemory runs and pushing first value to PQ
		for (unsigned int runIndex = 0; runIndex < blocksPerMemory; runIndex++) {
			mergingQueue.push(queueElement{
				.rIndex = runIndex,
				.rElement = 0,
				.rData = this->workTape.getRecord(runIndex * runSize)
				});
		}

		unsigned int nextRunInQueue = blocksPerMemory;
		unsigned int currentRunsCount = blocksPerMemory;

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
#if RWOpt
						.rIndex = edgeElement.rIndex, // Same index of the run
						.rElement = edgeElement.rElement + 1, // Get next element of the run
						.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1, (edgeElement.rElement + 1) % BLOCK_SIZE != 0) // get next record of the run, dont count towards IOO unless 
#else
						.rIndex = edgeElement.rIndex, // Same index of the run
						.rElement = edgeElement.rElement + 1, // Get next element of the run
						.rData = this->workTape.getRecord(edgeElement.rIndex * runSize + edgeElement.rElement + 1) // get next record of the run
#endif
					});
			} // If we reached the end of this tape and there are more runs in queue - add next run to queue
			else if (edgeElement.rElement >= runSize - 1 && nextRunInQueue < this->numberOfRuns) {
				mergingQueue.push(queueElement{
#if RWOpt
						.rIndex = nextRunInQueue, // index of the next run
						.rElement = 0, // First element of run
						.rData = this->workTape.getRecord(nextRunInQueue * runSize) // get next record of the run
#else
						.rIndex = nextRunInQueue, // index of the next run
						.rElement = 0, // First element of run
						.rData = this->workTape.getRecord(nextRunInQueue * runSize) // get next record of the run
#endif
					});
				nextRunInQueue++;
			}
		}
		
	}
	*/
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

			this->saveSotingMetadata();
		}
		else {
			this->mainTape->clear();
			this->mainTape->setSize(this->tapeSize);

			this->currentSortStage = stage2;
			this->PSOutputTape = this->mainTape;
			this->PSInputTape = &this->workTape;
			this->stage2NestedLevel = 0;
			this->PSRunIndex = 0;
			this->PSCurrentRunSize = this->runSize;
			this->PSCurrentPhazesCount = 0;
		}
		/*
		else {
			if (this->numberOfRuns <= blocksPerMemory) {
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
				this->PSOutputTape = this->mainTape;
				this->PSInputTape = &this->workTape;
				this->stage2NestedLevel = 0;
				this->PSRunIndex = 0;
			} else {
				//  Clearing up main tape to make room for sorted elements (I think it might not be necesarry if we override all elements with sorted ones, but I guess it is nicer)
				this->mainTape->clear();
				this->mainTape->setSize(this->tapeSize);
				for (unsigned int runIndex = 0; runIndex < blocksPerMemory; runIndex++) {
					PSMergingQueue.push(queueElement{
						.rIndex = runIndex,
						.rElement = 0,
						.rData = this->workTape.getRecord(runIndex * runSize)
						});
				}

				PSNextRunInQueue = blocksPerMemory;
			}


			this->saveSotingMetadata();
		} */
		break;
	case stage2:
		// 1: Adding runs to merging queue
		for (unsigned int runIndex = 0; runIndex < blocksPerMemory - 1 && (PSCurrentRunsInputIndex * PSCurrentRunSize) < this->tapeSize; runIndex++, PSCurrentRunsInputIndex++) {
			PSCurrentRunsCount++;
			PSMergingQueue.push(queueElement{
					.rIndex = PSCurrentRunsInputIndex,
					.rElement = 0,
					.rData = PSInputTape->getRecord(PSCurrentRunsInputIndex * PSCurrentRunSize)
				});
		}

		// 2: merge with merging queue
		while (PSMergingQueue.size() > 0) {
			// Adding edge element to output file
			queueElement edgeElement = PSMergingQueue.top();
			PSMergingQueue.pop();

			PSOutputTape->setRecord(PSOutputTapeInsertIndex, edgeElement.rData);
			PSOutputTapeInsertIndex++;

			// Adding element to merging queue
			if (edgeElement.rElement < PSCurrentRunSize - 1 && edgeElement.rElement + 1 + edgeElement.rIndex * PSCurrentRunSize < this->tapeSize) {
				PSMergingQueue.push(queueElement{
					.rIndex = edgeElement.rIndex,
					.rElement = edgeElement.rElement + 1,
					.rData = PSInputTape->getRecord(edgeElement.rIndex * PSCurrentRunSize + edgeElement.rElement + 1, (edgeElement.rElement + 1) % BLOCK_SIZE != 0)
					});
			}
		}

		// Metadata
		this->phazesCount = PSCurrentPhazesCount;
		this->saveSotingMetadata();

		if (!(PSOutputTapeInsertIndex < this->tapeSize)) {
			fileTape* tempTape = PSInputTape;
			PSInputTape = PSOutputTape;
			PSOutputTape = tempTape;
			PSCurrentRunSize *= (blocksPerMemory - 1);

			if (!(PSCurrentRunsCount > 1)) {
				if (PSOutputTape == this->mainTape) {
					for (unsigned int index = 0; index < this->tapeSize; index++) {
						PSOutputTape->setRecord(index, PSInputTape->getRecord(index, true));
					}
				}

				// Metadata
				this->phazesCount = PSCurrentPhazesCount;
				this->saveSotingMetadata();

				// Sanity check
				if (!this->mainTape->checkIfSorted()) throw std::runtime_error("YOU FUCKED UP SORTING AGAIN!!!");

				this->currentSortStage = sorted;
			}

			PSOutputTapeInsertIndex = 0;
			PSCurrentRunsInputIndex = 0;
			PSCurrentRunsCount = 0;
			PSCurrentPhazesCount++;
		}
		/*
		do {
			PSOutputTapeInsertIndex = 0;
			PSCurrentRunsInputIndex = 0;
			PSCurrentRunsCount = 0;
			PSCurrentPhazesCount++;

			// Possibly remove later
			do {
				// 1: Adding runs to merging queue
				for (unsigned int runIndex = 0; runIndex < blocksPerMemory - 1 && (PSCurrentRunsInputIndex * PSCurrentRunSize) < this->tapeSize; runIndex++, PSCurrentRunsInputIndex++) {
					PSCurrentRunsCount++;
					PSMergingQueue.push(queueElement{
							.rIndex = PSCurrentRunsInputIndex,
							.rElement = 0,
							.rData = PSInputTape->getRecord(PSCurrentRunsInputIndex * PSCurrentRunSize)
						});
				}

				// 2: merge with merging queue
				while (PSMergingQueue.size() > 0) {
					// Adding edge element to output file
					queueElement edgeElement = PSMergingQueue.top();
					PSMergingQueue.pop();

					PSOutputTape->setRecord(PSOutputTapeInsertIndex, edgeElement.rData);
					PSOutputTapeInsertIndex++;

					// Adding element to merging queue
					if (edgeElement.rElement < PSCurrentRunSize - 1 && edgeElement.rElement + 1 + edgeElement.rIndex * PSCurrentRunSize < this->tapeSize) {
						PSMergingQueue.push(queueElement{
							.rIndex = edgeElement.rIndex,
							.rElement = edgeElement.rElement + 1,
							.rData = PSInputTape->getRecord(edgeElement.rIndex * PSCurrentRunSize + edgeElement.rElement + 1, (edgeElement.rElement + 1) % BLOCK_SIZE != 0)
							});
					}
				}
			} while (PSOutputTapeInsertIndex < this->tapeSize);

			fileTape* tempTape = PSInputTape;
			PSInputTape = PSOutputTape;
			PSOutputTape = tempTape;

			PSCurrentRunSize *= (blocksPerMemory - 1);
		} while (PSCurrentRunsCount > 1);

		if (PSOutputTape == this->mainTape) {
			for (unsigned int index = 0; index < this->tapeSize; index++) {
				PSOutputTape->setRecord(index, PSInputTape->getRecord(index, true));
			}
		}

		// Metadata
		this->phazesCount = PSCurrentPhazesCount - 1; // adjusting for overcounting
		this->saveSotingMetadata();

		// Sanity check
		if (!this->mainTape->checkIfSorted()) throw std::runtime_error("YOU FUCKED UP SORTING AGAIN!!!");
		*/
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
	this->workTape.resetIOOCounter();
	if(this->mainTape != nullptr) this->mainTape->resetIOOCounter();
	this->phazesCount = 0;

	this->readOperations = 0;
	this->writeOperations = 0;
}

std::pair<unsigned int, unsigned int> tapeSorter::getIOperationsCount() {
	this->saveSotingMetadata();
	return std::pair<unsigned int, unsigned int>(this->readOperations, this->writeOperations);
}