#pragma once
#include "common.h"
#include "textModifiers.h"



class uiTable {
private:
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

	std::string cutoffString(std::string str, unsigned int length, unsigned int cutoffLength) const;

public:
	uiTable();
	uiTable(REC newPosition);
	uiTable(REC newPosition, std::string title);
	uiTable(REC newPosition, std::string title, std::vector<std::string> content);
	uiTable(REC newPosition, std::string title, textModifiers titleMod, std::vector<std::string> content, std::vector<textModifiers> contentMod);

	// For setting table size and position
	void resizeTable(REC newPosition);

	// For setting table title and content
	void setTitle(std::string newTitle);
	void setContent(std::vector<std::string> newContent);
	void setContentPart(std::string newContentPart, unsigned int contentLine);

	// For setting table styling
	void setTitleMod(textModifiers newTitleMod);
	void setContentMod(std::vector<textModifiers> newContentMod);
	void setContentPartMod(textModifiers newContentMod, unsigned int contentLine);

	// For setting content offset
	void setContentOffset(unsigned int newContentOffset);
	unsigned int getContentOffset() const;

	// For setting table visibility (duh)
	void setTableVisible(bool isTableVisible);
	bool getTableVisible() const;

	void updateTiming();


	// This function sets W & H parameters of position so there wont be any text cutoff
	void setPositionToText();

	// This function gets size of the WHOLE content
	unsigned int getContentHeight() const;

	// For rendering text
	std::string getTitle() const;
	std::vector<std::string> getFullContent() const; // returns full content, with all lines, but cuts them off
	std::vector<std::string> getContent() const; // returns content with lines fitting content height
	std::string getContentLine(unsigned int n) const;

	// For rendering text style
	textModifiers& getTitleMod();
	std::vector<textModifiers>& getFullContentMod(); // just returns all contentmods
	std::vector<textModifiers> getContentMod(); // same as getContent()
	textModifiers& getContentLineMod(unsigned int n);

	// For getting positions of rendered text
	REC getTitleRectangle() const;
	REC getContentRectangle() const;
};

