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
		++size;
	}
	else
	{
		tail->next = new_employee;
		new_employee->prev = tail;
		tail = new_employee;
		++size;
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
				if (tail)
				{
					tail->next = nullptr;
				}
			}
			if (cur->prev == nullptr) {
				head = cur->next;
				if (head)
				{
					head->prev = nullptr;
				}
			}
			else if(cur->next != nullptr && cur->prev != nullptr) {
				cur->prev->next = cur->next;
				cur->next->prev = cur->prev;
			}
			delete cur;
			--size;
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

bool List::isEmpty() const
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
	size = 0;
}

QString List::print() const
{
	QString result = "";
	Employee* cur = head;
	while (cur != nullptr) {
		result += cur->getAllInformation();
		cur = cur->next;
	}
	return result;
}

QString List::printInInputFormat() const
{
	QString result = "";
	Employee* cur = head;
	while (cur != nullptr) {
		result += cur->getAllInformationInInputFormat();
		cur = cur->next;
	}
	return result;
}

QString List::printEmployeesWithSpecificSurname(QString surname) const
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

QString List::printEmployeesWithSpecificDepNumber(int depNumber) const
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

Employee* List::getElement(int index)
{
	Employee* cur = head;
	for (int i = 0; i < index; ++i) {
		cur = cur->next;
	}
	return cur;
}

void List::swap(int i, int j)
{
	Employee* first = getElement(i);
	Employee* second = getElement(j);
	if (first->prev == nullptr) {
		head = second;
	}
	else if (second->prev == nullptr) {
		head = first;
	}

	if (second->next == nullptr) {
		tail = first;
	}
	else if (first->next == nullptr) {
		tail = second;
	}

	Employee* temp = first->next;
	first->next = second->next;
	second->next = temp;

	if (first->next != nullptr) {
		first->next->prev = first;
	}
	if (second->next != nullptr) {
		second->next->prev = second;
	}

	temp = first->prev;
	first->prev = second->prev;
	second->prev = temp;

	if (first->prev != nullptr) {
		first->prev->next = first;
	}

	if (second->prev != nullptr) {
		second->prev->next = second;
	}

}


