#pragma once
#include "common.h"
#include "uiTable.h"

struct tableMetadata {
	uiTable table;
	// Main options(s)
	bool isTableVisible = true;
	
	// Styling options
	//  Title options
	bool isTitleHighlighted = false;
	bool isTitleSelected = false;
	//  Content options
	bool isContentSelected = false;
	bool isContentHighlighted = false;
	//  Indexes data
	unsigned int contentSelectedIndex = 0; // what item from ALL content is selected
	unsigned int contentHighlightedIndex = 0;
	unsigned int contentOffset = 0;

	// Colors
	//  Title
	WORD titleColor = defaultTextColor;
	WORD titleSelectedColor = defaultSelectColor;
	WORD titleHighlightedColor = defaultHighLightColor;
	WORD titleSelectedHighlightedColor = defaultSelectHighlightColor;
	//  Content
	WORD contentColor = defaultTextColor;
	WORD contentSelectedColor = defaultSelectColor;
	WORD contentHighlightedColor = defaultHighLightColor;
	WORD contentSelectedHighlightedColor = defaultSelectHighlightColor;

	// Timing data
	std::chrono::time_point<std::chrono::system_clock> tUpdateDiff; // timestamp of the start of update highlight flash for title
	std::chrono::time_point<std::chrono::system_clock> tSelectionDiff; // timestamp of the start of selection highlight (on/off, depends on isTitleHighlighted) for title
	std::chrono::time_point<std::chrono::system_clock> cUpdateDiff; // timestamp of the start of update highlight flash for content
	std::chrono::time_point<std::chrono::system_clock> cSelectionDiff; // timestamp of the start of selection highlight (on/off, depends on isTitleHighlighted) for content


	std::optional<unsigned int> getSelectedContentIndex() const; // calculates line number for selected item (or none!)
	std::optional<unsigned int> getHighlightedContentIndex() const; // calculates line number for selected item (or none!)
};

