#include <iostream>
#include "BaseInvariant.h"
using namespace std;

void main()
{
	// Store doubles or integers
	BaseInvariant test1 = 100;
	BaseInvariant test2(10.25, 2);
	BaseInvariant test3(-234, 31);

	// Perform mathematical operations on numbers of different bases or data types
	test1 += test2;
	test2 -= 0.05;

	// Output numbers as a specific data type, or in their current base
	// - Numbers in a specific base will be outputted with spaces in between each digit.
	//   Otherwise, it's impossible to tell where one digit ends and the next begins
	cout << "test1(int)    = " << test1.toInt() << endl;
	cout << "test1(double) = " << test1.toDouble() << endl;
	cout << "test2         = " << test2 << endl;
	cout << "test3 - 16    = " << test3 - 16 << endl;
}