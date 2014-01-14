#pragma once
#include <deque>
#include <istream>
#include <ostream>
#include <functional>
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
	#define TEMPLATE template<typename T>

	#define RELATIONAL_OPERATOR(operation) bool operator##operation(const BaseInvariant& rhs)\
		{ return (double) *this operation (double) rhs; }

	#define OPERATORS(type, operation)\
		friend type operator##operation(type lhs, const BaseInvariant& rhs) { return lhs operation (type) rhs; }\
		friend BaseInvariant operator##operation(const BaseInvariant& lhs, type rhs) { return lhs operation BaseInvariant(rhs); }

	#define ALL_OPERATORS(operation)\
		friend BaseInvariant operator##operation(const BaseInvariant& lhs, const BaseInvariant& rhs)\
			{ return BaseInvariant((double) lhs operation (double) rhs, lhs.m_base); }\
		OPERATORS(short, operation)\
		OPERATORS(int, operation)\
		OPERATORS(long, operation)\
		OPERATORS(double, operation)

	#define COPY_CONSTRUCTOR(type)\
		BaseInvariant(type value, const int base = STANDARD_BASE, const int precision = STANDARD_PRECISION)\
		{ construct<type>(value, base, precision); }

	TEMPLATE T cast(bool hasDecimals = false) const
	{
		T returnValue = 0;

		for (int i = 0; i < (int) m_data.size() && (hasDecimals ? true : i < (int) m_decimalPosition); i++)
			returnValue += (T) m_data[i] * (T) pow(m_base, (int) m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	TEMPLATE void construct(T value, int base, int precision)
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

		for (UINT i = 0; i < newPrecision - (m_data.size() - m_decimalPosition); i++)
			m_data.push_back(0);

		for (UINT i = 0; i < (m_data.size() - m_decimalPosition) - newPrecision; i++)
			m_data.pop_back();
	}
	UINT getPrecision() const
	{
		return m_maximumPrecision;
	}

	//// Operators ////
	// Conversion
	operator short() const { return cast<short>(); }
	operator int() const { return cast<int>(); }
	operator long() const { return cast<long>(); }
	operator double() const { return cast<double>(true); }

	// Logical
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
	ALL_OPERATORS(+)
	ALL_OPERATORS(-)
	ALL_OPERATORS(*)
	ALL_OPERATORS(/)
	BaseInvariant& operator+=(const BaseInvariant& rhs) { return *this = *this + rhs; }
	BaseInvariant& operator-=(const BaseInvariant& rhs) { return *this = *this - rhs; }
	BaseInvariant& operator*=(const BaseInvariant& rhs) { return *this = *this * rhs; }
	BaseInvariant& operator/=(const BaseInvariant& rhs) { return *this = *this / rhs; }

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

#undef TEMPLATE
#undef OPERATORS
#undef RELATIONAL_OPERATOR
#undef ALL_OPERATORS
#undef COPY_CONSTRUCTOR
#pragma warning(pop)