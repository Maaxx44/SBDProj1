#pragma once
#include "common.h"
#include "uiTable.h"

struct tableMetadata {
	uiTable table;
	bool isTitleHighlighted = false;
	bool isTitleSelected = false;
	bool isContentSelected = false;
	unsigned int contentSelectedIndex = 0; // what item from ALL content is selected
	unsigned int contentOffset = 0; // TODO - LATER - presistent scrolling

	std::optional<unsigned int> getSelectedContentIndex() const; // calculates line number for selected item (or none!)
};

