#include "data_manager.h"

data_manager::data_manager() {
	this->storedFiles.clear(); // just to be sure
}
data_manager::~data_manager() {
	this->storedFiles.clear();
}
