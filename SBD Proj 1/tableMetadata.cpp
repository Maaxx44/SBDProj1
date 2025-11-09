#include "tableMetadata.h"

std::optional<unsigned int> tableMetadata::getSelectedContentIndex() const {
	if (!this->isContentSelected) return std::nullopt;
	
	unsigned int selectedItem = this->contentSelectedIndex % (unsigned int)this->table.getFullContent().size();
	if (selectedItem >= this->table.getContentHeight()) return std::nullopt;
	else return std::optional<unsigned int>(selectedItem - this->contentOffset);
}
std::optional<unsigned int> tableMetadata::getHighlightedContentIndex() const {
	if (!this->isContentHighlighted) return std::nullopt;

	unsigned int selectedItem = this->contentHighlightedIndex % (unsigned int)this->table.getFullContent().size();
	if (selectedItem >= this->table.getContentHeight()) return std::nullopt;
	else return std::optional<unsigned int>(selectedItem - this->contentOffset);
}
