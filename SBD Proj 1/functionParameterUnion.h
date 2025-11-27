#pragma once

enum functionExitCode {
	continueProgram,
	exitProgram
};

union returnData {
	unsigned int rUInt;
	double rDouble;
	bool rBool;
};
enum returnType {
	tUInt,
	tDouble,
	tBool,
	tNone
};

struct functionParameterUnion {
	returnData pData;
	returnType pType;

	functionParameterUnion();
	functionParameterUnion(returnType newReturnType);
	functionParameterUnion(returnType newReturnType, returnData newReturnData);
};

typedef functionParameterUnion fParUnion;

