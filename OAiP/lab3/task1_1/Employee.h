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
	explicit Employee() {};
public:
	Employee(int departmentNumber, QString position, QString name, QString surname, QString patronymic, short s_day, short s_month, short s_year);
	//Employee(const Employee&);
	//Employee& operator=(const Employee&);
	~Employee(){};
	size_t getDepartmentNumber() const { return departmentNumber; }
	QString getPosition() const { return position; }
	QString getName() const { return name; }
	QString getSurname() const { return surname; }
	QString getPatronymic() const { return patronymic; }
	
	int getS_day() const { return s_day; }
	int getS_month() const { return s_month; }
	int getS_year() const { return s_year; }
	
	//void setDepartmentNumber(size_t);
	//void setPosition(QString);
	//void setName(QString);
	//void setSurname(QString);
	//void setPatronymic(QString);
	//void setS_day(short);
	//void setS_month(short);
	//void setS_year(short);
	QString startDateToString() const;
	QString getAllInformation() const;
	Employee* getNext() {
		return next;
	}
};



