#include <iostream>
#include <fstream>
#include <exception>
#include <limits>
#include "PartialFunction.h"
#include "FunctionFactory.h"
#include "MyString.h"

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

	unsigned short N = 3, T = 3;

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

const static char SHOW_NEXT_VALUE = 's', GO_BACK = 'b';

static void resetCin() {
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static MyString getFileName() {
	cout << "Write the name of the file you want to create a function from: ";
	MyString file;
	cin >> file;
	resetCin();

	return file;
}

static void promptModeChoice() {
	cout << endl << "Choose what you would like to do:" << endl;
	cout << "\t1) generate all values for arguments in range [a;b]" << endl;
	cout << "\t2) generate all values where the function is defined" << endl;
	cout << "\t3) exit program" << endl;
}

static int getMode() {
	cout << "Choose mode: ";
	int choice = 0;
	while (choice != 1 && choice != 2 && choice != 3)
	{
		cin >> choice;
	}

	return choice;
}

static void runMode1(PartialFunction* func) {
	cout << "Choose which range to display values for(only where function is defined): ";
	int a, b;
	cin >> a >> b;

	int nonDefinedCount = 0;
	for (int i = a; i <= b; i++)
	{
		try
		{
			int value = (*func)(i);
			cout << "f(" << i << ") = " << value << endl;
		}
		catch (const std::logic_error& e) { nonDefinedCount++; }
	}

	if (nonDefinedCount == b - a + 1) {
		cout << "Function isn't defined anywhere in this range";
	}

	cout << endl;
}

static char mode2Wait() {
	char ch = NULL;
	while (ch != SHOW_NEXT_VALUE && ch != GO_BACK)
	{
		cin >> ch;
	}

	return ch;
}

static void runMode2(PartialFunction* func) {
	for (int64_t i = INT_MIN; i <= INT_MAX; i++) // takes a while
	{
		try
		{
			int value = (*func)(i);
			cout << "Show next value(s) or go back to menu(b)?" << endl;

			switch (mode2Wait())
			{
			case SHOW_NEXT_VALUE: break;
			case GO_BACK: return;
			}

			cout << "f(" << i << ") = " << value << endl;
		}
		catch (const std::logic_error& e) {  }
	}
}

static PartialFunction* getFunction() {
	PartialFunction* func = nullptr;

	while (!func) {
		try
		{
			MyString file = getFileName();
			func = factory(file.c_str());
		}
		catch (std::runtime_error& e) {}
	}

	return func;
}

static void doFunnyStuff(PartialFunction* func) {
	bool loop = true;
	while (loop)
	{
		promptModeChoice();
		int mode = getMode();

		switch (mode)
		{
		case 1: runMode1(func);
			break;
		case 2: runMode2(func);
			break;
		case 3: loop = false;
			break;
		}
	}
}

static void run() {
	PartialFunction* func = getFunction();

	doFunnyStuff(func);

	delete func;
}

int main()
{
	//createFiles();
	run();
}
