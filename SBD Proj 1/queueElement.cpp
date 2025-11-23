#include "queueElement.h"

bool queueElement::operator() (queueElement& A, queueElement& B) {
	return compareRecordsRef(A.rData, B.rData);
}

// I hope this is the'right direction'
bool queueElement::operator< (const queueElement& B) const {
	return this->rData.calculateArea() > B.rData.calculateArea();
}
