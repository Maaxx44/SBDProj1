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

	void resizeTable(REC newPosition);
	void setTitle(std::string newTitle);
	void setContent(std::vector<std::string> newContent);
	void setContentPart(std::string newContentPart, unsigned int contentLine);

	void setTitleMod(textModifiers newTitleMod);
	void setContentMod(std::vector<textModifiers> newContentMod);
	void setContentPartMod(textModifiers newContentMod, unsigned int contentLine);

	void setContentOffset(unsigned int newContentOffset);
	unsigned int getContentOffset() const;

	void setTableVisible(bool isTableVisible);
	bool getTableVisible() const;




	// This function sets W & H parameters of position so there wont be any text cutoff
	void setPositionToText();

	// This function gets size of the WHOLE content
	unsigned int getContentHeight() const;

	std::string getTitle() const;
	std::vector<std::string> getFullContent() const; // returns full content, with all lines, but cuts them off
	std::vector<std::string> getContent() const; // returns content with lines fitting content height
	std::string getContentLine(unsigned int n) const;

	textModifiers getTitleMod() const;
	std::vector<textModifiers> getFullContentMod() const; // just returns all contentmods
	std::vector<textModifiers> getContentMod() const; // same as getContent()
	textModifiers getContentLineMod(unsigned int n) const;



	REC getTitleRectangle() const;
	REC getContentRectangle() const;
};

