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

	// TODO - timing data

	std::optional<unsigned int> getSelectedContentIndex() const; // calculates line number for selected item (or none!)
	std::optional<unsigned int> getHighlightedContentIndex() const; // calculates line number for selected item (or none!)
};

