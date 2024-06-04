#include "FunctionContainer.h"
#include "FunctionFactory.h"
#include <iostream>
#include <exception>

FunctionContainer::FunctionContainer()
{
	data = new PartialFunction * [capacity] {nullptr};
}

FunctionContainer::FunctionContainer(st cap)
{
	capacity = cap;
	data = new PartialFunction * [capacity] {nullptr};
}

FunctionContainer::FunctionContainer(const FunctionContainer& other)
{
	copyFrom(other);
}

FunctionContainer::FunctionContainer(FunctionContainer&& other) noexcept
{
	moveFrom(std::move(other));
}

FunctionContainer& FunctionContainer::operator=(const FunctionContainer& other)
{
	if (this != &other) {
		free();
		copyFrom(other);
	}

	return *this;
}

FunctionContainer& FunctionContainer::operator=(FunctionContainer&& other) noexcept
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

void FunctionContainer::add(PartialFunction* obj)
{
	if (size >= capacity) {
		resize(capacity * 2);
	}

	data[size++] = obj;
}

const PartialFunction* FunctionContainer::operator[](int ind) const
{
	if (ind < 0 || ind >= size) {
		throw std::out_of_range("Invalid index!");
	}

	return data[ind];
}

st FunctionContainer::getSize() const
{
	return size;
}

FunctionContainer::~FunctionContainer()
{
	free();
}

void FunctionContainer::free()
{
	for (st i = 0; i < size; i++)
	{
		delete data[i];
	}

	delete[] data;
}

void FunctionContainer::copyFrom(const FunctionContainer& other)
{
	size = other.size;
	capacity = other.capacity;
	data = new PartialFunction* [size];

	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i]->clone();
	}
}

void FunctionContainer::moveFrom(FunctionContainer&& other)
{
	data = other.data;
	size = other.size;
	capacity = other.capacity;

	other.data = nullptr;
	other.size = other.capacity = 0;
}

void FunctionContainer::resize(st newCap)
{	
	capacity = newCap;
	PartialFunction** newData = new PartialFunction* [capacity];

	for (size_t i = 0; i < size; i++)
	{
		newData[i] = data[i];
	}

	delete[] data;
	data = newData;
}
