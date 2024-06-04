#include <iostream>
#include <exception>
#include "PartialFunctionByCriteria.h"
#include "Utility.h"

template<class T>
PartialFunctionByCriteria<T>::PartialFunctionByCriteria(const T& function) {
	if (!function) {
		throw std::logic_error("Function could not be created!");
	}

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
		throw std::logic_error();
	}
	return function(arg).getSecond();
}

template<class T>
PartialFunction* PartialFunctionByCriteria<T>::clone() const
{
	return new PartialFunctionByCriteria(function);
}