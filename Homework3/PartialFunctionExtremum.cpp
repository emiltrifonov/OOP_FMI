#include <iostream>
#include <exception>
#include "PartialFunctionExtremum.h"

PartialFunctionExtremum::PartialFunctionExtremum(const PartialFunctionExtremum& other)
{
	copyFrom(other);
}

PartialFunctionExtremum::PartialFunctionExtremum(PartialFunctionExtremum&& other)
{
	moveFrom(std::move(other));
}

PartialFunctionExtremum::PartialFunctionExtremum(MyVector<PartialFunction*> functions)
{
	this->functions = functions;
}

PartialFunctionExtremum& PartialFunctionExtremum::operator=(const PartialFunctionExtremum& other)
{
	if (this != &other) {
		free();
		copyFrom(other);
	}

	return *this;
}

PartialFunctionExtremum& PartialFunctionExtremum::operator=(PartialFunctionExtremum&& other)
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

bool PartialFunctionExtremum::isDefinedAt(ll arg) const
{
	for (size_t i = 0; i < functions.getSize(); i++)
	{
		if (!functions[i]->isDefinedAt(arg)) {
			return false;
		}
	}

	return true;
}

PartialFunctionExtremum::~PartialFunctionExtremum()
{
	free();
}

void PartialFunctionExtremum::free()
{
	for (size_t i = 0; i < functions.getSize(); i++)
	{
		delete functions[i];
	}
}

void PartialFunctionExtremum::copyFrom(const PartialFunctionExtremum& other)
{
	for (size_t i = 0; i < other.functions.getSize(); i++)
	{
		functions[i] = other.functions[i]->clone();
	}
}

void PartialFunctionExtremum::moveFrom(PartialFunctionExtremum&& other)
{
	for (size_t i = 0; i < other.functions.getSize(); i++)
	{
		functions[i] = other.functions[i];
		other.functions[i] = nullptr;
	}
}
