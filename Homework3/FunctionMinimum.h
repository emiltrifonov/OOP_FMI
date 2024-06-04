#pragma once
#include "FunctionExtremum.h"
#include "FunctionContainer.h"

class FunctionMinimum : public FunctionExtremum {
public:
	FunctionMinimum(const FunctionContainer&);
	FunctionMinimum(FunctionContainer&&);

	int operator()(int) const override final;

	PartialFunction* clone() const override final;
};