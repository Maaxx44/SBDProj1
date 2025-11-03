#include "uiTable.h"

std::string uiTable::cutoffString(std::string str, unsigned int length, unsigned int cutoffLength) const {
	if (str.size() < length) {
		str.append(std::string(length - str.size(), ' ')); // TODO - check if length is correct
		return str;
	}
	if(str.size() == length)
		return str;

	std::string retStr = str.substr(0, length);
	if (length > cutoffLength)
		retStr.replace(retStr.size() - 3, 3, 3, '.');

	return retStr;
}


uiTable::uiTable() : position({ 0, 0, 0, 0 }) {}
uiTable::uiTable(REC newPosition) : position(newPosition) { }
uiTable::uiTable(REC newPosition, std::string title) : position(newPosition), titleField(title) {}
uiTable::uiTable(REC newPosition, std::string title, std::vector<std::string> content) : position(newPosition), titleField(title), contentField(content) {}

void uiTable::resizeTable(REC newPosition) {
	this->position = newPosition;
}
void uiTable::setTitle(std::string newTitle) {
	this->titleField = newTitle;
}
void uiTable::setContent(std::vector<std::string> newContent) {
	this->contentField = newContent;
}
void uiTable::setContentPart(std::string newContentPart, unsigned int contentLine) {
	if (contentLine >= this->contentField.size())
		throw std::runtime_error("getContent error: n(" + std::to_string(contentLine) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	this->contentField[contentLine] = newContentPart;
}

void uiTable::setPositionToText() {
	this->position.H = this->contentField.size() + 1;

	unsigned int maxLength = this->titleField.size();

	for (std::string s : this->contentField)
		maxLength = (maxLength < s.size()) ? s.size() : maxLength;
	
	this->position.W = maxLength;
}

unsigned int uiTable::getContentHeight() const {
	return this->position.H - 1;
}


std::string uiTable::getTitle() const {
	return cutoffString(this->titleField, this->position.W, stringCutOffMin);
}
std::vector<std::string> uiTable::getFullContent() const {
	std::vector<std::string> retCont;
	for (unsigned int i = 0; i < this->contentField.size(); i++) {
		retCont.push_back(this->cutoffString(this->contentField[i], this->position.W, stringCutOffMin));
	}
	return retCont;
}
std::vector<std::string> uiTable::getContent(unsigned int scroll) const {
	std::vector<std::string> fullContent = this->getFullContent();
	unsigned int contentHeight = this->getContentHeight();

	// Adding empty lines to centent
	if (fullContent.size() < contentHeight) {
		for (unsigned int i = 0; i < contentHeight - fullContent.size(); i++)
			fullContent.push_back(std::string(this->position.W, ' '));
	}


	// TODO - SCROLL
	unsigned int realScroll = scroll % contentHeight;



}

std::string uiTable::getContentLine(unsigned int n) const {
	if (n >= this->contentField.size())
		throw std::runtime_error("getContent error: n(" + std::to_string(n) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	return cutoffString(this->contentField[n], this->position.W, stringCutOffMin);
}

REC uiTable::getTitleRectangle() const {
	return { .X = this->position.X, .Y = this->position.Y, .W = this->position.W, .H = 1 };
}
REC uiTable::getContentRectangle() const {
	return { .X = this->position.X, .Y = this->position.Y + 1, .W = this->position.W, .H = this->position.H - 1 };
}