#include "Employee.h"

Employee::Employee(int departmentNumber, QString position, QString name, QString surname, QString patronymic, short s_day, short s_month, short s_year):
	departmentNumber(departmentNumber), position(position), name(name),
	surname(surname), patronymic(patronymic), s_day(s_day), s_month(s_month), s_year(s_year){}

QString Employee::startDateToString() const
{
	QString year;
	year.setNum(this->s_year);
	QString month;
	month.setNum(this->s_month);
	QString day;
	day.setNum(this->s_day);

	if (this->s_month / 10 == 0) {
		month.prepend('0');
	}
	if (this->s_day / 10 == 0) {
		day.prepend('0');
	}
	if (this->s_year < 10)
	{
		year.prepend("000");
	}
	else if (this->s_year < 100)
	{
		year.prepend("00");
	}
	else if (this->s_year < 1000)
	{
		year.prepend('0');
	}
	QString start_date = day + '.' + month + '.' + year;
	return start_date;
}

QString Employee::startDateToStringWithoutDot() const
{
	QString year;
	year.setNum(this->s_year);
	QString month;
	month.setNum(this->s_month);
	QString day;
	day.setNum(this->s_day);

	if (this->s_month / 10 == 0) {
		month.prepend('0');
	}
	if (this->s_day / 10 == 0) {
		day.prepend('0');
	}
	if (this->s_year < 10)
	{
		year.prepend("000");
	}
	else if (this->s_year < 100)
	{
		year.prepend("00");
	}
	else if (this->s_year < 1000)
	{
		year.prepend('0');
	}
	QString start_date = year + month + day;
	return start_date;
}

QString Employee::getAllInformation() const
{
	QString info = "";
	info += "Name: " + this->name + "\n";
	info += "Surname: " + this->surname + "\n";
	info += "Patronymic: " + this->patronymic + "\n";
	info += "Date of employment: " + this->startDateToString() + "\n";
	info += "Department number: " + QString::number(this->departmentNumber) + "\n";
	info += "Position: " + this->position + "\n\n";
	return info;
}
