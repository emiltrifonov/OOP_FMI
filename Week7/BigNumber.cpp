#include <iostream>
#include <cstring>
#include <cmath>
#include "BigNumber.h"

void BigNumber::free() {
	delete[] number;
	number = nullptr;
	length = 0;
	isNegative = false;
}

void BigNumber::copyFrom(const BigNumber& other) {
	length = other.length;
	number = new char[length + 1];
	isNegative = other.isNegative;
	strcpy(number, other.number);
}

BigNumber::BigNumber() = default;

BigNumber::BigNumber(const char* number) {
	if (!number) {
		return;
	}
	else {
		if (number[0] == '-') {
			isNegative = true;
			number++;
		}

		length = strlen(number);
		this->number = new char[length + 1];
		this->number[length] = '\0';

		for (unsigned i = 0; i < length; i++)
		{
			char current = *number;
			if (isdigit(current)) {
				this->number[i] = current;
				number++;
			}
			else {
				free();
				break;
			}
		}
	}
}

st getNumLen(ll num) {
	st result = 1;
	if (num != 0) {
		result += log10(num);
	}

	return result;
}

char getFirstDigit(ll num, st len) {
	unsigned result = 0;

	result = num / (pow(10, len - 1));

	return result + '0';
}

void removeFirstDigit(ll& num, st& len) {
	ll divisor = pow(10, len - 1);

	num = num % divisor;

	len--;
}

BigNumber::BigNumber(ll number) {
	if (number < 0) {
		std::cout << "Make this negative\n\n";
		isNegative = true;
		number *= -1;
	}

	length = getNumLen(number);
	const st lenCopy = length;

	this->number = new char[length + 1];
	this->number[length] = '\0';

	if (number == 0) {
		this->number[0] = '0';
	}
	else {
		for (unsigned i = 0; i < lenCopy; i++)
		{
			char firstDigit = getFirstDigit(number, length);
			this->number[i] = firstDigit;
			removeFirstDigit(number, length);
		}
	}

	length = lenCopy;
}

BigNumber::BigNumber(const BigNumber& other) {
	copyFrom(other);
}

st BigNumber::getLength() const {
	return length;
}

BigNumber& BigNumber::operator=(const BigNumber& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

std::ostream& operator<<(std::ostream& os, const BigNumber& bigNumber) {
	return bigNumber.isNegative ? os << "-" << bigNumber.number : os << bigNumber.number;
}

bool BigNumber::operator==(const BigNumber& other) const {
	if (isNegative != other.isNegative) {
		return false;
	}
	return !strcmp(number, other.number) ? true : false;
}

BigNumber::~BigNumber() {
	free();
}
