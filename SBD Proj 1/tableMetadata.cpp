#include "tableMetadata.h"

std::optional<unsigned int> tableMetadata::getSelectedContentIndex() const {
	return (this->contentSelectedIndex % this->table.getFullContent().size() <= this->table.getContentHeight()) ? std::optional<unsigned int>{(this->contentSelectedIndex% this->table.getFullContent().size())} : std::nullopt; // ouch
}
