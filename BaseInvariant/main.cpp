#include <iostream>
#include "BaseInvariant.h"
using namespace std;

void main()
{
	// Store numbers with different bases, and with or without fractional parts
	BaseInvariant test1 = 100;
	BaseInvariant test2(10.25, 2);
	BaseInvariant test3(-234, 31);

	// Perform mathematical operations on numbers of different bases and/or data types
	test1 += test2;
	test2 -= 0.05;
	test2 *= 2;
	test3 /= 2;

	// Output numbers as a specific data type, or in their current base
	// - Numbers in a specific base will be outputted with spaces in between each digit.
	//   Otherwise, it's impossible to tell where one digit ends and the next begins
	cout << "test1(int)    = " << test1.toInt() << endl;
	cout << "test1(double) = " << test1.toDouble() << endl;
	cout << "test2         = " << test2 << endl;
	cout << "test3 - 13    = " << test3 - 13 << endl;
}