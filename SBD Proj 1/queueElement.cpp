#include "queueElement.h"

bool queueElement::operator() (queueElement& A, queueElement& B) {
	return compareRecordsRef(A.rData, B.rData);
}