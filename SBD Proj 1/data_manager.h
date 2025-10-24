#pragma once
#include <vector>

#include "merger.h"
#include "fileTape.h"

class data_manager {
private:
	std::vector<fileTape> storedFiles;
	merger fileMerger;

public:
	data_manager();
	~data_manager();
};

