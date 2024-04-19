#pragma once
#include <iostream>
#include "StaticSet.h"
#define ll long long int
#pragma warning (disable:4996)

class MIF {
private:
	// Including 0 there are 2^15 non-negatives
	short positiveValues[SHRT_MAX + 1] = { 0 };
	// There are 2^15 negatives (the element at index 0 will not be used for storing a value)
	short negativeValues[SHRT_MAX + 1 + 1] = { 0 };

	// 2 bitsets for whether function is defined for each argument
	StaticSet isUndefinedPositive; // Including 0
	StaticSet isUndefinedNegative;

	// Negative value(-1) -> false
	// Neutral  value( 0) -> Not checked
	// Positive value( 1) -> true
	mutable int8_t isinjective = 0, issurjective = 0;

	bool compare(const MIF&, bool(*pred)(short, short)) const;

	void checkInjectiveness() const;
	void checkSurjectiveness() const;

	void drawHeaderFooter(short, short, short, short, short, char) const;
	void drawRow(short, short, short) const;
	void draw(short, short, short, short) const;

public:
	MIF();
	MIF(ll (*function)(ll arg));
	// Binary file deserialization
	MIF(const char*);

	// Predefining for certain arguments
	void predefineAt(ll arg, ll newVal);
	void makeUndefinedAt(ll);
	bool isDefinedAt(ll) const;

	// Addition/subtraction of functions
	MIF& applyOperation(const MIF&, ll(*operation)(ll, ll));
	MIF& operator+=(const MIF&);
	MIF& operator-=(const MIF&);

	short operator()(ll) const;

	// Function composition
	MIF operator()(const MIF&) const;

	// Function comparison
	friend bool operator<(const MIF&, const MIF&);
	friend bool operator>(const MIF&, const MIF&);
	friend bool operator==(const MIF&, const MIF&);

	// Check for collinearity
	friend bool operator||(const MIF&, const MIF&);

	// Operator ^ for repeated application
	MIF operator^(ll) const;

	// Reverse function generation ---> TODO
	MIF getReverse() const;

	// Checks for injectiveness/surjectiveness/bijectiveness
	bool isInjective() const;
	bool isSurjective() const;
	bool isBijective() const;

	// Binary file serialization
	void save(const char*) const;

	// Drawing the function in a 20x20 area
	void drawFrom(ll, ll) const;


	void printInRange(ll smaller, ll bigger) const;
};

void tryValue(ll);
bool isInRange(ll);
unsigned getNumLen(int);
void Nchars(short, char);
int getIndex(int);

MIF operator+(const MIF&, const MIF&);
MIF operator-(const MIF&, const MIF&);
