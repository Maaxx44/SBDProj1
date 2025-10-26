#pragma once
#include "common.h"
#include "tapeSorter.h"
#include "fileTape.h"

class dataManager {
private:
	std::vector<fileTape> storedFiles;

public:
	dataManager();
	~dataManager();
};

