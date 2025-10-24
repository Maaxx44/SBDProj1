#include "fileTape.h"

fileTape::fileTape() {}
fileTape::fileTape(std::vector<dataBlock> dataFile) {
	this->dataFile = dataFile;
}