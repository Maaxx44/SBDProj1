#pragma once
#include <random>
#define BLOCK_SIZE 16


#define randomRadiusMin 3.0
#define randomRadiusMax 10.0
#define randomAngleMin 15.0
#define randomAngleMax 360.0
#define randomRecordsMin 48
#define randomRecordsMax 150

extern std::uniform_real_distribution<double> radiusRandDistr;
extern std::uniform_real_distribution<double> angleRandDistr;
extern std::uniform_int_distribution<unsigned int> recordsRandDistr;
extern std::default_random_engine randomEngine;