#include "FunctionMaximum.h"
#include <iostream>
#include <exception>

FunctionMaximum::FunctionMaximum(const FunctionContainer& fc)
{
	functions = fc;
}

FunctionMaximum::FunctionMaximum(FunctionContainer&& fc)
{
	functions = std::move(fc);
}

int FunctionMaximum::operator()(int arg) const
{
	return calculateExtremum(arg, [](int a, int b) { return a > b; });
}

PartialFunction* FunctionMaximum::clone() const
{
	return new FunctionMaximum(functions);
}
