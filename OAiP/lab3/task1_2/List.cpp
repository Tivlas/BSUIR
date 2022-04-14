#include "List.h"

void List::add(const Product& product) {
	if (!anyFreeCells)
	{
		//next[productCount] = productCount + 1;
		next.push_back(productCount + 1);
		products.push_back(product);
		//products[productCount] = product;
		++productCount;
		++actualSize;
		return;
	}

	next[freeCells[freeCells.size() - 1] - 1] = freeCells[freeCells.size() - 1];
	products[freeCells[freeCells.size() - 1] - 1] = product;
	++actualSize;
	/*
	Например:
	next = {1,2,4,4,5}, значит третья ячейка(если считать с единицы) products свободна
	freeCells = {3};
	next[freeCells[freeCells.size() - 1] - 1] = freeCells[freeCells.size() - 1];
	freeCells[freeCells.size() - 1] - 1 = 3 - 1 = 2
	freeCells[freeCells.size() - 1] = 3;
	next[2] = 3;
	теперь next = {1,2,3,4,5};
	все ячейки заняты
	products[freeCells[freeCells.size() - 1] - 1] ~ products[2] = product;
	*/
	freeCells.pop_back();
	if (freeCells.size() == 0) {
		anyFreeCells = false;
	}
}

int List::find(QString name) {
	for (int i = next[0]; i <= next.size();) {
		if (products[i - 1].getName() == name) {
			return i;
		}
		i = next[i];
	}
	return -1; // если нет товара с таким названием
}

void List::remove(QString name) {
	int index = find(name);
	if (index != -1) {
		if (products[index - 1].getNumberOfItems() > 1) {
			products[index - 1].setNumberOfItems(products[index - 1].getNumberOfItems() - 1);
			return;
		}
		for (int i = 0; i < next.size(); ++i) {
			if (next[i] == index) {
				next[i] = next[index];
				--actualSize;
			}
		}
		anyFreeCells = true;
		freeCells.push_back(index);
	}
	else {
		throw QString("Товара с таким названием нет в списке");
	}
}

void List::removeAll(QString name) {
	int index = find(name);
	if (index != -1) {
		for (int i = 0; i < next.size(); ++i) {
			if (next[i] == index) {
				next[i] = next[index];
				--actualSize;
			}
		}
		anyFreeCells = true;
		freeCells.push_back(index);
	}
	else {
		throw QString("Товаров с таким названием нет в списке");
	}
}

QString List::print() {
	QString result = "";
	for (int i = next[0]; i <= next.size();) {
		result += products[i - 1].getInfo();
		i = next[i];
	}
	return result;
}

QString List::printByProductName(QString name) {
	QString result = "";
	for (int i = next[0]; i < next.size();) {
		if (products[i - 1].getName() == name) {
			result += products[i - 1].getInfo();
		}
		i = next[i];
	}
	if (result == "") {
		throw "Нет таких товаров";
	}
	return result;
}

QString List::printProductsWithLowerPrice(int price) {
	QString result = "";
	for (int i = next[0]; i < next.size();) {
		if (products[i - 1].getPrice() < price) {
			result += products[i - 1].getInfo();
		}
		i = next[i];
	}
	if (result == "") {
		throw "Нет таких товаров";
	}
	return result;
}

QString List::printByStorageTime(int storageDays) {
	QString result = "";
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	int todaysYear = local->tm_year + 1900;
	int todaysMonth = local->tm_mon + 1;
	int todaysDay = local->tm_mday;

	for (int i = next[0]; i < next.size();) {
		int dur = duration(products[i - 1].day, products[i - 1].month, products[i - 1].year, todaysDay, todaysMonth, todaysYear);
		if (dur > storageDays) {
			result += products[i - 1].getInfo();
		}
		i = next[i];
	}
	if (result == "") {
		throw "Нет таких товаров";
	}
	return result;
}

bool dateCompare(int day1, int month1, int year1, int day2, int month2, int year2) {
	QString first_year;
	first_year.setNum(year1);
	QString first_month;
	first_month.setNum(month1);
	QString first_day;
	first_day.setNum(day1);
	if (month1 / 10 == 0) {
		first_month.prepend('0');
	}
	if (day1 / 10 == 0) {
		first_day.prepend('0');
	}
	if (year1 < 10)
	{
		first_year.prepend("000");
	}
	else if (year1 < 100)
	{
		first_year.prepend("00");
	}
	else if (year1 < 1000)
	{
		first_year.prepend('0');
	}
	QString full_first_date = first_year + first_month + first_day;


	QString second_year;
	second_year.setNum(year2);
	QString second_month;
	second_month.setNum(month2);
	QString second_day;
	second_day.setNum(day2);

	if (month2 / 10 == 0) {
		second_month.prepend('0');
	}
	if (day2 / 10 == 0) {
		second_day.prepend('0');
	}
	if (year2 < 10)
	{
		second_year.prepend("000");
	}
	else if (year2 < 100)
	{
		second_year.prepend("00");
	}
	else if (year2 < 1000)
	{
		second_year.prepend('0');
	}
	QString full_second_date = second_year + second_month + second_day;

	bool cmp = (full_second_date >= full_first_date) ? 1 : 0;
	return cmp;
}

int daysPassed(int day, int month, int year) {
	int leapYear[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int notLeapYear[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (month == 1)
	{
		return day;
	}
	else
	{
		int daysPassed = -1; // number of days that have passed until the current month
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		{
			for (int i = 0; i < month - 1; i++)
			{
				daysPassed += leapYear[i];
			}
		}
		else
		{
			for (int i = 0; i < month - 1; i++)
			{
				daysPassed += notLeapYear[i];
			}
		}

		return daysPassed + day;
	}
}

int duration(int day1, int month1, int year1, int day2, int month2, int year2) {
	// second date is current system date
	if (dateCompare(day1, month1, year1, day2, month2, year2))
	{
		int daysBetweenYears = 0; // 2019-2023(2020+2021+2022)
		int untilNextYear = 0; // from current date to next year
		int days_passed = 0; // from the beginning of the year until given date
		days_passed = daysPassed(day2, month2, year2);

		if (year2 - year1 >= 1)
		{
			if ((year1 % 4 == 0 && year1 % 100 != 0) || year1 % 400 == 0) {
				untilNextYear = 366 - daysPassed(day1, month1, year1);
			}
			else {
				untilNextYear = 365 - daysPassed(day1, month1, year1);
			}

			for (int i = year1 + 1; i < year2; i++)
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
			untilNextYear = daysPassed(day1, month1, year1);
			int res = abs(untilNextYear - days_passed);
			return res;
		}

	}
	else
	{
		return duration(day2, month2, year2, day1, month1, year1);
	}
}