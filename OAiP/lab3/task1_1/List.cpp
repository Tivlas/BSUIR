#include "List.h"

List::~List()
{
	Employee* next = nullptr;
	while (head) {
		next = head->next;
		delete head;
		head = next;
	}
}

void List::add(Employee* new_employee)
{
	if (head == nullptr)
	{
		head = new_employee;
		tail = new_employee;
	}
	else
	{
		tail->next = new_employee;
		new_employee->prev = tail;
		tail = new_employee;
	}
}

void List::remove(QString name, QString surname, QString patronymic, int departmentNumber)
{
	if (head == nullptr) return;

	Employee* cur = head;
	for (int i = 0; cur != nullptr; ++i) {
		if (cur->name == name && cur->surname == surname && cur->patronymic == patronymic && cur->departmentNumber == departmentNumber) {
			if (cur->next == nullptr) {
				tail = cur->prev;
				tail->next = nullptr;
			}
			else if (cur->prev == nullptr) {
				head = cur->next;
				head->prev = nullptr;
			}
			else {
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
			}
			delete cur;
			return;
		}
		cur = cur->next;
	}
	throw QString("No such employee");
}

Employee* List::search(QString surname)
{
	Employee* cur = head;
	while (cur != nullptr) {
		if (cur->surname == surname) {
			return cur;
		}
		cur = cur->next;
	}
	throw QString("This employee does not exist!");
}

bool List::isEmpty()
{
	return head == nullptr;
}

void List::clear()
{
	Employee* next = nullptr;
	while (head) {
		next = head->next;
		delete head;
		head = next;
	}
}

QString List::print()
{
	QString result = "";
	Employee* cur = head;
	while (cur != nullptr) {
		result += cur->getAllInformation();
		cur = cur->next;
	}
	return result;
}

QString List::printEmployeesWithSpecificSurname(QString surname)
{
	QString result = "";
	Employee* cur = head;
	while (cur != nullptr) {
		if (cur->surname == surname) {
			result += cur->getAllInformation();
		}
		cur = cur->next;
	}
	if (result == "") {
		throw QString("No employee with such surname");
	}
	return result;
}

QString List::printEmployeesWithSpecificDepNumber(int depNumber)
{
	QString result = "";
	Employee* cur = head;
	while (cur != nullptr) {
		if (cur->departmentNumber == depNumber) {
			result += cur->getAllInformation();
		}
		cur = cur->next;
	}
	if (result == "") {
		throw QString("No employee with such dep. number");
	}
	return result;
}


