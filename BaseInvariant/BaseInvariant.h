#pragma once
#include <deque>
using namespace std;

class BaseInvariant
{
	unsigned int m_base, m_decimalPosition;
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
	BaseInvariant(int value, int base = 10)
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

	int toInt()
	{
		int returnValue = 0;

		for (unsigned int i = 0; i < m_data.size() && i < m_decimalPosition; i--)
			returnValue += m_data[i] * (int) pow(m_base, m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	double toDouble()
	{
		double returnValue = 0;

		for (unsigned int i = 0; i < m_data.size(); i++)
			returnValue += (double) m_data[i] * pow(m_base, m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	void changeBase(int newBase)
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

	bool operator==(BaseInvariant& rhs)
	{
		return m_data == rhs.m_data && m_base == rhs.m_base && m_isNegative == rhs.m_isNegative;
	}
	bool operator!=(BaseInvariant& rhs)
	{
		return !(*this == rhs);
	}
	BaseInvariant& operator=(BaseInvariant& rhs)
	{
		m_data = rhs.m_data;

		changeBase(m_base);

		return *this;
	}
	BaseInvariant operator+(BaseInvariant& rhs)
	{
		return BaseInvariant((int) (toDouble() + rhs.toDouble()));
	}
	BaseInvariant operator-(BaseInvariant& rhs)
	{
		return BaseInvariant((int) (toDouble() - rhs.toDouble()));
	}
	BaseInvariant& operator+=(BaseInvariant& rhs)
	{
		return *this = *this + rhs;
	}
	BaseInvariant& operator-=(BaseInvariant& rhs)
	{
		return *this = *this - rhs;
	}
};