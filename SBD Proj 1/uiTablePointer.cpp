#include "uiTablePointer.h"

uiTablePointer::uiTablePointer() {
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;
}

uiTablePointer::uiTablePointer(uiTable* up, uiTable* down, uiTable* left, uiTable* right) {
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
}
