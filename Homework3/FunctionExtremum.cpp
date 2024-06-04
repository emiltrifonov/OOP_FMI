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
	bool isDefined = true;

	for (size_t i = 0; i < functions.getSize(); i++)
	{
		bool isCurrentDefined = functions[i]->isDefinedAt(arg);
		int currentValue = (*functions[i])(arg);

		if (!isCurrentDefined) {
			isDefined = false;
			break;
		}
		else if (pred(currentValue, lastValue)) {
			lastValue = currentValue;
		}
	}

	return lastValue;
}
