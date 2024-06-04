#pragma once
#include "PartialFunction.h"
#include "Pair.h"

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

template<class T>
PartialFunctionByCriteria<T>::PartialFunctionByCriteria(const T& function) {
	this->function = function;
}

template<class T>
bool PartialFunctionByCriteria<T>::isDefinedAt(int arg) const
{
	return function(arg).getFirst();
}

template<class T>
int PartialFunctionByCriteria<T>::operator()(int arg) const
{
	if (!isDefinedAt(arg)) {
		throw std::logic_error("Function not defined for argument!");
	}
	return function(arg).getSecond();
}

template<class T>
PartialFunction* PartialFunctionByCriteria<T>::clone() const
{
	return new PartialFunctionByCriteria(function);
}