#pragma once
#include "Employee.h"
class List
{
private:
	Employee* head = nullptr;
	Employee* tail = nullptr;
	int size = 0;
public:
	List() {};
	~List();
	void add(Employee*);
	void remove(QString name, QString surname, QString patronymic,int departmentNumber);
	Employee* search(QString surname);
	bool isEmpty() const;
	void clear();

	QString print() const;
	QString printInInputFormat() const;
	QString printEmployeesWithSpecificSurname(QString surname) const;
	QString printEmployeesWithSpecificDepNumber(int depNumber) const;
	
	Employee* getHead() const {
		return head;
	}
	
	Employee* getTail() const {
		return tail;
	}
	int getSize() const { return size; }
	Employee* getElement(int index);
	void swap(int i, int j);
};

