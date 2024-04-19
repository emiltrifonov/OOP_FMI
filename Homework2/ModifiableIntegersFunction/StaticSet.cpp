#include "StaticSet.h"

unsigned StaticSet::getBucketIndex(unsigned n) const
{
	return n / BUCKET_SIZE;
}
unsigned StaticSet::getBitIndex(unsigned n) const
{
	return n % BUCKET_SIZE;
}

void StaticSet::add(unsigned n)
{
	if (n > MAX_NUM)
		return;

	unsigned bucketIndex = getBucketIndex(n);
	unsigned bitIndex = getBitIndex(n);

	uint8_t mask = 1 << bitIndex;

	buckets[bucketIndex] |= mask;

}

void StaticSet::remove(unsigned n)
{
	unsigned bucketIndex = getBucketIndex(n);
	unsigned bitIndex = getBitIndex(n);

	uint8_t mask = 1 << bitIndex;
	mask = ~mask;

	buckets[bucketIndex] &= mask;
}

bool StaticSet::contains(unsigned n) const
{
	unsigned bucketIndex = getBucketIndex(n);
	unsigned bitIndex = getBitIndex(n);

	uint8_t mask = 1 << bitIndex;

	return (buckets[bucketIndex] & mask) != 0;
}
