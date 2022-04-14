#pragma once
#include <QString>
class Product
{
	friend class List;
private:
	QString name = "";
	int price = 0;
	int day = 1;
	int month = 1;
	int year = 2022;  // дата поступления товара
	int numberOfItems = 0; // количество штук определенного товара
public:
	Product() {}
	Product(QString name, int price, int day, int month, int year, int numberOfItems);

	QString getName() const {
		return name;
	}
	int getPrice() const {
		return price;
	}
	void setName(QString name) {
		this->name = name;
	}
	void setPrice(int price) {
		this->price = price;
	}
	int getDay() const {
		return day;
	}
	int getMonth() const {
		return month;
	}
	int getYear() const {
		return year;
	}
	void setDay(int day) {
		this->day = day;
	}
	void setMonth(int month) {
		this->month = month;
	}
	void setYear(int year) {
		this->year = year;
	}
	int getNumberOfItems() const {
		return numberOfItems;
	}
	void setNumberOfItems(int numberOfItems) {
		this->numberOfItems = numberOfItems;
	}

	QString dateToString() const; // in 01.05.2022 format
	QString dateToStringWithoutDot() const; // in 20220501 format

	QString getInfo();
};


