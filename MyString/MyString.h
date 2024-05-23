#pragma once
#define st size_t

class MyString {
private:
	char* text = nullptr;
	st size = 0;
	st capacity = 0;

	void copyFrom(const MyString&);
	void moveFrom(MyString&&);
	void free();

	void resize();
	void resize(st);

public:
	MyString();
	MyString(st);
	MyString(const char*);
	MyString(const MyString&); // C.C.
	MyString(MyString&&) noexcept; // M.C.

	st getLength() const;

	const char* c_str() const;

	char& operator[](int);
	const char& operator[](int) const;

	MyString& operator=(const MyString&); // OP=
	MyString& operator=(MyString&&) noexcept; // Move OP=

	friend std::ostream& operator<<(std::ostream&, const MyString&);
	friend std::istream& operator>>(std::istream&, MyString&);

	MyString& operator+=(const MyString&);

	~MyString();
};

MyString operator+(const MyString&, const MyString&);

bool operator==(const MyString&, const MyString&);
bool operator!=(const MyString&, const MyString&);
bool operator<(const MyString&, const MyString&);
bool operator<=(const MyString&, const MyString&);
bool operator>(const MyString&, const MyString&);
bool operator>=(const MyString&, const MyString&);
