#pragma once
#include <deque>
#include <istream>
#include <ostream>
using namespace std;

#ifndef UINT
typedef unsigned int UINT;
#endif

class BaseInvariant
{
	UINT m_base, m_decimalPosition;
	deque<int> m_data;
	bool m_isNegative;

public:
	BaseInvariant()
	{
		m_base = 10;
		m_decimalPosition = 1;
		m_data.push_back(0);
		m_isNegative = false;
	}
	BaseInvariant(const BaseInvariant& other)
	{
		m_base = other.m_base;
		m_decimalPosition = other.m_decimalPosition;
		m_data = other.m_data;
		m_isNegative = other.m_isNegative;
	}
	BaseInvariant(int value, const int base = 10)
	{
		m_base = base;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		
		do
		{
			m_decimalPosition++;
			m_data.push_front(value % m_base);
			value /= m_base;
		} while (value != 0);
	}
	BaseInvariant(long value, const int base = 10)
	{
		m_base = base;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		
		do
		{
			m_decimalPosition++;
			m_data.push_front(value % m_base);
			value /= m_base;
		} while (value != 0);
	}

	int toInt() const
	{
		int returnValue = 0;

		for (UINT i = 0; i < m_data.size() && i < m_decimalPosition; i--)
			returnValue += m_data[i] * (int) pow(m_base, m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	long toLong() const
	{
		long returnValue = 0;

		for (UINT i = 0; i < m_data.size() && i < m_decimalPosition; i--)
			returnValue += (long) (m_data[i] * (int) pow(m_base, m_decimalPosition - i - 1));

		return m_isNegative ? -returnValue : returnValue;
	}
	double toDouble() const
	{
		double returnValue = 0;

		for (UINT i = 0; i < m_data.size(); i++)
			returnValue += (double) m_data[i] * pow(m_base, m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	void setBase(const int newBase)
	{
		double asDouble = abs(toDouble());
		int asInt = (int) floor(asDouble);
		double decimalPart = asDouble - (double) asInt;

		m_data.clear();
		m_base = newBase;

		do
		{
			m_data.push_front(asInt % m_base);
			asInt /= m_base;
		} while (asInt != 0);

		double currentPower = 1.0 / m_base;
		m_decimalPosition = m_data.size();

		do
		{
			if (decimalPart > 0)
				m_data.push_back((int) floor(decimalPart /= currentPower /= m_base));
		} while (decimalPart > 0);
	}
	UINT getBase() const
	{
		return m_base;
	}

	bool operator==(const BaseInvariant& rhs) const
	{
		return m_data == rhs.m_data && m_base == rhs.m_base && m_isNegative == rhs.m_isNegative;
	}
	bool operator==(const int& rhs) const
	{
		return toInt() == rhs;
	}
	bool operator==(const long& rhs) const
	{
		return toLong() == rhs;
	}
	bool operator!=(const BaseInvariant& rhs) const
	{
		return !(*this == rhs);
	}
	bool operator!=(const int& rhs) const
	{
		return !(*this == rhs);
	}
	bool operator!=(const long& rhs) const
	{
		return !(*this == rhs);
	}
	BaseInvariant& operator=(const BaseInvariant& rhs)
	{
		m_data = rhs.m_data;

		setBase(m_base);

		return *this;
	}
	BaseInvariant& operator=(const int& rhs)
	{
		return *this = BaseInvariant(rhs);
	}
	BaseInvariant& operator=(const long& rhs)
	{
		return *this = BaseInvariant(rhs);
	}
	BaseInvariant operator+(const BaseInvariant& rhs) const
	{
		return BaseInvariant((int) (toDouble() + rhs.toDouble()));
	}
	int operator+(const int& rhs) const
	{
		return toInt() + rhs;
	}
	long operator+(const long& rhs) const
	{
		return toLong() + rhs;
	}
	BaseInvariant operator-(const BaseInvariant& rhs) const
	{
		return BaseInvariant((int) (toDouble() - rhs.toDouble()));
	}
	int operator-(const int& rhs)
	{
		return toInt() - rhs;
	}
	long operator-(const long& rhs)
	{
		return toLong() - rhs;
	}
	BaseInvariant& operator+=(const BaseInvariant& rhs)
	{
		return *this = *this + rhs;
	}
	BaseInvariant& operator+=(const int& rhs)
	{
		return *this = *this + rhs;
	}
	BaseInvariant& operator+=(const long& rhs)
	{
		return *this = *this + rhs;
	}
	BaseInvariant& operator-=(const BaseInvariant& rhs)
	{
		return *this = *this - rhs;
	}
	BaseInvariant& operator-=(const int& rhs)
	{
		return *this = *this - rhs;
	}
	BaseInvariant& operator-=(const long& rhs)
	{
		return *this = *this + rhs;
	}
	friend istream& operator>>(istream& inputStream, BaseInvariant& instance)
	{
		int value;
		inputStream >> value;

		instance = BaseInvariant(value);

		return inputStream;
	}
	friend ostream& operator<<(ostream& outputStream, const BaseInvariant& instance)
	{
		outputStream << instance.m_data[0];

		for (UINT i = 1; i < instance.m_data.size(); i++)
			outputStream << " " << instance.m_data[i];

		return outputStream;
	}
};