#pragma once
#include "common.h"

struct textModifiers {
	// Booleans for constant color
	bool isTextVisible = true;
	bool isTextHighlighted = false;
	bool isTextSelected = false;

	// Booleans for flashing lights
	bool isTextFlashing = false;
	bool isTextBlinking = false;

	// Colors
	WORD cDefault = defaultTextColor;
	WORD cHighlighted = defaultHighLightColor;
	WORD cSelected = defaultSelectColor;
	WORD cSelectedHighlighted = defaultSelectHighlightColor;
	WORD cFlashing = defaultSelectColor; // TODO - change later
	WORD cBlinking = defaultHighLightColor; // TODO - change later

	// Timing data
	bool tIsTextBlinkOn = false; // is the text flashed right now
	std::chrono::time_point<std::chrono::system_clock> tFlashStart = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> tBlinkDiff = std::chrono::system_clock::now();

	// Get text modifier - with dynamic lighting and advance the timing
	WORD getTextModifier(std::chrono::time_point<std::chrono::system_clock> newTS);
};

