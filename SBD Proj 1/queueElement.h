#pragma once
#include "record.h"

struct queueElement {
	unsigned int rIndex; // index fo the run
	unsigned int rElement; // offset from the beg. of the run
	record rData; // out record

	// Used by queue to compare recprds
	bool operator() (queueElement& A, queueElement& B);
};
