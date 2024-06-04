#pragma once
#include "PartialFunction.h"
#include "MyVector.h"
#include "FunctionResult.h"

class PartialFunctionExtremum : public PartialFunction {
public:
	PartialFunctionExtremum(const PartialFunctionExtremum&);
	PartialFunctionExtremum(PartialFunctionExtremum&&);
	PartialFunctionExtremum(MyVector<PartialFunction*> functions); // big 6 because of Vector<PartialFunction*>

	PartialFunctionExtremum& operator=(const PartialFunctionExtremum&);
	PartialFunctionExtremum& operator=(PartialFunctionExtremum&&);

	bool isDefinedAt(ll) const override final;
	FunctionResult operator()(ll) const override final;

	FunctionResult operator()(ll) const = 0;
	PartialFunction* clone() const = 0;

	~PartialFunctionExtremum();

protected:
	MyVector<PartialFunction*> functions;

private:

	void free();
	void copyFrom(const PartialFunctionExtremum&);
	void moveFrom(PartialFunctionExtremum&&);
};