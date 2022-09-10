#pragma once
#include <QString>
class Employee
{
	friend class List;
private:
	int departmentNumber;
	QString position;
	QString name;
	QString surname;
	QString patronymic;
	int s_day, s_month, s_year; // date of employment(start date)
	Employee* next = nullptr;
	Employee* prev = nullptr;
public:
	explicit Employee() {};
	Employee(int departmentNumber, QString position, QString name, QString surname, QString patronymic, short s_day, short s_month, short s_year);
	~Employee(){};
	size_t getDepartmentNumber() const { return departmentNumber; }
	QString getPosition() const { return position; }
	QString getName() const { return name; }
	QString getSurname() const { return surname; }
	QString getPatronymic() const { return patronymic; }
	
	int getS_day() const { return s_day; }
	int getS_month() const { return s_month; }
	int getS_year() const { return s_year; }
	
	void setDepartmentNumber(int num) {
		departmentNumber = num;	
	}
	void setPosition(QString pos) {
		position = pos;
	}
	void setName(QString name) {
		this->name = name;
	}
	void setSurname(QString surname) {
		this->surname = surname;
	}
	void setPatronymic(QString patr) {
		patronymic = patr;
	}
	void setS_day(int day) {
		s_day = day;
	}
	void setS_month(int month) {
		s_month = month;
	}
	void setS_year(int year) {
		s_year = year;
	}
	
	QString startDateToString() const; // in 01.05.2022 format
	QString startDateToStringWithoutDot() const; // in 20220501 format
	QString getAllInformation() const;
	QString getAllInformationInInputFormat() const;
	Employee* getNext() {
		return next;
	}
};



