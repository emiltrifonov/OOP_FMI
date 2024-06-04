#pragma once
#include "PartialFunction.h"
#include "Pair.h"

//#define funcRes Pair<bool,int>

template <class T>
class PartialFunctionByCriteria : public PartialFunction {
public:
	PartialFunctionByCriteria(const T& function);

	bool isDefinedAt(int) const override final;
	int operator()(int) const override final;

	PartialFunction* clone() const override final;

private:
	T function;
};