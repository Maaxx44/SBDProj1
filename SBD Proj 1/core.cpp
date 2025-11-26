#include "core.h"

// old function to be broken up and translated to new UI*() functions
/*
void ui::executeUserInput() {
	if (this->cursorInfo.currentTableCursorPoint == &this->tOptions) { 
		// Initializing possible user input - to please the compiler
		std::optional<unsigned int> userInput(std::nullopt);
		switch (this->cursorInfo.currentContentCursorPoint) {
			//TODO - SET TO FINAL FUNCTIONS !!!!
		case 0: // "Empty File"
			userInput = getUserInputUInt(" Enter number of records:");
			if (userInput != std::nullopt) {
				// User did not cancel operation - create new empty file
				this->openedFile.clear();
				this->openedFile.setSize(userInput.value());
				this->sorter.addTapeToSort(&this->openedFile);
			}
			break;
		case 1: // "Random File"
			userInput = getUserInputUInt(" Enter number of records:");
			if (userInput != std::nullopt) {
				// User did not cancel operation - create new random file
				this->openedFile.clear();
				this->openedFile = fileTape::getRandomFileTape(userInput.value());
				this->sorter.addTapeToSort(&this->openedFile);
			}
			break;
		case 2: // "Open File"
			// TODO
			break;
		case 3: // "Clear File"
			this->openedFile.clear();
			this->sorter.clear();
			break;
		case 4: // "Sort file"
			this->sorter.addTapeToSort(&this->openedFile);
			this->sorter.sortTapeFull();
				break;
		case 5: // "Sort step"
			if(!this->sorter.isTapeLoaded())
				this->sorter.addTapeToSort(&this->openedFile);
			this->sorter.sortNextPart();
				break;
		case 6: // "Reset sorting"
			this->sorter.resetSorting();
			break;
		}
		this->updateTables();
	}
	else if (this->cursorInfo.currentTableCursorPoint == &this->tFilePreview) {
		this->cursorInfo.currentContentCursorPoint;

		double userAngle = 0.0, userRadius = 0.0;
		std::optional<double> userInput;

		bool correctValueEntered = false;
		std::string message = " Enter angle:";

		// Getting angle
		while (!correctValueEntered) {
			userInput = getUserInputDouble(message);

			if (userInput == std::nullopt)
				return;
			else if (userInput > 360.0)
				message = " Angle cannot be more than 360. Enter correct angle:";
			else {
				correctValueEntered = true;
				userAngle = userInput.value();
			}
		}

		// Getting radius
		correctValueEntered = false;
		message = " Enter radius:";
		while (!correctValueEntered) {
			userInput = getUserInputDouble(message);

			if (userInput == std::nullopt)
				return;
			else {
				correctValueEntered = true;
				userRadius = userInput.value();
			}
		}

		// Setting values
		this->openedFile.setRecord(this->cursorInfo.currentContentCursorPoint, record(userAngle, userRadius));
		this->updateTables();
		this->tFilePreview.getContentLineMod(this->cursorInfo.currentContentCursorPoint).startFlash();
		this->selectContent(this->cursorInfo.currentContentCursorPoint);
	}
}*/

// ---- Functions called by UI ----
fParUnion core::UICreateEmptyFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UICreateRandomFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UIOpenFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UIClearFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UISortFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UISortStep(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UIResetSorting(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
fParUnion core::UIModifyFileLine(std::vector<fParUnion> funcParameters) {  /*TODO*/  }
// --------------------------------


core::core(): userInterface(*this)  {
	while (this->userInterface.runFrame() == continueProgram) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // waiting just a little bit so we don't spam out PC to death
	}
}
core::~core() {}
