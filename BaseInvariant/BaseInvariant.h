#pragma once
#include <deque>
#include <istream>
#include <ostream>
using namespace std;

#pragma warning(push, 3)

#define STANDARD_BASE 10
#define STANDARD_PRECISION 15

#ifndef UINT
typedef unsigned int UINT;
#endif

#define base2(a) BaseInvariant(a, 2)
#define base16(a) BaseInvariant(a, 16)
#define baseN(a, n) BaseInvariant(a, n)

class BaseInvariant
{
	#define RELATIONAL_OPERATOR(operation) bool operator##operation(const BaseInvariant& rhs)\
		{ return (double) *this operation (double) rhs; }

	#define CONVERSION_OPERATOR(type) operator type() const { return cast<type>(); }

	#define OPERATORS(type, operation)\
		friend type operator##operation(type lhs, const BaseInvariant& rhs) { return lhs operation (type) rhs; }\
		friend BaseInvariant operator##operation(const BaseInvariant& lhs, type rhs) { return lhs operation BaseInvariant(rhs); }

	#define MATH_OPERATORS(operation)\
		friend BaseInvariant operator##operation(const BaseInvariant& lhs, const BaseInvariant& rhs)\
			{ return BaseInvariant((double) lhs operation (double) rhs, lhs.m_base); }\
		OPERATORS(short, operation)\
		OPERATORS(int, operation)\
		OPERATORS(long, operation)\
		OPERATORS(double, operation)

	#define MODULUS_OPERATOR(type)\
		friend BaseInvariant operator%(const BaseInvariant& lhs, const type& rhs)\
		{ return lhs.modulus<type>(rhs); }

	#define MATH_ASSIGNMENT_OPERATOR(operation) BaseInvariant& operator##operation##=(const BaseInvariant& rhs) { return *this = *this operation rhs; }

	#define COPY_CONSTRUCTOR(type)\
		BaseInvariant(type value, const int base = STANDARD_BASE, const int precision = STANDARD_PRECISION)\
		{ construct<type>(value, base, precision); }

	template<typename T>
	BaseInvariant modulus(T rhs) const
	{
		double doubleRhs = (double) rhs;
		double doubleLhs = (double) *this;
		int nearestMultiple = (int) (floor(doubleLhs / doubleRhs) * doubleRhs);
		return BaseInvariant(doubleLhs - (double) nearestMultiple, m_base);
	}

	template<typename T>
	T cast() const
	{
		T returnValue = 0;

		for (int i = 0; i < (int) m_data.size(); i++)
			returnValue += (T) m_data[i] * (T) pow(m_base, (int) m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}

	template<typename T>
	void construct(T value, int base, int precision)
	{
		m_base = base;
		m_maximumPrecision = precision;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		m_data.clear();

		int integerPart = (int) floor(abs(value));
		double decimalPart = abs(value) - integerPart;

		do
		{
			if (integerPart != 0)
			{
				m_data.push_front(integerPart % m_base);
				integerPart /= m_base;
				m_decimalPosition++;
			}
			if (decimalPart > 0 && m_data.size() <= m_maximumPrecision)
			{
				m_data.push_back((int) floor(decimalPart /= 1.0 / m_base));
				decimalPart -= floor(decimalPart);
			}
		} while ((decimalPart > 0 && m_data.size() <= m_maximumPrecision) || integerPart != 0);
	}
	void construct(const BaseInvariant& other)
	{
		construct(other.m_base, other.m_maximumPrecision, other.m_decimalPosition, other.m_data, other.m_isNegative);
	}
	void construct(UINT base, UINT maximumPrecision, UINT decimalPosition, deque<int> data, bool isNegative)
	{
		m_base = base;
		m_maximumPrecision = maximumPrecision;
		m_decimalPosition = decimalPosition;
		m_data = data;
		m_isNegative = isNegative;
	}

	UINT m_base, m_maximumPrecision, m_decimalPosition;
	deque<int> m_data;
	bool m_isNegative;

public:
	BaseInvariant()
	{
		construct(STANDARD_BASE, STANDARD_PRECISION, 1, deque<int>(1, 0), false);
	}
	BaseInvariant(const BaseInvariant& other)
	{
		construct(other);
	}
	COPY_CONSTRUCTOR(short);
	COPY_CONSTRUCTOR(int);
	COPY_CONSTRUCTOR(long);
	COPY_CONSTRUCTOR(double);

	void setBase(const int newBase)
	{
		construct<double>((double) *this, newBase, m_maximumPrecision);
	}
	UINT getBase() const
	{
		return m_base;
	}
	void setPrecision(const int newPrecision)
	{
		m_maximumPrecision = newPrecision;

		// If newPrecision > currentPrecision
		int add = (int) m_maximumPrecision - ((int) m_data.size() - (int) m_decimalPosition);
		for (int i = 0; i < add; i++)
			m_data.push_back(0);

		// If newPrecision < currentPrecision
		int remove = ((int) m_data.size() - (int) m_decimalPosition) - (int) m_maximumPrecision;
		for (int i = 0; i < remove; i++)
			m_data.pop_back();
	}
	UINT getPrecision() const
	{
		return m_maximumPrecision;
	}

	//// Operators ////
	CONVERSION_OPERATOR(short)
	CONVERSION_OPERATOR(int)
	CONVERSION_OPERATOR(long)
	CONVERSION_OPERATOR(double)

	// Comparison
	bool operator==(const BaseInvariant& rhs) const
	{
		return m_data == rhs.m_data && m_base == rhs.m_base && m_isNegative == rhs.m_isNegative;
	}
	bool operator!=(const BaseInvariant& rhs) const
	{
		return !(*this == rhs);
	}

	// Relational
	RELATIONAL_OPERATOR(<);
	RELATIONAL_OPERATOR(>);
	RELATIONAL_OPERATOR(<=);
	RELATIONAL_OPERATOR(>=);

	// Math/assignment
	BaseInvariant& operator=(const BaseInvariant& rhs)
	{
		UINT baseBackup = m_base;

		construct(rhs.m_base, rhs.m_maximumPrecision, rhs.m_decimalPosition, rhs.m_data, rhs.m_isNegative);

		if (m_base != baseBackup)
			setBase(baseBackup);

		return *this;
	}
	MATH_OPERATORS(+)
	MATH_OPERATORS(-)
	MATH_OPERATORS(*)
	MATH_OPERATORS(/)
	MODULUS_OPERATOR(BaseInvariant)
	MODULUS_OPERATOR(short)
	MODULUS_OPERATOR(int)
	MODULUS_OPERATOR(long)
	MODULUS_OPERATOR(double)
	MATH_ASSIGNMENT_OPERATOR(+)
	MATH_ASSIGNMENT_OPERATOR(-)
	MATH_ASSIGNMENT_OPERATOR(*)
	MATH_ASSIGNMENT_OPERATOR(/)
	MATH_ASSIGNMENT_OPERATOR(%)
	BaseInvariant& operator++()
	{
		return *this = *this + 1;
	}
	BaseInvariant operator++(int num)
	{
		++*this;
		return BaseInvariant(num);
	}
	BaseInvariant& operator--()
	{
		return *this = *this - 1;
	}
	BaseInvariant operator--(int num)
	{
		++*this;
		return BaseInvariant(num);
	}

	// Stream
	friend istream& operator>>(istream& inputStream, BaseInvariant& instance)
	{
		int value;
		inputStream >> value;

		instance = BaseInvariant(value);

		return inputStream;
	}
	friend ostream& operator<<(ostream& outputStream, const BaseInvariant& instance)
	{
		if (instance.m_isNegative)
			outputStream << "- ";

		outputStream << instance.m_data[0];

		for (UINT i = 1; i < instance.m_data.size(); i++)
			outputStream << (i == instance.m_decimalPosition ? " . " : " ") << instance.m_data[i];

		return outputStream;
	}
};

#undef OPERATORS
#undef RELATIONAL_OPERATOR
#undef CONVERSION_OPERATOR
#undef MATH_OPERATORS
#undef MATH_ASSIGNMENT_OPERATOR
#undef COPY_CONSTRUCTOR
#pragma warning(pop)