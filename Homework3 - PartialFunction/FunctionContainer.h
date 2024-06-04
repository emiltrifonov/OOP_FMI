#pragma once
#include "PartialFunction.h"
#define st size_t

class FunctionContainer {
public:
	FunctionContainer();
	FunctionContainer(st cap);
	FunctionContainer(const FunctionContainer&);
	FunctionContainer(FunctionContainer&&) noexcept;

	FunctionContainer& operator=(const FunctionContainer&);
	FunctionContainer& operator=(FunctionContainer&&) noexcept;

	void add(PartialFunction*);

	const PartialFunction* operator[](int ind) const;

	st getSize() const;

	~FunctionContainer();

private:

	void free();
	void copyFrom(const FunctionContainer&);
	void moveFrom(FunctionContainer&&);

	void resize(st newCap);

	PartialFunction** data = nullptr;
	st size = 0;
	st capacity = 8;
};