#include "task3.h"



Date::Date(int day, int month, int year) : m_day(day), m_month(month), m_year(year)
{
}


Date Date::NextDay() const
{
	if (m_day == 31 && m_month == 12) // end of the year
	{
		Date next(1, 1, m_year + 1);
		return next;
	}
	else if (m_month == 2 && (m_day == 28 && !isLeap() || m_day == 29 && isLeap())) // February
	{
		Date next(1, m_month + 1, m_year);
		return next;
	}
	else if (m_day == 30 && !DaysInMonth(m_month) || m_day == 31) // 30 or 31 in the current month
	{
		Date next(1, m_month + 1, m_year);
		return next;
	}
	else
	{
		Date next(m_day + 1, m_month, m_year);
		return next;
	}
}

Date Date::PreviousDay() const
{
	if (m_day == 1 && m_month == 1) // beginning of the year
	{
		Date prev(31, 12, m_year - 1);
		return prev;
	}
	else if (m_day == 1 && m_month == 3 && !isLeap()) // February, current year is not leap
	{
		Date prev(28, m_month - 1, m_year);
		return prev;
	}
	else if (m_day == 1 && m_month == 3 && isLeap()) //  February, current year is leap
	{
		Date prev(29, m_month - 1, m_year);
		return prev;
	}
	else if (m_day == 1 && !DaysInMonth(m_month - 1)) // did the previous month have 30 days?
	{
		Date prev(30, m_month - 1, m_year);
		return prev;
	}
	else if (m_day == 1 && DaysInMonth(m_month - 1)) // did the previous month have 31 days?
	{
		Date prev(31, m_month - 1, m_year);
		return prev;
	}
	else
	{
		Date prev(m_day - 1, m_month, m_year);
		return prev;
	}
}

bool Date::isLeap() const
{
	if ((m_year % 4 == 0 && m_year % 100 != 0) || m_year % 400 == 0)
	{
		return 1;
	}
	else return 0;
}

bool Date::DaysInMonth(int x) const
{
	if (leapYear[x] == 31) {
		return 1;
	}
	else return 0;
}

int Date::daysPassed() const
{
	if (m_month == 1)
	{
		return m_day;
	}
	else
	{
		int daysPassed = -1; // number of days that have passed until the current month
		if (this->isLeap())
		{
			for (int i = 0; i < m_month - 1; i++)
			{
				daysPassed += leapYear[i];
			}
		}
		else
		{
			for (int i = 0; i < m_month - 1; i++)
			{
				daysPassed += notLeapYear[i];
			}
		}

		return daysPassed + m_day;
	}
}

short Date::WeekNumber() const
{
	int number = 1;
	int days = this->daysPassed();
	double w_number = (double)(days) / 7;

	if (days % 7 != 0)
	{
		number = w_number + 1;
	}
	else
	{
		number = w_number;
	}

	if (number == 0)
	{
		return 1;
	}
	return number;
}

int Date::DaysTillBirthday(Date& birthday) const
{
	return this->Duration(birthday);
}

int Date::Duration(const Date& givenDate) const
{

	if (this->dateCompare(givenDate))
	{
		int daysBetweenYears = 0; // 2019-2023(2020+2021+2022)
		int untilNextYear = 0; // from current date to next year
		int days_passed = 0; // from the beginning of the year till given date
		days_passed = givenDate.daysPassed();

		if (givenDate.m_year - this->m_year >= 1)
		{
			if (this->isLeap()) {
				untilNextYear = 366 - this->daysPassed();
			}
			else {
				untilNextYear = 365 - this->daysPassed();
			}

			for (int i = this->m_year + 1; i < givenDate.m_year; i++)
			{
				if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
				{
					daysBetweenYears += 366;
				}
				else
				{
					daysBetweenYears += 365;
				}
			}
			int res = untilNextYear + days_passed + daysBetweenYears;
			return res;
		}
		else {
			untilNextYear = this->daysPassed();
			int res = abs(untilNextYear - days_passed);
			return res;
		}

	}
	else 
	{
		return givenDate.Duration(*this);
	}
}

bool Date::dateCompare(const Date& givenDate) const
{
	QString cur_year;
	cur_year.setNum(this->m_year);
	QString cur_month;
	cur_month.setNum(this->m_month);
	QString cur_day;
	cur_day.setNum(this->m_day);

	if (this->m_month / 10 == 0) {
		cur_month.prepend('0');
	}
	if (this->m_day / 10 == 0) {
		cur_day.prepend('0');
	}
	if (this->m_year < 10)
	{
		cur_year.prepend("000");
	}
	else if (this->m_year < 100)
	{
		cur_year.prepend("00");
	}
	else if (this->m_year < 1000)
	{
		cur_year.prepend('0');
	}
	QString full_cur_date = cur_year + cur_month + cur_day;
	//int full_cur_date_int = full_cur_date.toInt();

	////////////////////////////////////////////////////////////////

	QString given_year;
	given_year.setNum(givenDate.m_year);
	QString given_month;
	given_month.setNum(givenDate.m_month);
	QString given_day;
	given_day.setNum(givenDate.m_day);

	if (givenDate.m_month / 10 == 0) {
		given_month.prepend('0');
	}
	if (givenDate.m_day / 10 == 0) {
		given_day.prepend('0');
	}
	if (givenDate.m_year < 10)
	{
		given_year.prepend("000");
	}
	else if (givenDate.m_year < 100)
	{
		given_year.prepend("00");
	}
	else if (givenDate.m_year < 1000)
	{
		given_year.prepend('0');
	}
	QString full_given_date = given_year + given_month + given_day;
	//int full_given_date_int = full_given_date.toInt();

	bool cmp = (full_given_date >= full_cur_date) ? 1 : 0;
	return cmp;
}

QTableWidgetItem* Date::dateItem() const
{
	QString day = QString::number(m_day);
	QString month = QString::number(m_month);
	QString year = QString::number(m_year);
	if (m_day < 10)
	{
		day.prepend('0');
	}
	if (m_month < 10)
	{
		month.prepend('0');
	}
	if (m_year < 10)
	{
		year.prepend("000");
	}
	else if (m_year < 100)
	{
		year.prepend("00");
	}
	else if (m_year < 1000)
	{
		year.prepend('0');
	}

	QTableWidgetItem* item = new QTableWidgetItem(QString("%1.%2.%3").arg(day).arg(month).arg(year));
	return item;
}

QString Date::dateToString() const
{
	QString year;
	year.setNum(this->m_year);
	QString month;
	month.setNum(this->m_month);
	QString day;
	day.setNum(this->m_day);

	if (this->m_month / 10 == 0) {
		month.prepend('0');
	}
	if (this->m_day / 10 == 0) {
		day.prepend('0');
	}
	if (this->m_year < 10)
	{
		year.prepend("000");
	}
	else if (this->m_year < 100)
	{
		year.prepend("00");
	}
	else if (this->m_year < 1000)
	{
		year.prepend('0');
	}
	QString full_date = day + '.' + month + '.' + year;
	return full_date;
}


