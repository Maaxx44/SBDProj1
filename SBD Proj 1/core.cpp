#include "core.h"
#include <thread>
#include <chrono>


core::core() {
	while (this->userInterface.runFrame()) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
core::~core() {}
