#include "FunctionExtremum.h"
#include <iostream>
#include <exception>

bool FunctionExtremum::isDefinedAt(int arg) const
{
	for (size_t i = 0; i < functions.getSize(); i++)
	{
		if (!functions[i]->isDefinedAt(arg)) {
			return false;
		}
	}

	return true;
}

int FunctionExtremum::calculateExtremum(int arg, bool(*pred)(int, int)) const
{
	int lastValue = (*functions[0])(arg);

	for (size_t i = 0; i < functions.getSize(); i++)
	{
		if (!functions[i]->isDefinedAt(arg)) {
			throw std::out_of_range("Function not defined for argument");
		}

		int currentValue = (*functions[i])(arg);
		if (pred(currentValue, lastValue)) {
			lastValue = currentValue;
		}
	}

	return lastValue;
}
