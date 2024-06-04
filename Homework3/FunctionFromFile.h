#pragma once
#include "Pair.h"
#define st size_t

class FunctionFromFile {
public:
	virtual Pair operator()(int) const = 0;
	virtual bool isDefinedAt(int) const = 0;

protected:
	const static int MAX_SIZE = 32; // N which <= 32
	int arguments[MAX_SIZE]{ 0 };
	st len = 0;
};
