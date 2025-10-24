#include <iostream>
#include "globalData.cpp"
#include "core.h"

void createExternGlobalVariables() {
	std::uniform_real_distribution<double> radiusRandDistr(randomRadiusMin, randomRadiusMax);
	std::uniform_real_distribution<double> angleRandDistr(randomAngleMin, randomAngleMax);
	std::uniform_int_distribution<unsigned int> recordsRandDistr(randomRecordsMin, randomRecordsMax);
	std::default_random_engine randomEngine;
}

int main() {
	createExternGlobalVariables();
	core core();

	return 0;
}