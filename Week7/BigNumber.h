#pragma once


class BigNumber {
private:
	char* number = nullptr;
	size_t length = 0;
	bool isNegative = false;

	void free();
	void copyFrom(const BigNumber&);

public:
	BigNumber();
	BigNumber(const char*);
	BigNumber(long long int);
	BigNumber(const BigNumber&);

	~BigNumber();
};
