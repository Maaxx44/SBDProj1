#pragma once
#define BLOCK_SIZE 16


#define randomRadiusMin 3.0
#define randomRadiusMax 10.0
#define randomAngleMin 15.0
#define randomAngleMax 360.0
#define randomRecordsMin 48
#define randomRecordsMax 150

#include <random>
std::uniform_real_distribution<double> radiusRandDistr(randomRadiusMin, randomRadiusMax);
std::uniform_real_distribution<double> angleRandDistr(randomAngleMin, randomAngleMax);
std::uniform_real_distribution<unsigned int> recordsRandDistr(randomRecordsMin, randomRecordsMax);
std::default_random_engine randomEngine;