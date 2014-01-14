#pragma once
#include <deque>
#include <istream>
#include <ostream>
using namespace std;

#pragma warning(push, 3)

class BaseInvariant
{
	unsigned int m_base, m_maximumPrecision, m_decimalPosition;
	deque<int> m_data;
	bool m_isNegative;

public:
	BaseInvariant()
	{
		m_base = 10;
		m_maximumPrecision = 15;
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
	BaseInvariant(short value, const int base = 10, const int precision = 15)
	{
		m_base = base;
		m_maximumPrecision = precision;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		
		value = abs(value);

		do
		{
			m_decimalPosition++;
			m_data.push_front(value % m_base);
			value /= m_base;
		} while (value != 0);
	}
	BaseInvariant(int value, const int base = 10, const int precision = 15)
	{
		m_base = base;
		m_maximumPrecision = precision;
		m_isNegative = value < 0;
		m_decimalPosition = 0;

		value = abs(value);
		
		do
		{
			m_decimalPosition++;
			m_data.push_front(value % m_base);
			value /= m_base;
		} while (value != 0);
	}
	BaseInvariant(long value, const int base = 10, const int precision = 15)
	{
		m_base = base;
		m_maximumPrecision = precision;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		
		value = abs(value);

		do
		{
			m_decimalPosition++;
			m_data.push_front(value % m_base);
			value /= m_base;
		} while (value != 0);
	}
	BaseInvariant(double value, const int base = 10, const int precision = 15)
	{
		m_base = base;
		m_maximumPrecision = precision;
		m_isNegative = value < 0;
		m_decimalPosition = 0;

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

	short toShort() const
	{
		short returnValue = 0;

		for (int i = 0; i < (int) m_data.size() && i < (int) m_decimalPosition; i++)
			returnValue += (short) (m_data[i] * (int) pow(m_base, (int) m_decimalPosition - i - 1));

		return m_isNegative ? -returnValue : returnValue;
	}
	int toInt() const
	{
		int returnValue = 0;

		for (int i = 0; i < (int) m_data.size() && i < (int) m_decimalPosition; i++)
			returnValue += (int) (m_data[i] * pow(m_base, (int) m_decimalPosition - i - 1));

		return m_isNegative ? -returnValue : returnValue;
	}
	long toLong() const
	{
		long returnValue = 0;

		for (int i = 0; i < (int) m_data.size() && i < (int) m_decimalPosition; i++)
			returnValue += (long) (m_data[i] * pow(m_base, (int) m_decimalPosition - i - 1));

		return m_isNegative ? -returnValue : returnValue;
	}
	double toDouble() const
	{
		double returnValue = 0;

		for (int i = 0; i < (int) m_data.size(); i++)
			returnValue += ((double) m_data[i]) * pow(m_base, (int) m_decimalPosition - i - 1);

		return m_isNegative ? -returnValue : returnValue;
	}
	void setBase(const int newBase)
	{
		double asDouble = abs(toDouble());
		int asInt = (int) floor(asDouble);
		double decimalPart = asDouble - (double) asInt;

		m_data.clear();
		m_base = newBase;
		m_decimalPosition = 0;

		do
		{
			if (asInt != 0)
			{
				m_data.push_front(asInt % m_base);
				asInt /= m_base;
				m_decimalPosition++;
			}
			if (decimalPart > 0 && m_data.size() <= m_maximumPrecision)
			{
				m_data.push_back((int) floor(decimalPart /= 1.0 / m_base));
				decimalPart -= floor(decimalPart);
			}
		} while ((decimalPart > 0 && m_data.size() <= m_maximumPrecision) || asInt != 0);
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

	bool operator==(const BaseInvariant& rhs) const
	{
		return m_data == rhs.m_data && m_base == rhs.m_base && m_isNegative == rhs.m_isNegative;
	}
	bool operator!=(const BaseInvariant& rhs) const
	{
		return !(*this == rhs);
	}
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
	BaseInvariant operator+(const BaseInvariant& rhs) const
	{
		return BaseInvariant(toDouble() + rhs.toDouble(), m_base, m_maximumPrecision);
	}
	BaseInvariant operator-(const BaseInvariant& rhs) const
	{
		return BaseInvariant(toDouble() - rhs.toDouble(), m_base, m_maximumPrecision);
	}
	BaseInvariant operator*(const BaseInvariant& rhs)
	{
		return BaseInvariant(toDouble() * rhs.toDouble(), m_base, m_maximumPrecision);
	}
	BaseInvariant operator/(const BaseInvariant& rhs)
	{
		return BaseInvariant(toDouble() / rhs.toDouble(), m_base, m_maximumPrecision);
	}
	BaseInvariant& operator+=(const BaseInvariant& rhs)
	{
		return *this = *this + rhs;
	}
	BaseInvariant& operator-=(const BaseInvariant& rhs)
	{
		return *this = *this - rhs;
	}
	BaseInvariant& operator*=(const BaseInvariant& rhs)
	{
		return *this = *this * rhs;
	}
	BaseInvariant& operator/=(const BaseInvariant& rhs)
	{
		return *this = *this / rhs;
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
		if (instance.m_isNegative)
			outputStream << "- ";

		outputStream << instance.m_data[0];

		for (unsigned int i = 1; i < instance.m_data.size(); i++)
			outputStream << (i == instance.m_decimalPosition ? " . " : " ") << instance.m_data[i];

		return outputStream;
	}
};

#pragma warning(pop)