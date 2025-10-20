#pragma once
#include "merger.h"
#include "fileTape.h"

#include <vector>

class data_manager {
private:
	std::vector<fileTape> storedFiles;
	merger fileMerger;

public:
	data_manager();
	~data_manager();
};

