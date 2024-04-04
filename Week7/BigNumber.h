#pragma once
#pragma warning(disable : 4996)
#define ll long long int
#define st size_t

class BigNumber {
private:
	char* number = nullptr;
	st length = 0;
	bool isNegative = false;

	void free();
	void copyFrom(const BigNumber&);

public:
	BigNumber();
	BigNumber(const char*);
	BigNumber(ll);
	BigNumber(const BigNumber&);

	st getLength() const;

	BigNumber& operator=(const BigNumber&);

	friend std::ostream& operator<<(std::ostream&, const BigNumber&);

	bool operator == (const BigNumber& other) const;
	bool operator != (const BigNumber& other) const;
	bool operator < (const BigNumber& other) const;
	bool operator > (const BigNumber& other) const;

	~BigNumber();
};

st getNumLen(ll);
char getFirstDigit(ll, st);
void removeFirstDigit(ll&, st&);
