#include "tableMetadata.h"

std::optional<unsigned int> tableMetadata::getSelectedContentIndex() const {
	if (!this->isContentSelected) return std::nullopt;
	
	unsigned int selectedIndex = this->contentSelectedIndex % (unsigned int)this->table.getFullContent().size();
	unsigned int minSelectedIndexIndex = this->contentOffset;
	unsigned int maxSelectedIndexIndex = (this->contentOffset + this->table.getContentHeight() - 1) % (unsigned int)this->table.getFullContent().size();

	if (minSelectedIndexIndex <= maxSelectedIndexIndex) {
		// Content window does not wrap around
		if (selectedIndex >= minSelectedIndexIndex && selectedIndex <= maxSelectedIndexIndex)
			return selectedIndex - this->contentOffset;
	}
	else {
		// Content window wraps around
		if(selectedIndex >= minSelectedIndexIndex) return selectedIndex - this->contentOffset;
		else if(selectedIndex <= maxSelectedIndexIndex) return selectedIndex + (unsigned int)this->table.getFullContent().size() - this->contentOffset;
	}

	//if (selectedItem >= this->table.getContentHeight() + this->contentOffset) return std::nullopt;
	//else return std::optional<unsigned int>(selectedItem - this->contentOffset);
}
std::optional<unsigned int> tableMetadata::getHighlightedContentIndex() const {
	if (!this->isContentHighlighted) return std::nullopt;

	unsigned int HighlightedIndex = this->contentHighlightedIndex % (unsigned int)this->table.getFullContent().size();
	unsigned int minHighlightedIndexIndex = this->contentOffset;
	unsigned int maxHighlightedIndexIndex = (this->contentOffset + this->table.getContentHeight() - 1) % (unsigned int)this->table.getFullContent().size();

	if (minHighlightedIndexIndex <= maxHighlightedIndexIndex) {
		// Content window does not wrap around
		if (HighlightedIndex >= minHighlightedIndexIndex && HighlightedIndex <= maxHighlightedIndexIndex)
			return HighlightedIndex - this->contentOffset;
	}
	else {
		// Content window wraps around
		if (HighlightedIndex >= minHighlightedIndexIndex) return HighlightedIndex - this->contentOffset;
		else if (HighlightedIndex <= maxHighlightedIndexIndex) return HighlightedIndex + (unsigned int)this->table.getFullContent().size() - this->contentOffset;
	}



	//unsigned int selectedItem = this->contentHighlightedIndex % (unsigned int)this->table.getFullContent().size();
	//if (selectedItem >= this->table.getContentHeight() + this->contentOffset) return std::nullopt;
	//else return std::optional<unsigned int>(selectedItem - this->contentOffset);
}
