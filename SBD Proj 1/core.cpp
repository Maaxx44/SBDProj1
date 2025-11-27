#include "ui-core-merged.h"

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
functionExitCode core::UICreateEmptyFile(std::vector<fParUnion> funcParameters) {  /*TODO*/  return continueProgram; }
functionExitCode core::UICreateRandomFile(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UIOpenFile(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UIClearFile(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UISortFile(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UISortStep(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UIResetSorting(std::vector<fParUnion> funcParameters) {  /*TODO*/ return continueProgram; }
functionExitCode core::UIModifyFileLine(std::vector<fParUnion> funcParameters) {  /*TODO*/  return continueProgram; }

void core::updateFilePreviewTable() {
	// Required fields for table
	std::vector<std::string> parsedFile;
	std::vector<textModifiers> parsedFileMod;

	// For every content in opened file - get and parse its values into string
	for (unsigned int i = 0; i < this->openedTape.getSize(); i++) {
		record nextRecord = this->openedTape.getRecord(i);

		// Get values and parse them into stringstream (we cau use setw)
		std::stringstream ss;
		ss << " A: " << std::setw(10) << std::to_string(nextRecord.getAngle()) << ", R: " << std::setw(10) << std::to_string(nextRecord.getRadius()) << " = " << std::setw(11) << std::to_string(nextRecord.calculateArea());

		// Push back values into vectors
		parsedFile.push_back(ss.str());
		parsedFileMod.push_back(tableLinesModInterA[i % 2]);
	}

	// Setting tables content
	this->tFilePreview.setContent(parsedFile);
	this->tFilePreview.setContentMod(parsedFileMod);

	// Adjusting file readn and write counter, because we use those operations to display data
	this->openedTape.setReadOperations(this->openedTape.getReadOperations() - this->openedTape.getSize());

}
void core::updateWorkFilePreviewTable() {
	std::vector<std::string> parsedFile;
	std::vector<textModifiers> parsedFileMod;
	for (unsigned int i = 0; i < this->sorter.getWorkTapeP().getSize(); i++) {
		record nextRecord = this->sorter.getWorkTapeP().getRecord(i);
		this->sorter.getWorkTapeP().setReadOperations(this->sorter.getWorkTapeP().getReadOperations() - 1); // adjusting for dispalying
		std::stringstream ss;
		ss << " A: " << std::setw(10) << std::to_string(nextRecord.getAngle()) << ", R: " << std::setw(10) << std::to_string(nextRecord.getRadius()) << " = " << std::setw(11) << std::to_string(nextRecord.calculateArea());
		parsedFile.push_back(ss.str());
		parsedFileMod.push_back(tableLinesModInterB[i % 2]);
	}
	this->tWorkFilePreview.setContent(parsedFile);
	this->tWorkFilePreview.setContentMod(parsedFileMod);
}
void core::updateSortingDataPreviewTable() {
	std::pair<unsigned int, unsigned int> IOOperations = this->sorter.getIOperationsCount();
	unsigned int runsCount = this->sorter.getRunsCount();
	unsigned int runSize = this->sorter.getRunSize();
	std::vector<std::string> sortingMetadata;
	sortingMetadata.push_back("Reads: " + std::to_string(IOOperations.first));
	sortingMetadata.push_back("Writes: " + std::to_string(IOOperations.second));
	sortingMetadata.push_back("Run size: " + std::to_string(runSize));
	sortingMetadata.push_back("Runs: " + std::to_string(runsCount));
	this->tSortingMetadata.setContent(sortingMetadata);
}
void core::updateTables() {
	this->updateFilePreviewTable();
	this->updateWorkFilePreviewTable();
	this->updateSortingDataPreviewTable();
}
// --------------------------------

void core::initTables() {
	// Creating functions references
	std::vector<std::function<functionExitCode(std::vector<fParUnion>)>> fTOptions = {
		std::bind_front(&core::UICreateEmptyFile, this),
		std::bind_front(&core::UICreateRandomFile, this),
		std::bind_front(&core::UIOpenFile, this),
		std::bind_front(&core::UIClearFile, this),
		std::bind_front(&core::UISortFile, this),
		std::bind_front(&core::UISortStep, this),
		std::bind_front(&core::UIResetSorting, this)
	};
	std::function<functionExitCode(std::vector<fParUnion>)> fTFilePreview = std::bind_front(&core::UIModifyFileLine, this);

	// Creating tables
	this->tOptions = uiTable({ .X = 2, .Y = 1, .W = 25, .H = 13 }, "Options", { "Create empty file", "Create random file", "Open file", "Clear file", "Sort file", "Make sorting step", "Reset sorting" }, fTOptions);
	this->tFilePreview = uiTable({ .X = 29, .Y = 1, .W = 45, .H = 35 }, "File Preview", {}, fTFilePreview);
	//this->tOptions = uiTable({ .X = 2, .Y = 1, .W = 25, .H = 13 }, "Options", { "Create empty file", "Create random file", "Open file", "Clear file", "Sort file", "Make sorting step", "Reset sorting" }); // TODO - ADD FUNCTIONS
	//this->tFilePreview = uiTable({ .X = 29, .Y = 1, .W = 45, .H = 35 }, "File Preview", {}); // TODO - ADD FUNCTIONS
	this->tWorkFilePreview = uiTable({ .X = 76, .Y = 1, .W = 45, .H = 35 }, "Work File Preview", {}); // not editable
	this->tSortingMetadata = uiTable({ .X = 2, .Y = 15, .W = 25, .H = 21 }, "Sorting Data", {}); // not editable

	// Changing title select color to corresponding one
	this->tFilePreview.getTitleMod().cSelected = tableLinesModInterA[1].cSelected;
	this->tWorkFilePreview.getTitleMod().cSelected = tableLinesModInterB[1].cSelected;

	// Creating links
	this->tOptions.setTablePointers(uiTablePointer(&this->tSortingMetadata, &this->tSortingMetadata, &this->tWorkFilePreview, &this->tFilePreview));
	this->tFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tOptions, &this->tWorkFilePreview));
	this->tWorkFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tFilePreview, &this->tOptions));
	this->tSortingMetadata.setTablePointers(uiTablePointer(&this->tOptions, &this->tOptions, &this->tWorkFilePreview, &this->tFilePreview));


	// Init. UI
	this->userInterface.addTable(&this->tOptions);
	this->userInterface.addTable(&this->tFilePreview);
	this->userInterface.addTable(&this->tWorkFilePreview);
	this->userInterface.addTable(&this->tSortingMetadata);
}

core::core() {
	this->userInterface.setAppCore(this, (&core::updateTables));

	this->initTables();

	//this->updateTables();

	while (this->userInterface.runFrame() == continueProgram) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // waiting just a little bit so we don't spam out PC to death
	}
}
core::~core() {}
