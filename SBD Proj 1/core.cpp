#include "core.h"
#include <thread>
#include <chrono>


core::core() {
	while (true) {
		this->userInterface.draw();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
core::~core() {}
