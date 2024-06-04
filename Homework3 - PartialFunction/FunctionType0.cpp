#include <iostream>
#include "FunctionType0.h"

FunctionType0::FunctionType0(const int* args, const int* values, st len) // not final version
{
	for (size_t i = 0; i < len; i++)
	{
		arguments[i] = args[i];
		this->values[i] = values[i];
	}

	this->len = len;
}

bool FunctionType0::isDefinedAt(int arg) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (arguments[i] == arg) {
			return true;
		}
	}

	return false;
}

Pair FunctionType0::operator()(int arg) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (arguments[i] == arg) {
			return Pair(true,values[i]);
		}
	}

	return Pair(false, 0);
}