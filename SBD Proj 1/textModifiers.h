#pragma once
#include "common.h"

struct textModifiers {
private:
	// Booleans for flashing lights
	bool isTextFlashing = false;
	bool isTextBlinking = false;

	// Timing data
	std::chrono::time_point<std::chrono::steady_clock> tFlashStart = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> tBlinkStart = std::chrono::steady_clock::now();

public:
	// Booleans for constant color
	bool isTextVisible = true;
	bool isTextHighlighted = false;
	bool isTextSelected = false;

	// Colors
	WORD cDefault = defaultTextColor;
	WORD cHighlighted = defaultHighLightColor;
	WORD cSelected = defaultSelectColor;
	WORD cSelectedHighlighted = defaultSelectHighlightColor;
	WORD cFlashing = defaultFlashColor;
	WORD cBlinking = defaultBlinkColor;

	// Get text modifier - with dynamic lighting and advance the timing
	WORD getTextModifier() const;

	void startBlink();
	void startFlash();

	void updateTiming();
};

