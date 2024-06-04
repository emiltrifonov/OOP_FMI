#pragma once
#include "FunctionFromFile.h"
#define st size_t

class FunctionType0 : public FunctionFromFile {
public:
	FunctionType0() = default;
	FunctionType0(const int* args, const int* values, st len); // not final version

	bool isDefinedAt(int) const override final;
	Pair operator()(int) const override final;

private:
	int values[MAX_SIZE]{ 0 };
};