#include <iostream>
#include <cstring>
#include "MyString.h"
#pragma warning (disable:4996)

MyString::MyString() = default;

MyString::MyString(const char* str) {
	if (!str) {
		return;
	}

	size = strlen(str);
	text = new char[size + 1];
	strcpy(text, str);
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

size_t MyString::length() const {
	return size;
}

MyString& MyString::operator=(const MyString& other) {
	free();
	copyFrom(other);

	return *this;
}

MyString::~MyString() {
	free();
}

void MyString::free() {
	delete[] text;
	text = nullptr;
	size = 0;
}

void MyString::copyFrom(const MyString& other) {
	if (this != &other) {
		if (!other.text) {
			return;
		}
		size = other.size;
		text = new char[size + 1];
		strcpy(text, other.text);
	}
}

bool MyString::operator==(const MyString& other) const {
	if (!text && !other.text) {
		return true;
	}
	else if (!text || !other.text) {
		return false;
	}
	else { 
		return !strcmp(text, other.text);
	}
}

//friend std::ostream& MyString::operator<<(std::ostream& os, const MyString& myStr) const {
//	if (!myStr.text) {
//		return os;
//	}
//
//	return os << myStr.text;
//}
