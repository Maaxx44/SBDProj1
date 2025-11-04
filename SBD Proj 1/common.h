#pragma once
#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif
#include <random>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <optional>

#define BLOCK_SIZE 16

// For random data generation
#define randomRadiusMin 3.0
#define randomRadiusMax 10.0
#define randomAngleMin 15.0
#define randomAngleMax 360.0
#define randomRecordsMin 48
#define randomRecordsMax 150

// For UI
#define stringCutOffMin 5 // if box has more than 5 width - the last 3 chars of string will be replaced with '...'
#define defaultTextColor FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define defaultHighLightColor FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_REVERSE_VIDEO
#define defaultSelectColor FOREGROUND_GREEN | FOREGROUND_INTENSITY

typedef struct RECTANGLE {
	int X, Y, W, H;
} RECTANGLE, REC;

typedef struct COORDINATES {
	int X, Y;
} COORDINATES, COR;