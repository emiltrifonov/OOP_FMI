#pragma once

class MyString {
private:
	char* text = nullptr;
	size_t size = 0;

	void copyFrom(const MyString&);
	void free();

public:
	MyString();
	MyString(const char* str);
	MyString(const MyString& other);

	size_t length() const;

	MyString& operator=(const MyString& other);
	bool operator==(const MyString& other) const;
	//friend std::ostream& operator<<(std::ostream&, const MyString&) const;

	~MyString();
};
