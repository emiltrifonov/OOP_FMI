#include "FunctionMinimum.h"
#include <iostream>

FunctionMinimum::FunctionMinimum(const FunctionContainer& fc)
{
	functions = fc;
}

FunctionMinimum::FunctionMinimum(FunctionContainer&& fc)
{
	functions = std::move(fc);
}

int FunctionMinimum::operator()(int arg) const
{
	return calculateExtremum(arg, [](int a, int b) { return a < b; });
}

PartialFunction* FunctionMinimum::clone() const
{
	return new FunctionMinimum(functions);
}
