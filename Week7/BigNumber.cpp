#include <iostream>
#include <cstring>
#include <cmath>
#include "BigNumber.h"
#pragma warning disable:4996

void BigNumber::free() {
	delete[] number;
	number = nullptr;
	length = 0;
	isNegative = false;
}

void BigNumber::copyFrom(const BigNumber& other) {
	if (this != &other) {
		length = other.length;
		isNegative = other.isNegative;
		strcpy(number, other.number);
	}
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

		while (number)
		{
			if (isdigit(*number)) {
				*(this->number) = *number;
				this->number++;
				number++;
			}
			else {
				free();
				break;
			}
		}
		this->number = '\0';
	}
}

BigNumber::BigNumber(long long int number) {
	if (number < 0) {
		isNegative = true;
		number *= -1;
	}

	length = 1;
	if (number != 0) {
		length += log10(number);
	}

	this->number = new char[length + 1];

	//TODO copy number to object
}

BigNumber::BigNumber(const BigNumber& other) {
	free();
	copyFrom(other);
}

BigNumber::~BigNumber() {
	free();
}
