#pragma once
#include "Employee.h"
class List
{
private:
	Employee* head = nullptr;
	Employee* tail = nullptr;
public:
	List() {};
	~List();
	void add(Employee*);
	void remove(QString name, QString surname, QString patronymic,int departmentNumber);
	Employee* search(QString surname);
	bool isEmpty();
	void clear();

	QString print();
	QString printEmployeesWithSpecificSurname(QString surname);
	QString printEmployeesWithSpecificDepNumber(int depNumber);
	
	Employee* getHead() {
		return head;
	}
	
	Employee* getTail() {
		return tail;
	}
};

