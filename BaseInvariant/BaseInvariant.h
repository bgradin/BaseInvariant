#pragma once
#include <vector>
using namespace std;

class BaseInvariant
{
	int m_base, m_decimalPosition;
	vector<int> m_data;
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
	BaseInvariant(int value)
	{
		m_base = 10;
		m_isNegative = value < 0;
		m_decimalPosition = 0;
		
		do
		{
			m_decimalPosition++;
			m_data.push_back(value % 10);
			value /= 10;
		} while (value != 0);
	}
};