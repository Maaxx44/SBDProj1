#include "textModifiers.h"

WORD textModifiers::getTextModifier() const {
	// welp - i need to think about it
	if (!isTextVisible) {
		if (returnNoColorOnInvisible)
			return 0x00;
		else
			return cDefault;
	}
	

	// 1. Checking for static modifiers
	WORD currentColor = 0x00;
	switch (isTextHighlighted ? 1 : 0 + isTextSelected ? 2 : 0) {
	case 0: // text is normal
		currentColor = cDefault;
		break;
	case 1: // text is highlighted
		currentColor = cHighlighted;
		break;
	case 2: // text is selected
		currentColor = cSelected;
		break;
	case 3: // text is both highlighted and slected
		currentColor = cSelectedHighlighted;
		break;
	}

	// 2. Calculating time delta
	// When calculating blinkDiff - divide wholee delta by sum of on and off time, so the logic for handling blinking wont need to account for repeat blinks
	// Checking if blinking cycle is beeing repeated and if so - advance it
	std::chrono::time_point<std::chrono::steady_clock> currentTS = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> fullBlinkDiff = std::chrono::duration<double, std::milli>(currentTS - this->tBlinkStart);
	std::chrono::duration<double, std::milli> tBlinkCycle = std::chrono::duration<double, std::milli>(std::chrono::duration<double, std::milli>(blinkOnTimeMs) + std::chrono::duration<double, std::milli>(blinkOffTimeMs));
	std::chrono::time_point<std::chrono::steady_clock> tOptimisedBlinkStart = this->tBlinkStart;
	if (fullBlinkDiff >= tBlinkCycle) {
		// Convertiong std::chrono::duration to double
		double	dFullBlinkDiff = fullBlinkDiff.count(),
				dtBlinkCycle = tBlinkCycle.count();

		// Counting how many cycles of blinking occured
		unsigned int blinkCyclesCount = (unsigned int)std::floor(dFullBlinkDiff / dtBlinkCycle);
		
		// Advancing blink start by cycles count
		tOptimisedBlinkStart += std::chrono::milliseconds( (unsigned int)((double)dtBlinkCycle * (double)blinkCyclesCount) );
	}

	// 3. Checking for blinking
	if (isTextBlinking && std::chrono::duration<double, std::milli>(currentTS - tOptimisedBlinkStart) <= std::chrono::duration<double, std::milli>(blinkOnTimeMs))
		currentColor = cBlinking;

	// 4. Checking for flashing
	if (isTextFlashing && std::chrono::duration<double, std::milli>(currentTS - this->tFlashStart) <= std::chrono::duration<double, std::milli>(flashOnTimeMs))
		currentColor = cFlashing;

	return currentColor;
}

void textModifiers::startBlink() {
	this->tBlinkStart = std::chrono::steady_clock::now();
	this->isTextBlinking = true;
}
void textModifiers::startFlash() {
	this->tFlashStart = std::chrono::steady_clock::now();
	this->isTextFlashing = true;
}

void textModifiers::updateTiming() {
	std::chrono::time_point<std::chrono::steady_clock> currentTS = std::chrono::steady_clock::now();

	// Updating flashing
	if (isTextFlashing && std::chrono::duration<double, std::milli>(currentTS - this->tFlashStart) > std::chrono::duration<double, std::milli>(flashOnTimeMs))
		this->isTextFlashing = false;

	// Updating blinking
	std::chrono::duration<double, std::milli> fullBlinkDiff = std::chrono::duration<double, std::milli>(currentTS - this->tBlinkStart);
	std::chrono::duration<double, std::milli> tBlinkCycle = std::chrono::duration<double, std::milli>(std::chrono::duration<double, std::milli>(blinkOnTimeMs) + std::chrono::duration<double, std::milli>(blinkOffTimeMs));
	if (fullBlinkDiff >= tBlinkCycle) // Advancing tBlinkStart by floor of times of cycles
		this->tBlinkStart += std::chrono::milliseconds((unsigned int)((double)tBlinkCycle.count() * (double)std::floor(fullBlinkDiff.count() / tBlinkCycle.count())));
}