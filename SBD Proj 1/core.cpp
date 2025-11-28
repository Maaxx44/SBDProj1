#include "ui-core-merged.h"

// ---- Functions called by UI ----
functionExitCode core::UICreateEmptyFile() {
	std::optional<unsigned int> userInput = this->userInterface.getUserInputUInt(" Enter number of records:");
	if (userInput == std::nullopt) return continueProgram; // If user canceled operation - dont proceed

	this->openedTape.clear();
	this->openedTape.setSize(userInput.value());
	this->sorter.addTapeToSort(&this->openedTape);
	this->updateTables();

	return continueProgram;
}
functionExitCode core::UICreateRandomFile() {
	std::optional<unsigned int> userInput = this->userInterface.getUserInputUInt(" Enter number of records:");
	if (userInput == std::nullopt) return continueProgram; // If user canceled operation - dont proceed

	this->openedTape.clear();
	this->openedTape = fileTape::getRandomFileTape(userInput.value());
	this->sorter.addTapeToSort(&this->openedTape);
	this->updateTables();

	return continueProgram;
}
functionExitCode core::UIOpenFile() {
	// Getting user input
	std::optional<std::string> userInput = this->userInterface.getUserInputString(" Enter path to file:");
	if (userInput == std::nullopt) return continueProgram; // If user canceled operation - dont proceed









	/*TODO*/
	return continueProgram;
}
functionExitCode core::UIClearFile() {
	this->openedTape.clear();
	this->sorter.clear();
	this->updateTables();
	return continueProgram;
}
functionExitCode core::UISortFile() {
	this->sorter.addTapeToSort(&this->openedTape);
	this->sorter.sortTapeFull();
	this->updateTables();
	return continueProgram;
}
functionExitCode core::UISortStep() {
	if (!this->sorter.isTapeLoaded())
		this->sorter.addTapeToSort(&this->openedTape);
	this->sorter.sortNextPart();
	this->updateTables();
	return continueProgram;
}
functionExitCode core::UIResetSorting() {
	this->sorter.resetSorting();
	this->updateTables();
	return continueProgram;
}
functionExitCode core::UIModifyFileLine(unsigned int selectedContent) {
	double userAngle = 0.0, userRadius = 0.0;
	std::optional<double> userInput;

	bool correctValueEntered = false;
	std::string message = " Enter angle:";

	// Getting angle
	while (!correctValueEntered) {
		userInput = this->userInterface.getUserInputDouble(message);

		if (userInput == std::nullopt)
			return continueProgram;
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
		userInput = this->userInterface.getUserInputDouble(message);

		if (userInput == std::nullopt)
			return continueProgram;
		else {
			correctValueEntered = true;
			userRadius = userInput.value();
		}
	}

	// Setting values
	this->openedTape.setRecord(selectedContent, record(userAngle, userRadius));
	this->updateTables();
	this->tFilePreview.getContentLineMod(selectedContent).startFlash();
	this->userInterface.changeSelectedTableContentSelection({ {selectedContent, true} });

	return continueProgram;
}

void core::updateFilePreviewTable() {
	// Preserve blinking and flashing states


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
void core::initTables() {
	// Creating tables
	this->tOptions = uiTable({ .X = 2, .Y = 1, .W = 25, .H = 13 }, "Options", { "Create empty file", "Create random file", "Open file", "Clear file", "Sort file", "Make sorting step", "Reset sorting" });
	this->tFilePreview = uiTable({ .X = 29, .Y = 1, .W = 45, .H = 35 }, "File Preview", {});
	this->tWorkFilePreview = uiTable({ .X = 76, .Y = 1, .W = 45, .H = 35 }, "Work File Preview", {}); // not editable
	this->tSortingMetadata = uiTable({ .X = 2, .Y = 15, .W = 25, .H = 21 }, "Sorting Data", {}); // not editable

	// Changing title select color to corresponding one
	this->tOptions.getTitleMod().cSelected = defaultHighLightColor;
	this->tFilePreview.getTitleMod().cSelected = tableLinesModInterA[1].cSelected;
	this->tWorkFilePreview.getTitleMod().cSelected = tableLinesModInterB[1].cSelected;
	this->tSortingMetadata.getTitleMod().cSelected = defaultHighLightColor;

	// Creating links
	this->tOptions.setTablePointers(uiTablePointer(&this->tSortingMetadata, &this->tSortingMetadata, &this->tWorkFilePreview, &this->tFilePreview));
	this->tFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tOptions, &this->tWorkFilePreview));
	this->tWorkFilePreview.setTablePointers(uiTablePointer(nullptr, nullptr, &this->tFilePreview, &this->tOptions));
	this->tSortingMetadata.setTablePointers(uiTablePointer(&this->tOptions, &this->tOptions, &this->tWorkFilePreview, &this->tFilePreview));

	// Setting functions 
	this->tOptions.setFunctionCallingParameters(true, false);
	this->tFilePreview.setFunctionCallingParameters(true, false);
	this->tWorkFilePreview.setFunctionCallingParameters(false, false);
	this->tSortingMetadata.setFunctionCallingParameters(false, false);

	// Init. UI
	this->userInterface.addTable(&this->tOptions);
	this->userInterface.addTable(&this->tFilePreview);
	this->userInterface.addTable(&this->tWorkFilePreview);
	this->userInterface.addTable(&this->tSortingMetadata);
}
// --------------------------------

functionExitCode core::callTableFunction(uiTable* selectedTable, unsigned int selectedContent) {
	// I need to think about better solution. For now it checks individually for each table
	if (!selectedTable->isFunctionCallingEnabled())
		return continueProgram;

	// Small shortcut, untill I figure out better solution
	if (selectedTable == &this->tOptions) {
		switch (selectedContent) {
			case 0: // "Create Empty File"
				return this->UICreateEmptyFile();
			case 1: // "Create Random File"
				return this->UICreateRandomFile();
			case 2: // "Open File"
				return this->UIOpenFile();
			case 3: // "Clear File"
				return this->UIClearFile();
			case 4: // "Sort file"
				return this->UISortFile();
			case 5: // "Sort step"
				return this->UISortStep();
			case 6: // "Reset sorting"
				return this->UIResetSorting();
		}
	}
	else if (selectedTable == &this->tFilePreview) {
		return UIModifyFileLine(selectedContent);
	}
	return continueProgram;
}
void core::updateTables() {
	this->updateFilePreviewTable();
	this->updateWorkFilePreviewTable();
	this->updateSortingDataPreviewTable();
}


core::core(): userInterface(*this) {
	this->initTables();
	this->userInterface.setCursor(&this->tOptions);

	while (this->userInterface.runFrame() == continueProgram) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(5)); // waiting just a little bit so we don't spam out PC to death
	}
}
core::~core() {}
