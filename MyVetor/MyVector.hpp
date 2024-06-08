#pragma once
#include <iostream>
#include <exception>
#define st size_t

template<class T>
class MyVector {
public:
	MyVector();
	MyVector(int);
	MyVector(const MyVector&);
	MyVector(MyVector&&);

	MyVector& operator=(const MyVector&);
	MyVector& operator=(MyVector&&);

	void push(const T&);
	void push(T&&);
	void pop();
	const T& peek() const;

	st getSize() const;
	bool isEmpty() const;
	void clear();

	T& operator[](int);
	const T& operator[](int) const;

	~MyVector();

private:

	void free();
	void copyFrom(const MyVector&);
	void moveFrom(MyVector&&);

	void resize(st);

	T* data = nullptr;
	st size = 0;
	st capacity = 8;
};

template<class T>
MyVector<T>::MyVector() {
	data = new T[capacity];
}

template<class T>
MyVector<T>::MyVector(int cap) {
	if (cap <= 0) {
		throw std::logic_error("Vector must have a positive capacity!");
	}

	this->capacity = cap;
	data = new T[capacity];
}

template<class T>
MyVector<T>::MyVector(const MyVector& other)
{
	copyFrom(other);
}

template<class T>
MyVector<T>::MyVector(MyVector&& other)
{
	moveFrom(std::move(other));
}

template<class T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other)
{
	if (this != &other) {
		free();
		copyFrom(other);
	}

	return *this;
}

template<class T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other)
{
	if (this != &other) {
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

template<class T>
void MyVector<T>::push(const T& obj)
{
	if (size >= capacity) {
		resize(capacity * 2);
	}

	data[size++] = obj;
}

template<class T>
void MyVector<T>::push(T&& obj)
{
	if (size >= capacity) {
		resize(capacity * 2);
	}

	data[size++] = std::move(obj);
}

template<class T>
void MyVector<T>::pop()
{
	if (isEmpty()) {
		throw std::out_of_range("Vector is empty!");
	}

	size--;
}

template<class T>
const T& MyVector<T>::peek() const
{
	if (isEmpty()) {
		throw std::out_of_range("Vector is empty!");
	}

	return data[size - 1];
}

template<class T>
st MyVector<T>::getSize() const
{
	return size;
}

template<class T>
bool MyVector<T>::isEmpty() const
{
	return !size;
}

template<class T>
void MyVector<T>::clear()
{
	size = 0;
}

template<class T>
T& MyVector<T>::operator[](int ind)
{
	if (ind < 0 || ind >= size) {
		throw std::out_of_range("Index out of range");
	}

	return data[ind];
}

template<class T>
const T& MyVector<T>::operator[](int ind) const
{
	if (ind < 0 || ind >= size) {
		throw std::out_of_range("Index out of range");
	}

	return data[ind];
}

template<class T>
MyVector<T>::~MyVector()
{
	free();
}

template<class T>
void MyVector<T>::free()
{
	delete[] data;
	data = nullptr;
	size = capacity = 0;
}

template<class T>
void MyVector<T>::copyFrom(const MyVector& other)
{
	data = new T[other.size];
	size = other.size;
	capacity = other.capacity;

	for (size_t i = 0; i < size; i++)
	{
		data[i] = other.data[i];
	}
}

template<class T>
void MyVector<T>::moveFrom(MyVector&& other)
{
	data = other.data;
	size = other.size;
	capacity = other.capacity;
	other.data = nullptr;
	other.size = other.capacity = 0;
}

template<class T>
void MyVector<T>::resize(st newCap)
{
	capacity = newCap;
	T* newData = new T[capacity];

	for (size_t i = 0; i < size; i++) {
		newData[i] = std::move(data[i]);
	}

	delete[] data;
	data = newData;
}
