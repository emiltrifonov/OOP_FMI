#include <iostream>
#include <cstring>
#include "MyString.h"
#pragma warning (disable:4996)

MyString::MyString() = default;

MyString::MyString(st cap)
{
	text = new char[cap + 1];
	capacity = cap;
	text[0] = '\0';
}

MyString::MyString(const char* str) {
	if (!str) {
		return;
	}

	size = capacity = strlen(str);
	text = new char[size + 1];
	strcpy(text, str);
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString::MyString(MyString&& other) noexcept
{
	moveFrom(std::move(other));
}

st MyString::getLength() const {
	return size;
}

const char* MyString::c_str() const
{
	return text;
}

char& MyString::operator[](int ind)
{
	return text[ind];
}

const char& MyString::operator[](int ind) const
{
	return text[ind];
}

MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		if (other.size > capacity) {
			free();
			copyFrom(other);
		}
	}

	return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

MyString& MyString::operator+=(const MyString& other)
{
	if (capacity < size + other.size) {
		resize(size + other.size);
	}

	strcat(text, other.text);
	size += other.size;

	return *this;
}

MyString::~MyString() {
	free();
}

void MyString::free() {
	delete[] text;
	text = nullptr;
	size = capacity = 0;
}

void MyString::resize()
{
	resize(capacity * 2);
}

void MyString::resize(st newCap)
{
	char* newData = new char[newCap + 1];
	strcpy(newData, text);
	delete[] text;
	text = newData;
	capacity = newCap;
}

void MyString::copyFrom(const MyString& other) {
	if (this != &other) {
		if (!other.text) {
			return;
		}
		size = other.size;
		capacity = other.capacity;
		text = new char[size + 1];
		strcpy(text, other.text);
	}
}

void MyString::moveFrom(MyString&& other)
{
	text = other.text;
	size = other.size;
	capacity = other.capacity;
	other.text = nullptr;
	other.size = other.capacity = 0;
}

MyString operator+(const MyString& first, const MyString& second)
{
	MyString result(first.getLength() + second.getLength());
	result += first;
	result += second;
	return result;
}

bool operator==(const MyString& first, const MyString& second) {
	return strcmp(first.c_str(), second.c_str()) == 0;
}

bool operator!=(const MyString& first, const MyString& second)
{
	return strcmp(first.c_str(), second.c_str()) != 0;
}

bool operator<(const MyString& first, const MyString& second)
{
	return strcmp(first.c_str(), second.c_str()) < 0;
}

bool operator<=(const MyString& first, const MyString& second)
{
	return strcmp(first.c_str(), second.c_str()) <= 0;
}

bool operator>(const MyString& first, const MyString& second)
{
	return strcmp(first.c_str(), second.c_str()) > 0;
}

bool operator>=(const MyString& first, const MyString& second)
{
	return strcmp(first.c_str(), second.c_str()) >= 0;
}

std::ostream& operator<<(std::ostream& os, const MyString& myStr) {
	if (!myStr.text) {
		return os;
	}

	return os << myStr.text;
}

std::istream& operator>>(std::istream& is, MyString& myStr)
{
	char buff[1024];
	is >> buff;
	st buffSize = strlen(buff);

	if (buffSize > myStr.capacity) {
		myStr.resize(buffSize);
	}

	strcpy(myStr.text, buff);
	myStr.size = buffSize;

	return is;
}
