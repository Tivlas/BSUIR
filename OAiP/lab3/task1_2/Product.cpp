#include "Product.h"

Product::Product(QString name, int price, int day, int month, int year, int numberOfItems) : name(name), price(price), day(day), month(month), year(year), numberOfItems(numberOfItems)
{

}

QString Product::dateToString() const
{
	QString year;
	year.setNum(this->year);
	QString month;
	month.setNum(this->month);
	QString day;
	day.setNum(this->day);

	if (this->month / 10 == 0) {
		month.prepend('0');
	}
	if (this->day / 10 == 0) {
		day.prepend('0');
	}
	if (this->year < 10)
	{
		year.prepend("000");
	}
	else if (this->year < 100)
	{
		year.prepend("00");
	}
	else if (this->year < 1000)
	{
		year.prepend('0');
	}
	QString start_date = day + '.' + month + '.' + year;
	return start_date;
}

QString Product::dateToStringWithoutDot() const
{
	QString year;
	year.setNum(this->year);
	QString month;
	month.setNum(this->month);
	QString day;
	day.setNum(this->day);

	if (this->month / 10 == 0) {
		month.prepend('0');
	}
	if (this->day / 10 == 0) {
		day.prepend('0');
	}
	if (this->year < 10)
	{
		year.prepend("000");
	}
	else if (this->year < 100)
	{
		year.prepend("00");
	}
	else if (this->year < 1000)
	{
		year.prepend('0');
	}
	QString start_date = year + month + day;
	return start_date;
}

QString Product::getInfo()
{
	QString result;
	result = "Товар: " + name + "\n" + "Цена: " + QString::number(price) + "\n" + "Дата поступления на склад: " + dateToString() + "\n" + "Количество единиц товара: " + QString::number(numberOfItems) + "\n\n";
	return result;
}

QString Product::getInfoInInputFormat()
{
	QString result;
	result = name + "\n" + QString::number(price) + "\n" + dateToString() + "\n" + QString::number(numberOfItems) + "\n";
	return result;
}
