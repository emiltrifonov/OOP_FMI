#include <iostream>
#include <exception>
#include "FunctionType1.h"

FunctionType1::FunctionType1(const int* args, st len)
{
	for (size_t i = 0; i < len; i++)
	{
		arguments[i] = args[i];
	}

	this->len = len;
}

bool FunctionType1::isDefinedAt(int arg) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (arguments[i] == arg) {
			return false;
		}
	}

	return true;
}

Pair FunctionType1::operator()(int arg) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (arguments[i] == arg) {
			return Pair(false, 0);
		}
	}

	return Pair(true, arg);
}
