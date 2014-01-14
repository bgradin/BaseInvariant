#pragma once
#include <deque>
#include <istream>
#include <ostream>
using namespace std;

#pragma warning(push, 3)
#define TEMPLATE template<typename T>
#define DEFAULT_BASE 10
#define DEFAULT_PRECISION 15
#define base2(a) BaseInvariant(a, 2)
#define base10(a) BaseInvariant(a, 10)
#define base16(a) BaseInvariant(a, 16)

#define OPERATORS(type, operatorName, operationName, operation) \
	friend type operatorName(type lhs, const BaseInvariant& rhs) { return lhs operation (type) rhs; } \
	friend BaseInvariant operatorName(const BaseInvariant& lhs, type rhs) { return lhs.perform(operationName<double>, base10(rhs)); }
#define ALL_OPERATORS(operatorName, operationName, operation) \
	friend BaseInvariant operatorName(const BaseInvariant& lhs, const BaseInvariant& rhs) { return lhs.perform(operationName<double>, rhs); } \
	OPERATORS(short, operatorName, operationName, operation) \
	OPERATORS(int, operatorName, operationName, operation) \
	OPERATORS(long, operatorName, operationName, operation) \
	OPERATORS(double, operatorName, operationName, operation)
#define ADD_OPERATORS ALL_OPERATORS(operator+, add, +)
#define SUBTRACT_OPERATORS ALL_OPERATORS(operator-, subtract, -)
#define MULTIPLY_OPERATORS ALL_OPERATORS(operator*, multiply, *)
#define DIVIDE_OPERATORS ALL_OPERATORS(operator/, divide, *)

#define COPY_CONSTRUCTOR(type) BaseInvariant(type value, const int base = DEFAULT_BASE, const int precision = DEFAULT_PRECISION)\
	{\
		construct<type>(value, base, precision);\
	}

class BaseInvariant
{
	unsigned int m_base, m_maximumPrecision, m_decimalPosition;
	deque<int> m_data;
	bool m_isNegative;

	TEMPLATE static T add(T a, T b) { return a + b; }
	TEMPLATE static T subtract(T a, T b) { return a - b; }
	TEMPLATE static T multiply(T a, T b) { return a * b; }
	TEMPLATE static T divide(T a, T b) { return a / b; }

	TEMPLATE BaseInvariant perform(T (*operation)(T, T), const BaseInvariant& rhs) const
	{
		return BaseInvariant(operation((T) *this, (T) rhs), m_base, m_maximumPrecision);
	}
	TEMPLATE BaseInvariant& perform_assign(T (*operation)(T, T), const BaseInvariant& rhs)
	{
		return *this = operation((T) *this, (T) rhs);
	}
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

public:
	BaseInvariant()
	{
		m_base = DEFAULT_BASE;
		m_maximumPrecision = DEFAULT_PRECISION;
		m_decimalPosition = 1;
		m_data.push_back(0);
		m_isNegative = false;
	}
	BaseInvariant(const BaseInvariant& other)
	{
		m_base = other.m_base;
		m_maximumPrecision = other.m_maximumPrecision;
		m_decimalPosition = other.m_decimalPosition;
		m_data = other.m_data;
		m_isNegative = other.m_isNegative;
	}
	COPY_CONSTRUCTOR(short);
	COPY_CONSTRUCTOR(int);
	COPY_CONSTRUCTOR(long);
	COPY_CONSTRUCTOR(double);

	void setBase(const int newBase)
	{
		construct<double>((double) *this, newBase, m_maximumPrecision);
	}
	unsigned int getBase() const
	{
		return m_base;
	}
	void setPrecision(const int newPrecision)
	{
		m_maximumPrecision = newPrecision;

		for (unsigned int i = 0; i < newPrecision - (m_data.size() - m_decimalPosition); i++)
			m_data.push_back(0);

		for (unsigned int i = 0; i < (m_data.size() - m_decimalPosition) - newPrecision; i++)
			m_data.pop_back();
	}
	unsigned int getPrecision() const
	{
		return m_maximumPrecision;
	}

	//// Operators ////
	// Conversion
	operator short() const { return cast<short>(); }
	operator int() const { return cast<int>(); }
	operator long() const { return cast<long>(); }
	operator double() const { return cast<double>(true); }

	// Logic
	bool operator==(const BaseInvariant& rhs) const
	{
		return m_data == rhs.m_data && m_base == rhs.m_base && m_isNegative == rhs.m_isNegative;
	}
	bool operator!=(const BaseInvariant& rhs) const
	{
		return !(*this == rhs);
	}

	// Math/assignment
	BaseInvariant& operator=(const BaseInvariant& rhs)
	{
		unsigned int baseBackup = m_base;

		m_base = rhs.m_base;
		m_data = rhs.m_data;
		m_isNegative = rhs.m_isNegative;
		m_maximumPrecision = rhs.m_maximumPrecision;
		m_decimalPosition = rhs.m_decimalPosition;

		if (m_base != baseBackup)
			setBase(baseBackup);

		return *this;
	}
	ADD_OPERATORS
	SUBTRACT_OPERATORS
	MULTIPLY_OPERATORS
	DIVIDE_OPERATORS
	BaseInvariant& operator+=(const BaseInvariant& rhs) { return perform_assign(add<BaseInvariant>, rhs); }
	BaseInvariant& operator-=(const BaseInvariant& rhs) { return perform_assign(subtract<BaseInvariant>, rhs); }
	BaseInvariant& operator*=(const BaseInvariant& rhs) { return perform_assign(multiply<BaseInvariant>, rhs); }
	BaseInvariant& operator/=(const BaseInvariant& rhs) { return perform_assign(divide<BaseInvariant>, rhs); }

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

		for (unsigned int i = 1; i < instance.m_data.size(); i++)
			outputStream << (i == instance.m_decimalPosition ? " . " : " ") << instance.m_data[i];

		return outputStream;
	}
};

#undef OPERATORS
#undef ALL_OPERATORS
#undef ADD_OPERATORS
#undef SUBTRACT_OPERATORS
#undef MULTIPLY_OPERATORS
#undef DIVIDE_OPERATORS
#undef COPY_CONSTRUCTOR
#pragma warning(pop)