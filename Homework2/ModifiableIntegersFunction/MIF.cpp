#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>
#include "MIF.h"
#include "StaticSet.h"
#include "Exceptions.h"

namespace Utility {
	constexpr int DRAWING_RANGE = 20;

	int getIndex(int ind) {
		return ind >= 0 ? ind : -ind;
	}

	bool isInRange(ll arg) {
		return (arg >= SHRT_MIN && arg <= SHRT_MAX);
	}

	void tryValue(ll value) {
		if (!isInRange(value)) {
			throw std::out_of_range(Exceptions::OUT_OF_RANGE_EXCEPTION);
		}
	}

	unsigned getNumLen(int num) {
		unsigned result = 1;

		num = std::abs(num);

		if (num > 9) {
			result += log10(num);
		}

		return result;
	}

	void Nchars(short len, char ch) {
		for (short i = 0; i < len; i++)
		{
			std::cout << ch;
		}
	}
}

MIF::MIF() = default;

MIF::MIF(ll (*function) (ll arg)) {
	// Calculate all values
	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		ll currentValue = function(i);

		if (i < 0) {
			// If value is outside of range, make function undefined for current argument
			if (!Utility::isInRange(currentValue)) {
				isUndefinedNegative.add(-i);
			}
			// Otherwise add the valid value
			else {
				negativeValues[-i] = currentValue;
			}
		}
		else {
			if (!Utility::isInRange(currentValue)) {
				isUndefinedPositive.add(i);
			}
			else {
				positiveValues[i] = currentValue;
			}
		}
	}
}

// positiveValues -> negativeValues -> isUndefinedPositve -> isUndefinedNegative
MIF::MIF(const char* file) {
	if (!file) {
		throw std::exception(Exceptions::INVALID_FILENAME_EXCEPTION);
	}

	std::ifstream ifs(file, std::ios::binary);

	if (!ifs.is_open()) {
		throw std::exception(Exceptions::CLOSED_FILE_EXCEPTION);
	}

	ifs.read((char*)positiveValues, sizeof(short) * (SHRT_MAX + 1));
	ifs.read((char*)negativeValues, sizeof(short) * (SHRT_MAX + 1 + 1));

	ifs.read((char*)&isUndefinedPositive, sizeof(StaticSet));
	ifs.read((char*)&isUndefinedNegative, sizeof(StaticSet));

	ifs.close();
}

void MIF::checkInjectiveness() const {
	StaticSet posValues;
	StaticSet negValues;

	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		int index = Utility::getIndex(i);

		if (isUndefinedNegative.contains(index) || isUndefinedPositive.contains(index)) {
			continue;
		}

		short currentValue = (*this)(i);

		if (currentValue < 0) {
			if (negValues.contains(-currentValue)) {
				isinjective = issurjective = -1;
				return;
			}
			else {
				negValues.add(-currentValue);
			}
		}
		else {
			if (posValues.contains(currentValue)) {
				isinjective = issurjective = -1;
				return;
			}
			else {
				posValues.add(currentValue);
			}
		}
	}

	isinjective = 1;
}

void MIF::checkSurjectiveness() const {
	StaticSet posValues;
	StaticSet negValues;

	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		int index = Utility::getIndex(i);

		if (isUndefinedNegative.contains(index) || isUndefinedPositive.contains(index)) {
			issurjective = -1;
			return;
		}
		else {
			short currentValue = (*this)(i);

			if (currentValue < 0) {
				if (negValues.contains(-currentValue)) {
					issurjective = -1;
					return;
				}
				else {
					negValues.add(-currentValue);
				}
			}
			else {
				if (posValues.contains(currentValue)) {
					issurjective = -1;
					return;
				}
				else {
					posValues.add(currentValue);
				}
			}
		}
	}

	isinjective = issurjective = 1;
}

void MIF::predefineAt(ll ind, ll newValue) {
	Utility::tryValue(ind);

	if (ind >= 0) {
		if (Utility::isInRange(newValue)) {
			positiveValues[ind] = newValue;
			isUndefinedPositive.remove(ind);
			isinjective = issurjective = 0;
		}
		else {
			isUndefinedPositive.add(ind);
			issurjective = -1;
		}
	}
	else {
		if (Utility::isInRange(newValue)) {
			negativeValues[-ind] = newValue;
			isUndefinedNegative.remove(-ind);
			isinjective = issurjective = 0;
		}
		else {
			isUndefinedNegative.add(-ind);
			issurjective = -1;
		}
	}
}

void MIF::makeUndefinedAt(ll ind) {
	Utility::tryValue(ind);

	if (ind >= 0) {
		isUndefinedPositive.add(ind);
	}
	else {
		isUndefinedNegative.add(-ind);
	}

	issurjective = -1;
}

bool MIF::isDefinedAt(ll ind) const {
	Utility::tryValue(ind);

	if (ind >= 0) {
		return !isUndefinedPositive.contains(ind);
	}
	else {
		return !isUndefinedNegative.contains(-ind);
	}
}

short MIF::operator()(ll arg) const {
	Utility::tryValue(arg);

	if (arg >= 0) {
		if (isUndefinedPositive.contains(arg)) {
			throw std::logic_error(Exceptions::UNDEFINED_FUNCTION_EXCEPTION);
		}
		else {
			return positiveValues[arg];
		}
	}
	else {
		if (isUndefinedNegative.contains(-arg)) {
			throw std::logic_error(Exceptions::UNDEFINED_FUNCTION_EXCEPTION);
		}
		else {
			return negativeValues[-arg];
		}
	}
}

bool MIF::compare(const MIF& other, bool(*pred)(short, short)) const {
	//Compare all values
	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		if (i < 0) {
			ll thisValue = this->negativeValues[-i];
			ll otherValue = other.negativeValues[-i];

			if (this->isUndefinedNegative.contains(-i)) {
				thisValue = SHRT_MIN;
			}

			if (other.isUndefinedNegative.contains(-i)) {
				otherValue = SHRT_MIN;
			}

			if (!pred(thisValue, otherValue)) {
				return false;
			}
		}
		else {
			ll thisValue = this->positiveValues[i];
			ll otherValue = other.positiveValues[i];

			if (this->isUndefinedPositive.contains(i)) {
				thisValue = SHRT_MIN;
			}

			if (other.isUndefinedPositive.contains(i)) {
				otherValue = SHRT_MIN;
			}

			if (!pred(thisValue, otherValue)) {
				return false;
			}
		}
	}

	return true;
}

bool operator<(const MIF& first, const MIF& second) {
	if (&first == &second) {
		return false;
	}

	return first.compare(second, [](short thisValue, short otherValue) { return thisValue < otherValue; });
}

bool operator>(const MIF& first, const MIF& second) {
	if (&first == &second) {
		return false;
	}

	return first.compare(second, [](short thisValue, short otherValue) { return thisValue > otherValue; });
}

bool operator==(const MIF& first, const MIF& second) {
	if (&first == &second) {
		return true;
	}

	return first.compare(second, [](short thisValue, short otherValue) { return thisValue == otherValue; });
}

MIF& MIF::applyOperation(const MIF& other, ll(*operation)(ll, ll)) {
	//Apply operation for all arguments
	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		if (i < 0) {
			ll newValue = operation(this->negativeValues[-i], other.negativeValues[-i]);

			if (!Utility::isInRange(newValue)) {
				this->isUndefinedNegative.add(-i);
			}
			else {
				this->negativeValues[-i] = newValue;
				this->isUndefinedNegative.remove(-i);
			}
		}
		else {
			ll newValue = operation(this->positiveValues[i], other.positiveValues[i]);

			if (!Utility::isInRange(newValue)) {
				this->isUndefinedPositive.add(i);
			}
			else {
				this->positiveValues[i] = newValue;
				this->isUndefinedPositive.remove(i);
			}
		}
	}

	return *this;
}

MIF& MIF::operator+=(const MIF& other) {
	isinjective = issurjective = 0;
	return applyOperation(other, [](ll thisValue, ll otherValue) { return thisValue += otherValue; });
}

MIF& MIF::operator-=(const MIF& other) {
	isinjective = issurjective = 0;
	return applyOperation(other, [](ll thisValue, ll otherValue) { return thisValue -= otherValue; });
}

MIF operator+(const MIF& first, const MIF& second) {
	MIF result = first;
	return result += second;
}

MIF operator-(const MIF& first, const MIF& second) {
	MIF result = first;
	return result -= second;
}

bool operator||(const MIF& first, const MIF& second) {
	if (&first == &second) {
		return true;
	}

	int prevDiff = INT_MAX;

	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		if (i < 0) {
			if (first.isUndefinedNegative.contains(-i) || second.isUndefinedNegative.contains(-i)) {
				prevDiff = INT_MAX;
				continue;
			}

			const int currentDiff = first.negativeValues[-i] - second.negativeValues[-i];

			if (prevDiff == currentDiff || prevDiff == INT_MAX) {
				prevDiff = currentDiff;
				continue;
			}
			else if (currentDiff != prevDiff) {
				return false;
			}
		}
		else {
			if (first.isUndefinedPositive.contains(i) || second.isUndefinedPositive.contains(i)) {
				prevDiff = INT_MAX;
				continue;
			}

			const int currentDiff = first.positiveValues[i] - second.positiveValues[i];

			if (prevDiff == currentDiff || prevDiff == INT_MAX) {
				prevDiff = currentDiff;
				continue;
			}
			else if (currentDiff != prevDiff) {
				return false;
			}
		}
	}

	return true;
}

MIF MIF::operator()(const MIF& other) const {
	MIF result = *this;

	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		if (i < 0) {
			if (other.isUndefinedNegative.contains(-i) || this->isUndefinedNegative.contains(-i)) {
				result.isUndefinedNegative.add(-i);
				continue;
			}

			try {
				result.negativeValues[-i] = (*this)(other(i));
			}
			catch (...) {
				result.isUndefinedNegative.add(-i);
			}
		}
		else {
			if (other.isUndefinedPositive.contains(i) || this->isUndefinedPositive.contains(i)) {
				result.isUndefinedPositive.add(i);
				continue;
			}

			try {
				result.positiveValues[i] = (*this)(other(i));
			}
			catch (...) {
				result.isUndefinedPositive.add(i);
			}
		}
	}

	return result;
}

MIF MIF::operator^(ll power) const {
	if (power < 1) {
		throw std::exception(Exceptions::INVALID_POWER_EXCEPTION);
	}
	else {
		MIF result = *this;

		for (unsigned i = 1; i < power; i++)
		{
			result = (*this)(result);
		}

		return result;
	}
}

MIF MIF::getReverse() const {
	if (!this->isBijective()) {
		throw std::exception(Exceptions::IRREVIRSIBLE_FUNCTION_EXCEPTION);
	}

	MIF result;
	result.isinjective = result.issurjective = 1;

	for (int i = SHRT_MIN; i <= SHRT_MAX; i++)
	{
		short y = (*this)(i);
		if (y < 0) {
			result.negativeValues[-y] = i;
		}
		else {
			result.positiveValues[y] = i;
		}
	}

	return result;
}

void MIF::drawHeaderFooter(short X, short Y, short indent, short spaceCount, char ch) const {
	Utility::Nchars(spaceCount, ' ');
	std::cout << "(" << X << ";" << Y << ") ";
	Utility::Nchars(2 * Utility::DRAWING_RANGE, ch);
	std::cout << " (" << X + 20 << ";" << Y << ") " << std::endl;
}

void MIF::drawRow(short indent, short x, short y) const {
	Utility::Nchars(indent, ' ');

	std::cout << '|';

	for (unsigned i = 0; i < Utility::DRAWING_RANGE; i++)
	{
		if (x < 0) {
			if (!isUndefinedNegative.contains(-x) && negativeValues[-x] == y) {
				std::cout << "()";
			}
			else {
				Utility::Nchars(2, ' ');
			}
		}
		else {
			if (!isUndefinedPositive.contains(x) && positiveValues[x] == y) {
				std::cout << "()";
			}
			else {
				Utility::Nchars(2, ' ');
			}
		}

		x++;
	}

	std::cout << '|';
}

// The drawing will be 20 '|' in height and 40 '_' in width with every 2 '_' representing one point ()
void MIF::draw(short x, short y) const {
	unsigned xLen = x > 0 ? Utility::getNumLen(x) : Utility::getNumLen(x) + 1;
	unsigned yLen = y > 0 ? Utility::getNumLen(y) : Utility::getNumLen(y) + 1;
	short y1 = y + 20;
	unsigned y1Len = y1 > 0 ? Utility::getNumLen(y1) : Utility::getNumLen(y1) + 1;

	unsigned indent = 1 + xLen + 1 + std::max(yLen, y1Len) + 1;

	drawHeaderFooter(x, y1, indent, indent - 1 - xLen - 1 - y1Len - 1, '_');

	short y1Copy = y1 - 1;
	for (unsigned i = 0; i < Utility::DRAWING_RANGE; i++)
	{
		drawRow(indent, x, y1Copy);
		y1Copy--;
		std::cout << std::endl;
	}

	// Last row of drawing
	Utility::Nchars(indent, ' ');
	std::cout << '|';
	Utility::Nchars(Utility::DRAWING_RANGE * 2, '_');
	std::cout << '|' << std::endl;

	//										(   num    ;   num    )
	drawHeaderFooter(x, y, indent, indent - 1 - xLen - 1 - yLen - 1, ' ');
}

void MIF::drawFrom(ll x, ll y) const {
	Utility::tryValue(x);
	Utility::tryValue(y);

	if (x + Utility::DRAWING_RANGE > SHRT_MAX || y + Utility::DRAWING_RANGE > SHRT_MAX) {
		throw std::exception(Exceptions::BIG_STARTING_COORDINATES_EXCEPTION);
	}

	std::cout << std::endl;
	draw(x, y);

	// f(x) = y drawing
	// 
	// (x;y1) ________________________________ (x1;y1)
	//       |		  					    ()|
	//		 |							  ()  |
	//		 |							()    |
	//		 |						  ()      |
	//		 |						()		  |
	//		 |					  ()		  |
	//		 |					()			  |
	//		 |				  ()			  |
	//		 |				()				  |
	//		 |			  ()				  |
	//		 |			()					  |
	//		 |		  ()					  |
	//		 |		()						  |
	//		 |	  ()						  |
	//	     |	()							  |
	//       |________________________________|
	//  (x;y)								   (x1;y)
}

bool MIF::isInjective() const {
	if (isinjective == 0) {
		checkInjectiveness();
	}

	return isinjective > 0 ? true : false;
}

bool MIF::isSurjective() const {
	if (issurjective == 0) {
		checkSurjectiveness();
	}

	return (issurjective > 0 && isinjective > 0) ? true : false;
}

bool MIF::isBijective() const {
	// A function with equal domain and codomain can only be surjective if it is also injective
	return isSurjective();
}

// positiveValues -> negativeValues -> isUndefinedPositve -> isUndefinedNegative
void MIF::save(const char* file) const {
	if (!file) {
		throw std::exception(Exceptions::INVALID_FILENAME_EXCEPTION);
	}

	std::ofstream ofs(file, std::ios::binary);

	if (!ofs.is_open()) {
		throw std::exception(Exceptions::CLOSED_FILE_EXCEPTION);
	}

	ofs.write((const char*)positiveValues, sizeof(short) * (SHRT_MAX + 1));
	ofs.write((const char*)negativeValues, sizeof(short) * (SHRT_MAX + 1 + 1));

	ofs.write((const char*)&isUndefinedPositive, sizeof(StaticSet));
	ofs.write((const char*)&isUndefinedNegative, sizeof(StaticSet));

	ofs.close();
}
