#pragma once
#include "common.h"
#include "textModifiers.h"
#include "uiTablePointer.h"
#include "functionParameterUnion.h"



class uiTable {
private:
	// Relative position of other tables
	uiTablePointer tablePointers;

	// Text fields
	std::string titleField;
	std::vector<std::string> contentField;

	// Text fields styles
	textModifiers titleMod;
	std::vector<textModifiers> contentMod;

	// Other options
	REC position;
	bool isTableVisible = true;
	unsigned int contentOffset = 0;
	
	// Functions for interaction with program
	bool isGlobalFunctionActive = false; // if true then any interaction with content will call this function with else - every line will have individual function called
	std::function<fParUnion(std::vector<fParUnion>)> groupContentFunction;
	std::vector<std::function<fParUnion(std::vector<fParUnion>)>> contentFunctions;


	std::string cutoffString(std::string str, unsigned int length, unsigned int cutoffLength) const;

public:
	uiTable();
	uiTable(REC newPosition);
	uiTable(REC newPosition, std::string title);
	uiTable(REC newPosition, std::string title, std::vector<std::string> content);
	uiTable(REC newPosition, std::string title, textModifiers titleMod, std::vector<std::string> content, std::vector<textModifiers> contentMod);
	uiTable(REC newPosition, std::string title, textModifiers titleMod, std::vector<std::string> content, std::vector<textModifiers> contentMod, std::function<fParUnion(std::vector<fParUnion>)> groupContentFunc);
	uiTable(REC newPosition, std::string title, textModifiers titleMod, std::vector<std::string> content, std::vector<textModifiers> contentMod, std::vector<std::function<fParUnion(std::vector<fParUnion>)>> contentFunc);

	// ---- SETTERS ----
	/// Setting text
	void setTitle(std::string newTitle);
	void setContent(std::vector<std::string> newContent);
	void setContentPart(std::string newContentPart, unsigned int contentLine);

	/// Setting styling
	void setTitleMod(textModifiers newTitleMod);
	void setContentMod(std::vector<textModifiers> newContentMod);
	void setContentPartMod(textModifiers newContentMod, unsigned int contentLine);

	/// Setting offset & visibility
	void setContentOffset(unsigned int newContentOffset);
	void setTableVisible(bool isTableVisible);
	void setTableSize(REC newPosition);

	/// Setting pointers
	void setTablePointers(uiTablePointer newTablePointers);

	// -----------------


	// ---- GETTERS ----
	/// For rendering text
	std::string getTitle() const;
	std::vector<std::string> getFullContent() const; // returns full content, with all lines, but cuts them off
	std::vector<std::string> getContent() const; // returns content with lines fitting content height
	std::string getContentLine(unsigned int n) const;
	
	/// For rendering text style
	textModifiers& getTitleMod();
	std::vector<textModifiers>& getFullContentMod(); // just returns all contentmods
	std::vector<textModifiers> getContentMod(); // same as getContent()
	textModifiers& getContentLineMod(unsigned int n);

	/// For getting positions of rendered text
	REC getTitleRectangle() const;
	REC getContentRectangle() const;

	/// For getting position of the whole table
	REC getTableRectangle() const;

	/// This function gets size of the WHOLE content
	unsigned int getContentHeight() const;
	unsigned int getContentSize() const;
	unsigned int getContentOffset() const;
	bool getTableVisible() const;

	/// Getting pointers
	uiTablePointer& getTablePointers();
	// -----------------


	// ---- Unique logic functions ----
	/// Upadtes timestamps for each text element to allow dynamic styling and color changes
	void updateTiming();

	/// This function sets W & H parameters of position so there wont be any text cutoff
	void setPositionToText();

	/// This function checks if given line is visible ( is not cut off )
	bool isContentLineVisible(unsigned int lineIndex) const;

	/// This function calculates offset needed to make content line visible
	unsigned int getOffsetForLine(unsigned int lineIndex) const;
};

