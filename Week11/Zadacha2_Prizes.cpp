#include <iostream>
#define st size_t

enum class PrizeType {
	PENGUIN,
	PACMAN,
	COMPUTERBOY,
	SQUIDWARD,
	SYLVESTER
};

class Prize {
protected:
	int price = 0;

public:
	virtual void visualize() const = 0;
	virtual Prize* clone() const = 0;

	virtual ~Prize() = default;
};

static const int PENGUIN_PRICE = 50;
class Penguin : public Prize {
public:
	Penguin() {
		price = PENGUIN_PRICE;
	}

	void visualize() const override final {
		std::cout << "I'm a penguin" << std::endl;
	}
	Prize* clone() const override final {
		return new (std::nothrow) Penguin(*this);
	}
};

static const int PACMAN_PRICE = 150;
class Pacman : public Prize {
public:
	Pacman() {
		price = PACMAN_PRICE;
	}

	void visualize() const override final {
		std::cout << "I'm pacman" << std::endl;
	}
	Prize* clone() const override final {
		return new (std::nothrow) Pacman(*this);
	}
};

static const int COMPUTERBOY_PRICE = 500;
class ComputerBoy : public Prize {
public:
	ComputerBoy() {
		price = COMPUTERBOY_PRICE;
	}

	void visualize() const override final {
		std::cout << "I'm computer boy" << std::endl;
	}
	Prize* clone() const override final {
		return new (std::nothrow) ComputerBoy(*this);
	}
};

static const int SQUIDWARD_PRICE = 1000;
class Squidward : public Prize {
public:
	Squidward() {
		price = SQUIDWARD_PRICE;
	}

	void visualize() const override final {
		std::cout << "I'm squidward" << std::endl;
	}
	Prize* clone() const override final {
		return new (std::nothrow) Squidward(*this);
	}
};

static const int SYLVESTER_PRICE = 5000;
class Sylvester : public Prize {
public:
	Sylvester() {
		price = SYLVESTER_PRICE;
	}

	void visualize() const override final {
		std::cout << "I'm sylvester" << std::endl;
	}
	Prize* clone() const override final {
		return new (std::nothrow) Sylvester(*this);
	}
};

Prize* prizeFactory(PrizeType type) {
	switch (type)
	{
	case PrizeType::PENGUIN: return new Penguin();
	case PrizeType::PACMAN: return new Pacman();
	case PrizeType::COMPUTERBOY: return new ComputerBoy();
	case PrizeType::SQUIDWARD: return new Squidward();
	case PrizeType::SYLVESTER: return new Sylvester();
	}
}

class PrizeContainer {
private:
	Prize** data = nullptr;
	st size = 0;
	st capacity = 0;

	void copyFrom(const PrizeContainer& other) {
		size = other.size;
		capacity = other.capacity;
		data = new Prize * [capacity];

		for (st i = 0; i < size; i++)
		{
			Prize* cloned = other.data[i]->clone();
			if (cloned) {
				data[i] = cloned;
			}
		}
	}

	void moveFrom(PrizeContainer&& other) {
		data = other.data;
		size = other.size;
		capacity = other.capacity;

		other.data = nullptr;
		other.size = other.capacity = 0;
	}

	void free() {
		for (st i = 0; i < size; i++)
		{
			delete data[i];
		}
		delete[] data;
		size = capacity = 0;
	}

	void resize() {
		Prize** newData = new (std::nothrow) Prize*[capacity * 2];
		if (!newData) {
			return;
		}

		for (st i = 0; i < size; i++)
		{
			newData[i] = data[i];
		}
		delete[] data;
		data = newData;
		capacity *= 2;
	}

public:
	PrizeContainer() {
		capacity = 8;
		data = new Prize*[capacity];
	}
	PrizeContainer(st cap) {
		data = new Prize*[cap];
		capacity = cap;
	}
	PrizeContainer(const PrizeContainer& other) {
		copyFrom(other);
	}
	PrizeContainer(PrizeContainer&& other) noexcept {
		moveFrom(std::move(other));
	}

	PrizeContainer& operator=(const PrizeContainer& other) {
		if (this != &other) {
			free();
			copyFrom(other);
		}

		return *this;
	}

	PrizeContainer& operator=(PrizeContainer&& other) noexcept {
		if (this != &other) {
			free();
			moveFrom(std::move(other));
		}

		return *this;
	}

	void addPrize(PrizeType type) {
		if (size == capacity) {
			resize();
		}
		data[size++] = prizeFactory(type);
	}

	void visualizeAll() const {
		for (st i = 0; i < size; i++)
		{
			data[i]->visualize();
		}
	}

	~PrizeContainer() {
		free();
	}
};

int main()
{
	PrizeContainer pc(5);
	pc.addPrize(PrizeType::COMPUTERBOY);
	pc.addPrize(PrizeType::SQUIDWARD);
	pc.addPrize(PrizeType::PACMAN);
	pc.addPrize(PrizeType::COMPUTERBOY);
	pc.addPrize(PrizeType::PENGUIN);

	pc.visualizeAll();
}
