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
#include <thread>
#include <chrono>
#include <algorithm>
#include <queue>
#include <Windows.h> //i hate it i hate it i hate it i hate it i hate it i hate it i hate it i hate it i hate it i hate it

// Data parameters
#define BLOCK_SIZE 8 // how many records in a block
#define blocksPerMemory 1 // how many block can we read into memory

// Random data generation
#define randomRadiusMin 3.0
#define randomRadiusMax 10.0
#define randomAngleMin 45.0
#define randomAngleMax 360.0
#define randomRecordsMin 48
#define randomRecordsMax 150

// UI
//  Table drawing
#define stringCutOffMin 5 // if box has more than 5 width - the last 3 chars of string will be replaced with '...'
#define defaultTextColor FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define defaultHighLightColor FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_REVERSE_VIDEO
#define defaultSelectColor FOREGROUND_GREEN
#define defaultSelectHighlightColor FOREGROUND_GREEN | COMMON_LVB_REVERSE_VIDEO
#define defaultBlinkColor FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | COMMON_LVB_REVERSE_VIDEO
#define defaultFlashColor FOREGROUND_RED | FOREGROUND_INTENSITY
//  Small toggle to define behavoiur
#define returnNoColorOnInvisible false
//  Console drawing
#define defaultConsoleSizeX 150
#define defaultConsoleSizeY 50
#define defaultConsoleSizeToBufferOffset 1 // console API requires console buffer to be larger than size (i don't know how that works, documentation is shit about that) so we add default offset to buffer
#define defaultConsoleTitle "SBD Proj 1"
//  Console parameters
#define consoleNoTextMode 0x0000 | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_LVB_GRID_WORLDWIDE | DISABLE_NEWLINE_AUTO_RETURN 
//  Timing parameters
#define isSelectionFlashingEnabled true 
#define isUpdateHighlightingEnabled true
#define blinkOnTimeMs 250
#define blinkOffTimeMs 250
#define flashOnTimeMs 750		// flash does not repeat so it wont need off time
//  UserInput Parameters
#define ignoredInputEvents (FOCUS_EVENT | MENU_EVENT | MOUSE_EVENT)

typedef struct RECTANGLE {
	unsigned int X, Y, W, H;
} RECTANGLE, REC;

typedef struct COORDINATES {
	unsigned int X, Y;
} COORDINATES, COR;

enum functionExitCode {
	continueProgram,
	exitProgram
};