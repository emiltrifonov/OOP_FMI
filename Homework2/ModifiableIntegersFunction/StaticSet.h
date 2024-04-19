#pragma once
#include <iostream>

constexpr unsigned MAX_NUM = SHRT_MAX + 1;
constexpr unsigned NUMBERS_COUNT = MAX_NUM;
constexpr unsigned BUCKET_SIZE = sizeof(uint8_t) * 8;
constexpr unsigned BUCKETS_COUNT = NUMBERS_COUNT / BUCKET_SIZE + 1;

class StaticSet
{
private:
	uint8_t buckets[BUCKETS_COUNT] = { 0 };
	unsigned getBucketIndex(unsigned n) const;
	unsigned getBitIndex(unsigned n) const;

public:
	void add(unsigned n);
	void remove(unsigned n);

	bool contains(unsigned n) const;
};
