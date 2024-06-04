#pragma once
class Pair {
public:
	Pair(bool f, int s);

	bool getFirst() const;

	int getSecond() const;

private:
	bool first;
	int second;
};