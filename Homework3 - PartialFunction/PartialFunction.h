#pragma once
#include "Pair.h"

class PartialFunction {
public:
	virtual bool isDefinedAt(int) const = 0;
	virtual int operator()(int) const = 0;

	virtual PartialFunction* clone() const = 0;

	virtual ~PartialFunction() = default;
};