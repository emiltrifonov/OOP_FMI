#include <iostream>
#include "Pair.h"

Pair::Pair(bool f, int s)
{
	first = f;
	second = s;
}

bool Pair::getFirst() const
{
	return first;
}

int Pair::getSecond() const
{
	return second;
}
