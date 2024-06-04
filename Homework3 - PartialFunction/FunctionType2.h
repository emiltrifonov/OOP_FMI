#pragma once
#include "FunctionFromFile.h"
#define st size_t

class FunctionType2 : public FunctionFromFile {
public:
	FunctionType2() = default;
	FunctionType2(const int* args, st len); // not final version

	bool isDefinedAt(int) const override final;
	Pair operator()(int) const override final;
};