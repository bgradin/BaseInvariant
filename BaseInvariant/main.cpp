#include <iostream>
#include "BaseInvariant.h"
using namespace std;

void main()
{
	// Store numbers with different bases, with or without fractional parts, and with variable precision
	BaseInvariant test1 = 100;
	BaseInvariant test2 = base2(10.25);
	BaseInvariant test3 = baseN(-234, 31);
	BaseInvariant test4(10.0 / 3, 10, 7);
	BaseInvariant test5(100, 6);

	// Modify precision or base
	test4.setPrecision(4);
	test4.setBase(4);

	// Perform mathematical operations on numbers of different bases and/or data types
	test1 += test2;
	test2 -= 0.05;
	test2 *= 2;
	test3 /= 2;

	// Output numbers as a specific data type, or in their current base
	// - Numbers in a specific base will be outputted with spaces in between each digit.
	//   Otherwise, it's impossible to tell where one digit ends and the next begins
	cout << "test1(int)           = " << (int) test1 << endl;
	cout << "test1(double)        = " << (double) test1 << endl;
	cout << endl;
	cout << "test2 (base " << test2.getBase() << ")       = " << test2 << endl;
	cout << endl;
	cout << "test3 (base " << test3.getBase() << ")      = " << test3 << endl;
	cout << "test3 (base 10)      = " << (double) test3 << endl;
	cout << "test3 - 13 (base " << test3.getBase() << ") = " << test3 - 13 << endl;
	cout << "test3 - 13 (base 10) = " << (double) test3 - 13 << endl;
	cout << endl;
	cout << "test4 (base " << test4.getBase() << ")       = " << test4 << endl;
	cout << "test4 (base 10)      = " << (double) test4 << endl;
	cout << endl;
	cout << "test5 (base 6)       = " << test5 << endl;
	cout << "test5 % 21 (base 6)  = " << test5 % 21 << endl;
	cout << endl;
}