#pragma once
#include "common.h"

#include <Windows.h>


class uiTable {
private:
	std::string titleField;
	std::vector<std::string> contentField;
	REC position;

	std::string cutoffString(std::string str, unsigned int length, unsigned int cutoffLength) const;

public:
	WORD titleColor = defaultTextColor;
	WORD contentColor = defaultTextColor;
	WORD highlightColor = defaultHighLightColor;
	WORD selectColor = defaultSelectColor;

	uiTable();
	uiTable(REC newPosition);
	uiTable(REC newPosition, std::string title);
	uiTable(REC newPosition, std::string title, std::vector<std::string> content);

	void resizeTable(REC newPosition);
	void setTitle(std::string newTitle);
	void setContent(std::vector<std::string> newContent);
	void setContentPart(std::string newContentPart, unsigned int contentLine);


	// This function sets W & H parameters of position so there wont be any text cutoff
	void setPositionToText();

	unsigned int getContentHeight() const;

	std::string getTitle() const;
	std::vector<std::string> getFullContent() const; // returns full content, with all lines
	std::vector<std::string> getContent(unsigned int scroll) const; // returns content with lines fitting content height
	std::string getContentLine(unsigned int n) const;

	REC getTitleRectangle() const;
	REC getContentRectangle() const;
};

