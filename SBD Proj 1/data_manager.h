#pragma once
#include "common.h"
#include "fileTape.h"

class data_manager {
private:
	std::vector<fileTape> storedFiles;

public:
	data_manager();
	~data_manager();
};

