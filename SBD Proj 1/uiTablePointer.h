#pragma once
#include "common.h"

class uiTable;

struct uiTablePointer {
	uiTable *up, *down, *left, *right; // raw pointer - OUCH!
};

