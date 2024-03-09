#include <iostream>
#include <fstream>

bool isPrime(int num) {
	for (unsigned i = num / 2; i >= 2; i--)
	{
		if (num % i == 0) {
			return false;
		}
	}

	return true;
}

int getFileSize(std::ifstream& ifs) {
	int currPos = ifs.tellg();
	ifs.seekg(0, std::ios::end);
	int fileSize = ifs.tellg();

	ifs.clear();
	ifs.seekg(currPos);

	return fileSize;
}

void writeNumsToSeparateFiles(const char* fileName) {
	std::ifstream ifs(fileName, std::ios::binary);
	if (!ifs.is_open()) {
		return;
	}

	size_t fileSize = getFileSize(ifs);
	size_t arrSize = fileSize / sizeof(int);
	int* arr = new int[arrSize];
	ifs.read((char*)arr, fileSize);

	std::ofstream primeOfs("prime.dat", std::ios::binary);
	if (!primeOfs.is_open()) {
		return;
	}
	std::ofstream complexOfs("complex.dat", std::ios::binary);
	if (!complexOfs.is_open()) {
		return;
	}

	for (unsigned i = 0; i < arrSize; i++)
	{
		switch (isPrime(arr[i]))
		{
		case true:
			primeOfs.write((const char*)&arr[i], sizeof(arr[i]));
			break;
		case false:
			complexOfs.write((const char*)&arr[i], sizeof(arr[i]));
			break;
		}
	}

	ifs.close();
	primeOfs.close();
	complexOfs.close();

	delete[] arr;
}

int main()
{
	/*constexpr size_t size = 10;
    int a[size] = { 2,5,7,8,9,10,15,17,29,30 };
    std::ofstream ofs("numbers.dat", std::ios::binary);
	ofs.write((const char*)a, size * sizeof(int));*/
	writeNumsToSeparateFiles("numbers.dat");
}
