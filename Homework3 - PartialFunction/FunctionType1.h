#pragma once
#include "FunctionFromFile.h"
#define st size_t

class FunctionType1 : public FunctionFromFile {
public:
	FunctionType1() = default;
	FunctionType1(const int* args, st len); // not final version

	bool isDefinedAt(int) const override final;
	Pair operator()(int) const override final;
};
