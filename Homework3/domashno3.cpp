#include <iostream>
#include <fstream>
#include "PartialFunction.h"
#include "PartialFunctionByCriteria.h"
#include "FunctionType0.h"
#include "MyVector.hpp"
#include "MyString.h"
#include "FunctionFactory.h"

using std::cout;
using std::cin;
using std::endl;

static void createFiles() {
	char func[] = "func.dat";
	char first[] = "first.dat";
	char second[] = "second.dat";
	char third[] = "third.dat";
	std::ofstream ofsFunc(func, std::ios::binary);
	std::ofstream ofs1(first, std::ios::binary);
	std::ofstream ofs2(second, std::ios::binary);
	std::ofstream ofs3(third, std::ios::binary);

	unsigned short N = 3;
	unsigned short T = 3;

	ofsFunc.write(reinterpret_cast<const char*>(&N), sizeof(N));
	ofsFunc.write(reinterpret_cast<const char*>(&T), sizeof(T));
	ofsFunc.write(reinterpret_cast<const char*>(first), sizeof(char) * 10);
	ofsFunc.write(reinterpret_cast<const char*>(second), sizeof(char) * 11);
	ofsFunc.write(reinterpret_cast<const char*>(third), sizeof(char) * 10);

	unsigned short N1 = 7, T1 = 0;
	int firstNums[] = { 0,1,2,3,5,6,7,0,3,3,3,4,4,0 };
	ofs1.write(reinterpret_cast<const char*>(&N1), sizeof(N1));
	ofs1.write(reinterpret_cast<const char*>(&T1), sizeof(T1));
	ofs1.write(reinterpret_cast<const char*>(firstNums), sizeof(int) * 14);

	unsigned short N2 = 2, T2 = 1;
	int secondNums[] = { 3,5 };
	ofs2.write(reinterpret_cast<const char*>(&N2), sizeof(N2));
	ofs2.write(reinterpret_cast<const char*>(&T2), sizeof(T2));
	ofs2.write(reinterpret_cast<const char*>(secondNums), sizeof(int) * 2);

	unsigned short N3 = 4, T3 = 2;
	int thirdNums[] = { 0,5,6,7 };
	ofs3.write(reinterpret_cast<const char*>(&N3), sizeof(N3));
	ofs3.write(reinterpret_cast<const char*>(&T3), sizeof(T3));
	ofs3.write(reinterpret_cast<const char*>(thirdNums), sizeof(int) * 4);

	ofsFunc.close();
	ofs1.close();
	ofs2.close();
	ofs3.close();
}

static void run() {
	cout << "Write the name of the file you want to create a function from: ";
	MyString file;
	cin >> file;

	PartialFunction* func = factory(file.c_str());

	cout << "Choose what you would like to do:" << endl;
	cout << "\t1) generate all values for arguments in range [a;b]" << endl;
	cout << "\t2) generate all values where the function is defined" << endl;

	/*int choice = 0;
	cin >> choice;*/

	//Only implementing 1) for testing
	int a = 0, b = 10;
	//cin >> a >> b;

	for (int i = a; i <= b; i++)
	{
		try
		{
			int value = (*func)(i);
			cout << "f(" << i << ") = " << value << endl;
		}
		catch (const std::exception& e) { cout << e.what() << endl; }
		catch (...) { }
	}

	/*switch (choice)
	{
	case 1:
	case 2:
	}*/

	delete func;
}

int main()
{
	//createFiles();
	run();
}