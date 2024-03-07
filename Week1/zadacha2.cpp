#include<iostream>
using std::cout;

enum Degree
{
	Bachelor,
	Masters,
	Doctor
};

struct Teacher
{
	unsigned id;
	Degree degree;
	unsigned failedStudents;
};

bool isIdValid(unsigned id)
{
	unsigned min = 10000;
	unsigned max = 99999;
	return (id >= min && id <= max);
}

Teacher createTeacher(unsigned newId, const Degree& newDegree, unsigned newFailedStudents)
{
	Teacher t = { 00000, Bachelor, 0 };
	if(isIdValid(newId))
	{
		t.id = newId;
		t.degree = newDegree;
		t.failedStudents = newFailedStudents;
		return t;
	}
	else
	{
		cout << "The data is not valid.\n\n";
		return t;
	}
}

void printDegreeFromIndex(int d)
{
	if (d == 0)
	{
		cout << "Bachelor";
	}
	else if (d == 1)
	{
		cout << "Master\'s";
	}
	else
	{
		cout << "Doctor";
	}
}

void printTeacher(const Teacher& t)
{
	if(isIdValid(t.id))
	{
		cout << "Id: " << t.id << '\n' << "Degree: ";
		printDegreeFromIndex(t.degree);
		cout <<'\n' << "Failed students: " << t.failedStudents << '\n';
	}
}

int main()
{
	Teacher t1 = createTeacher(23872, Masters, 54);
	Teacher t2 = createTeacher(5322, Doctor, 104);
	Teacher t3 = createTeacher(88097, Doctor, 39);

	printTeacher(t1);
	cout << '\n';
	printTeacher(t2);
	cout << '\n';
	printTeacher(t3);
	cout << '\n';
}
