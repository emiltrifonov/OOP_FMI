#pragma once
#define st size_t

class MultiSet {
private:
	// Will have length: [(maxNum * bitsUsedForStorage) / 8 ] + 1 if remainder != 0
	char* storage = nullptr;
	// Represents the actual length of the storage since it's initiated with '\0' as its elements and strlen() doesn't work
	st storageLength = -1;
	// Numbers in Multiset are in interval [0 ; maxNum]
	int maxNum = -1;
	// In interval [1 ; 8], limits each number's max count to (2 ^ bitsUsedForStorage - 1)
	int bitsUsedForStorage = -1;

	void copyFrom(const MultiSet&);
	void free();

	void initStorage(st);

	int getStartingCellIndex(int) const;
	int getStartingBitIndex(int) const;
	void incrementCellAndBitIndex(int&, int&) const;

	void add(int, int);

public:
	MultiSet();
	MultiSet(int, int);
	MultiSet(const MultiSet&);
	MultiSet(const char*);

	MultiSet& operator=(const MultiSet&);
	friend std::ostream& operator<<(std::ostream&, const MultiSet&);
	MultiSet& operator+=(int);
	// Set difference
	friend MultiSet operator/(const MultiSet&, const MultiSet&);
	// Intersection of sets
	friend MultiSet operator^(const MultiSet&, const MultiSet&);

	MultiSet getComplement() const;

	void printMemoryRepresentation() const;
	int count(int) const;
	void save(const char*) const;

	~MultiSet();
};
