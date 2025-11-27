#include "uiTable.h"

std::string uiTable::cutoffString(std::string str, unsigned int length, unsigned int cutoffLength) const {
	if (str.size() < length) {
		str.append(std::string(length - str.size(), ' '));
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
uiTable::uiTable(REC newPosition, std::string title, std::vector<std::string> content) : position(newPosition), titleField(title), contentField(content) { this->contentMod.resize(contentField.size()); }
uiTable::uiTable(REC newPosition, std::string title, textModifiers titleMod, std::vector<std::string> content, std::vector<textModifiers> contentMod) : position(newPosition), titleField(title), titleMod(titleMod), contentField(content), contentMod(contentMod) {}



// ---- SETTERS ----
void uiTable::setTitle(std::string newTitle) {
	this->titleField = newTitle;
	this->titleMod = textModifiers();
}
void uiTable::setContent(std::vector<std::string> newContent) {
	this->contentField = newContent;
	this->contentMod.clear();
	this->contentMod.resize(contentField.size());
}
void uiTable::setContentPart(std::string newContentPart, unsigned int contentLine) {
	if (contentLine >= this->contentField.size())
		throw std::runtime_error("setContentPart error: n(" + std::to_string(contentLine) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	this->contentField[contentLine] = newContentPart;
	this->contentMod[contentLine] = textModifiers();
}

void uiTable::setTitleMod(textModifiers newTitleMod) {
	this->titleMod = newTitleMod;
}
void uiTable::setContentMod(std::vector<textModifiers> newContentMod) {
	if (newContentMod.size() != this->contentMod.size()) throw std::runtime_error("setContentMod error: newContentMod.size() was not equal to contentMod.size()!");
	this->contentMod = newContentMod;
}
void uiTable::setContentPartMod(textModifiers newContentMod, unsigned int contentLine) {
	if (contentLine >= this->contentField.size())
		throw std::runtime_error("setContentPartMod error: n(" + std::to_string(contentLine) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	this->contentMod[contentLine] = newContentMod;
}

void uiTable::setContentOffset(unsigned int newContentOffset) {
	this->contentOffset = newContentOffset % contentField.size();
}
void uiTable::setTableVisible(bool isTableVisible) {
	this->isTableVisible = isTableVisible;
}
void uiTable::setTableSize(REC newPosition) {
	this->position = newPosition;
}

void uiTable::setTablePointers(uiTablePointer newTablePointers) {
	this->tablePointers = newTablePointers;
}

void uiTable::setFunctionCallingParameters(bool isFunctionCallingEnabled, bool isGlobalFunctionEnabled) {
	this->isFunctionCallingActive = isFunctionCallingEnabled;
	this->isGlobalFunctionActive = isGlobalFunctionEnabled;
}
// -----------------

// ---- GETTERS ----
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
std::vector<std::string> uiTable::getContent() const {
	std::vector<std::string> fullContent = this->getFullContent(); // getting cut-off lines

	// 1. Scrolling lines
	unsigned int realScroll = (fullContent.size() > 0) ? this->contentOffset % fullContent.size() : 0;

	if(realScroll != 0) {
		// Copying block out of beg.
		std::vector<std::string>::const_iterator blockFirst = fullContent.begin();
		std::vector<std::string>::const_iterator blockLast = fullContent.begin() + realScroll;
		std::vector<std::string> block(blockFirst, blockLast);

		// Removing block from oryginal vector
		fullContent.erase(fullContent.begin(), fullContent.begin() + realScroll);

		// Appending block at the end
		fullContent.reserve(fullContent.size() + block.size());
		fullContent.insert(std::end(fullContent), std::begin(block), std::end(block)); // if performance suffers I could use std::move()
	}

	// 2. Add empty lines ro remove extra lines (to fit into context's height)
	if (fullContent.size() < this->getContentHeight()) {
		// Adding empty lines
		unsigned int sizeBeforeUpdating = (unsigned int)fullContent.size();
		for (unsigned int i = 0; i < this->getContentHeight() - sizeBeforeUpdating; i++)
			fullContent.push_back(std::string(this->position.W, ' '));
	}
	else if (fullContent.size() > this->getContentHeight()) {
		// Removing extra lines
		fullContent.erase(fullContent.begin() + this->getContentHeight(), fullContent.end());
	}

	return fullContent;
}
std::string uiTable::getContentLine(unsigned int n) const {
	if (n >= this->contentField.size())
		throw std::runtime_error("getContent error: n(" + std::to_string(n) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	return cutoffString(this->contentField[n], this->position.W, stringCutOffMin);
}

textModifiers& uiTable::getTitleMod() {
	return this->titleMod;
}
std::vector<textModifiers>& uiTable::getFullContentMod() {
	return this->contentMod;
}
std::vector<textModifiers> uiTable::getContentMod() {
	std::vector<textModifiers> fullContentMod = this->getFullContentMod();

	unsigned int realScroll = (fullContentMod.size() > 0) ? this->contentOffset % fullContentMod.size() : 0;

	if (realScroll != 0) {
		// Copying block out of beg.
		std::vector<textModifiers>::const_iterator blockFirst = fullContentMod.begin();
		std::vector<textModifiers>::const_iterator blockLast = fullContentMod.begin() + realScroll;
		std::vector<textModifiers> block(blockFirst, blockLast);

		// Removing block from oryginal vector
		fullContentMod.erase(fullContentMod.begin(), fullContentMod.begin() + realScroll);

		// Appending block at the end
		fullContentMod.reserve(fullContentMod.size() + block.size());
		fullContentMod.insert(std::end(fullContentMod), std::begin(block), std::end(block)); // if performance suffers I could use std::move()
	}

	// 2. Add empty lines ro remove extra lines (to fit into context's height)
	if (fullContentMod.size() < this->getContentHeight()) {
		textModifiers emptyMod;
		// Adding empty lines
		unsigned int sizeBeforeUpdating = (unsigned int)fullContentMod.size();
		for (unsigned int i = 0; i < this->getContentHeight() - sizeBeforeUpdating; i++)
			fullContentMod.push_back(emptyMod);
	}
	else if (fullContentMod.size() > this->getContentHeight()) {
		// Removing extra lines
		fullContentMod.erase(fullContentMod.begin() + this->getContentHeight(), fullContentMod.end());
	}

	return fullContentMod;
}
textModifiers& uiTable::getContentLineMod(unsigned int n) {
	if (n >= this->contentField.size())
		throw std::runtime_error("getContentLineMod error: n(" + std::to_string(n) + ") out of range(" + std::to_string(this->contentField.size()) + ")");
	return this->contentMod[n];
}

REC uiTable::getTitleRectangle() const {
	return { .X = this->position.X, .Y = this->position.Y, .W = this->position.W, .H = 1 };
}
REC uiTable::getContentRectangle() const {
	return { .X = this->position.X, .Y = this->position.Y + 1, .W = this->position.W, .H = this->position.H - 1 };
}

REC uiTable::getTableRectangle() const {
	return this->position;
}

unsigned int uiTable::getContentHeight() const {
	return this->position.H - 1;
}
unsigned int uiTable::getContentSize() const {
	return (unsigned int)this->contentField.size();
}
unsigned int uiTable::getContentOffset() const {
	return this->contentOffset;
}
bool uiTable::getTableVisible() const {
	return this->isTableVisible;
}

uiTablePointer& uiTable::getTablePointers() {
	return this->tablePointers;
}
// -----------------


void uiTable::updateTiming() {
	for (textModifiers& contentLineMod : this->contentMod)
		contentLineMod.updateTiming();
}
void uiTable::setPositionToText() {
	this->position.H = (int)this->contentField.size() + 1;

	unsigned int maxLength = (unsigned int)this->titleField.size();

	for (std::string s : this->contentField)
		maxLength = (maxLength < (unsigned int)s.size()) ? (unsigned int)s.size() : maxLength;

	this->position.W = maxLength;
}

/// This function checks if given line is visible ( is not cut off )
bool uiTable::isContentLineVisible(unsigned int lineIndex) const {
	if (this->contentOffset > 0 && lineIndex < this->contentOffset) return false;
	else if (this->contentOffset + this->getContentHeight() <= lineIndex) return false;
	return true;
}

/// This function calculates offset needed to make content line visible
unsigned int uiTable::getOffsetForLine(unsigned int lineIndex) const {
	if (this->contentOffset > 0 && lineIndex < this->contentOffset) return lineIndex;
	else if (this->contentOffset + this->getContentHeight() <= lineIndex) return lineIndex - this->getContentHeight() + 1;
	else return 0;
}

bool uiTable::isGlobalContentFunctionEnabled() const {
	return this->isGlobalFunctionActive;
}
bool uiTable::isFunctionCallingEnabled() const {
	return this->isFunctionCallingActive;
}