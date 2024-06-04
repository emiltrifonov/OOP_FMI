#include "FunctionType2.h"

FunctionType2::FunctionType2(const int* args, st len)
{
	for (size_t i = 0; i < len; i++)
	{
		arguments[i] = args[i];
	}

	this->len = len;
}

bool FunctionType2::isDefinedAt(int arg) const
{
	return true;
}

Pair FunctionType2::operator()(int arg) const
{
	for (size_t i = 0; i < len; i++)
	{
		if (arguments[i] == arg) {
			return Pair(true, 1);
		}
	}

	return Pair(true,0);
}
