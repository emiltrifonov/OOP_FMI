#pragma once
#include "FunctionExtremum.h"
#include "FunctionContainer.h"

class FunctionMaximum : public FunctionExtremum {
public:
	FunctionMaximum(const FunctionContainer&);
	FunctionMaximum(FunctionContainer&&);

	int operator()(int) const override final;

	PartialFunction* clone() const override final;
};