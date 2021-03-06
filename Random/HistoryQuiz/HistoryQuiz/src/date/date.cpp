#include "date.h"
#include <iostream>

Date::Date(unsigned int year, unsigned int month, unsigned int day)
	: m_year(year), m_month(month), m_day(day), m_dateType(YEAR_MONTH_DAY)
{

}
Date::Date(unsigned int year, unsigned int month)
	: m_year(year), m_month(month), m_dateType(YEAR_MONTH)
{

}
Date::Date(unsigned int year)
	: m_year(year), m_dateType(YEAR)
{

}
Date::Date()
{

}

Date::~Date()
{
	
}

void Date::PrintDate(void) const
{
	switch (m_dateType)
	{
	case(YEAR_MONTH_DAY): std::cout << m_day << '/';
	case(YEAR_MONTH): std::cout << m_month << '/';
	case(YEAR): std::cout << m_year;
	}
}

const bool Date::operator==(Date &other) const
{
	if (other.Year() == this->m_year && other.Month() == this->m_month && other.Day() == this->m_day) return true;
	return false;
}

const unsigned int Date::Day() const
{
	return m_day;
}
const unsigned int Date::Month() const
{
	return m_month;
}
const unsigned int Date::Year() const
{
	return m_year;
}
const DateType Date::GetType() const
{
	return m_dateType;
}