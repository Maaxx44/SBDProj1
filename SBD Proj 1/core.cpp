#include "core.h"

core::core() {
	while (this->userInterface.runFrame() == continueProgram) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // waiting just a little bit so we don't spam out PC to death
	}
}
core::~core() {}
