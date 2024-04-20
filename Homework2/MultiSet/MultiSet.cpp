#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>
#include "MultiSet.h"
#include "Exceptions.h"

namespace Constants {
	constexpr int BITS_IN_CHAR = 8;
}

void MultiSet::copyFrom(const MultiSet& other) {
	maxNum = other.maxNum;
	bitsUsedForStorage = other.bitsUsedForStorage;
	storageLength = other.storageLength;

	storage = new (std::nothrow) char[storageLength + 1]; // +1 for '\0'
	if (!storage) {
		throw std::bad_alloc();
	}

	for (unsigned i = 0; i < storageLength; i++)
	{
		storage[i] = other.storage[i];
	}
}

void MultiSet::free() {
	maxNum = -1;
	bitsUsedForStorage = -1;
	storageLength = -1;
	delete[] storage;
	storage = nullptr;
}

MultiSet::MultiSet() = default;

void MultiSet::initStorage(st len) {
	for (unsigned i = 0; i <= len; i++)
	{
		storage[i] = '\0';
	}
}

MultiSet::MultiSet(int maxNum, int bitsUsedForStorage) {
	if (maxNum <= 0) {
		throw Exceptions::INVALID_MAX_NUM_EXCEPTION;
	}
	else if (bitsUsedForStorage < 1 || bitsUsedForStorage > Constants::BITS_IN_CHAR) {
		throw Exceptions::INVALID_BIT_STORAGE_EXCEPTION;
	}
	else {
		this->maxNum = maxNum;
		this->bitsUsedForStorage = bitsUsedForStorage;

		storageLength = (maxNum + 1) * bitsUsedForStorage / Constants::BITS_IN_CHAR;
		const bool hasRemainder = (((maxNum + 1) * bitsUsedForStorage) % Constants::BITS_IN_CHAR) != 0;

		storageLength += hasRemainder;

		storage = new (std::nothrow) char[storageLength + 1]; // +1 for '\0'
		if (!storage) {
			throw std::bad_alloc();
		}
		else {
			initStorage(storageLength);
		}
	}
}

MultiSet::MultiSet(const MultiSet& other) {
	copyFrom(other);
}

//Read maxNum(4B) --> bitsUsedForStorage(4B) --> storageLength(8B) --> storage(storageLength B)
MultiSet::MultiSet(const char* file) {
	if (!file) {
		throw Exceptions::INVALID_FILE_NAME_EXCEPTION;
	}

	std::ifstream ifs(file, std::ios::binary);

	if (!ifs.is_open()) {
		throw Exceptions::CLOSED_FILE_EXCEPTION;
	}

	ifs.read((char*)&maxNum, sizeof(maxNum));
	ifs.read((char*)&bitsUsedForStorage, sizeof(bitsUsedForStorage));
	ifs.read((char*)&storageLength, sizeof(storageLength));

	storage = new (std::nothrow) char[storageLength + 1]; // +1 for '\0'
	if (!storage) {
		throw std::bad_alloc();
	}
	else {
		ifs.read(storage, storageLength);
	}

	ifs.close();
}

MultiSet& MultiSet::operator=(const MultiSet& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	
	return *this;
}

int MultiSet::getStartingCellIndex(int num) const {
	return ((num * bitsUsedForStorage) / Constants::BITS_IN_CHAR);
}

int MultiSet::getStartingBitIndex(int num) const {
	return ((num * bitsUsedForStorage) % Constants::BITS_IN_CHAR);
}

void MultiSet::incrementCellAndBitIndex(int& cellIndex, int& bitIndex) const {
	bitIndex += 1;
	if (bitIndex >= Constants::BITS_IN_CHAR) {
		bitIndex = 0;
		cellIndex++;
	}
}

MultiSet& MultiSet::operator+=(int num) { // Will not do anything if number is already at max count
	if (num < 0 || num > maxNum)
	{
		throw Exceptions::INVALID_NEW_NUMBER_EXCEPTION;
	}

	if (count(num) == (1 << bitsUsedForStorage) - 1) {
		return *this;
	}

	int cellIndex = getStartingCellIndex(num); // Index starting from 0
	int bitIndex = getStartingBitIndex(num); // Index from 0 to 7

	for (unsigned i = 0; i < bitsUsedForStorage; i++)
	{
		const unsigned char mask = (1 << bitIndex);

		if ((storage[cellIndex] & mask) == 0) {
			storage[cellIndex] |= mask;
			break;
		}
		else {
			storage[cellIndex] ^= mask;
		}

		incrementCellAndBitIndex(cellIndex, bitIndex);
	}

	return *this;
}

int MultiSet::count(int num) const {
	int result = 0;
	int cellIndex = getStartingCellIndex(num); // Index starting from 0
	int bitIndex = getStartingBitIndex(num); // Index from 0 to 7

	for (unsigned i = 0; i < bitsUsedForStorage; i++)
	{
		const unsigned char mask = (1 << bitIndex);

		if ((storage[cellIndex] & mask) != 0) {
			result += (1 << i);
		}

		incrementCellAndBitIndex(cellIndex, bitIndex);
	}

	return result;
}

std::ostream& operator<<(std::ostream& os, const MultiSet& set) {
	os << "MultiSet (0 - " << set.maxNum << ", max. count -> " << ((1 << set.bitsUsedForStorage) - 1) << ")" << std::endl;

	for (unsigned i = 0; i <= set.maxNum; i++)
	{
		const int currentCount = set.count(i);
		if (currentCount != 0) {
			os << i << " - x" << currentCount << std::endl;
		}
	}

	return os;
}

void MultiSet::printMemoryRepresentation() const {

	for (unsigned i = 0; i < storageLength; i++)
	{
		const char currChar = storage[i];
		for (int i = Constants::BITS_IN_CHAR - 1; i >= 0; i--)
		{
			const char bit = ((currChar) & (1 << i)) ? '1' : '0';
			std::cout << bit;
		}
		std::cout << " ";
	}
}

MultiSet MultiSet::getComplement() const {
	MultiSet complement = *this;

	for (unsigned i = 0; i < storageLength; i++)
	{
		complement.storage[i] = ~complement.storage[i];
	}

	return complement;
}

void MultiSet::add(int num, int count) {
	for (unsigned i = 0; i < count; i++)
	{
		*this += num;
	}
}

// Returns intersection of 2 MultiSets -> this ^ other = { x | x is in 'first' and x is in 'second' }
MultiSet operator^(const MultiSet& first, const MultiSet& second) {
	if (&first == &second) {
		return first;
	}

	const int maxNum = std::min(first.maxNum, second.maxNum);
	const int bitsUsedForStorage = std::min(first.bitsUsedForStorage, second.bitsUsedForStorage);

	MultiSet result(maxNum, bitsUsedForStorage);

	for (unsigned i = 0; i <= maxNum; i++)
	{
		const int count = std::min(first.count(i), second.count(i));
		result.add(i, count);
	}

	return result;
}

// Returns set difference -> this / other = { x | x is in 'first' and x is not in 'second' }
MultiSet operator/(const MultiSet& first, const MultiSet& second) {
	MultiSet result(first.maxNum, first.bitsUsedForStorage);

	for (unsigned i = 0; i < result.maxNum; i++)
	{
		const int count = first.count(i) - second.count(i);
		if (count > 0) {
			result.add(i, count);
		}
	}

	return result;
}

//Save maxNum(4B) --> bitsUsedForStorage(4B) --> storageLength(8B) --> storage(storageLength B)
void MultiSet::save(const char* file) const {
	std::ofstream ofs(file, std::ios::binary | std::ios::trunc);

	if (!ofs.is_open()) {
		throw Exceptions::CLOSED_FILE_EXCEPTION;
	}

	ofs.write((const char*)&maxNum, sizeof(maxNum));
	ofs.write((const char*)&bitsUsedForStorage, sizeof(bitsUsedForStorage));
	ofs.write((const char*)&storageLength, sizeof(storageLength));
	ofs.write((const char*)storage, storageLength);

	ofs.close();
}

MultiSet::~MultiSet() {
	free();
}
