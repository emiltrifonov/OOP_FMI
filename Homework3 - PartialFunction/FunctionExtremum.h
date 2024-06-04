#pragma once
#include "PartialFunction.h"
#include "FunctionContainer.h"

class FunctionExtremum : public PartialFunction {
public:
	bool isDefinedAt(int) const override final;
	int operator()(int) const = 0;

	PartialFunction* clone() const = 0;

protected:
	FunctionContainer functions;
	int calculateExtremum(int arg, bool(*pred)(int, int)) const;
};