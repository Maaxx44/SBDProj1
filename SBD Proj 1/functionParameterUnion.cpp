#include "functionParameterUnion.h"

functionParameterUnion::functionParameterUnion() {
	this->pData = returnData(0);
	this->pType = returnType::tNone;
}
functionParameterUnion::functionParameterUnion(returnType newReturnType) {
	this->pData = returnData(0);
	this->pType = newReturnType;
}
functionParameterUnion::functionParameterUnion(returnType newReturnType, returnData newReturnData) {
	this->pData = newReturnData;
	this->pType = newReturnType;
}