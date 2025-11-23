#include "core.h"

core::core() {
	while (this->userInterface.runFrame()) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}




	//DEGUB - TEMP - REMOVE LATER - checking sorting
	//fileTape randomTape = fileTape::getRandomFileTape(128);
	//
	//tapeSorter newSorter;
	//newSorter.addTapeToSort(&randomTape);
	//newSorter.sortTapeFull();
	//
	//for (unsigned int i = 0; i < 24/*randomTape.getSize()*/; i++) {
	//	std::cout << i << ": [A=" << randomTape.getRecord(i).getAngle() << ", R=" << randomTape.getRecord(i).getRadius() << "] => Area: " << randomTape.getRecord(i).calculateArea() << std::endl;
	//}
}
core::~core() {}
