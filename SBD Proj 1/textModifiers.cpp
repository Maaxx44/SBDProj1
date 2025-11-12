#include "textModifiers.h"

WORD textModifiers::getTextModifier(std::chrono::time_point<std::chrono::system_clock> newTS) {
	if (!isTextVisible) return 0x00; // TODO - CHANGE LATER !!!!!!!

	// TODO - LOADS OF LOGIC
	// 1. check fo static modifiers (selected/highlighted) - they can be combined together
	// 2. check for dynamic modifiers - they override static modifiers and the update overrides blink:
	//		Static modifiers -> Blinking -> Update flash

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

	// 2. Checking for blinking
	if (isTextBlinking) {
		// TODO
	}

	// 2. Checking for blinking
	if (isTextFlashing) {
		// TODO
	}

	return currentColor;
}