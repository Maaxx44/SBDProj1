#include "dataManager.h"

dataManager::dataManager() {
	this->storedFiles.clear(); // just to be sure
}
dataManager::~dataManager() {
	this->storedFiles.clear();
}
