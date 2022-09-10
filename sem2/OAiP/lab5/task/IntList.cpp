#include "IntList.h"
#include <climits>


IntList::~IntList()
{
	Node* next = nullptr;
	while (head) {
		next = head->next;
		delete head;
		head = next;
	}
}

void IntList::add(int number)
{
	Node* newNode = new Node(number);
	if (head == nullptr)
	{
		head = tail = newNode;
		++sz;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
		++sz;
	}

}

QString IntList::printFirstLoop()
{
	QString str;
	Node* cur = firstLoopStart;
	if (cur)
	{
		while (cur != firstLoopEnd)
		{
			str += QString::number(cur->number) + " ";
			cur = cur->next;
		}
		str += QString::number(cur->number);
	}
	return str;
}

QString IntList::printSecondLoop()
{
	QString str;
	Node* cur = secondLoopStart;
	if (cur)
	{
		while (cur != secondLoopEnd)
		{
			str += QString::number(cur->number) + " ";
			if (cur->next == nullptr) {
				cur = head;
				continue;
			}
			cur = cur->next;
		}
		str += QString::number(cur->number);
	}

	return str;
}

QString IntList::printList()
{
	QString str;
	Node* cur = head;
	while (cur != nullptr)
	{
		str += QString::number(cur->number) + " ";
		cur = cur->next;
	}
	return str;
}

void IntList::clear()
{
	Node* next = nullptr;
	while (head) {
		next = head->next;
		delete head;
		head = next;
	}
	sz = 0;
	head = nullptr;
	tail = nullptr;
}

void IntList::createLoops()
{
	Node* curMax = head;
	Node* curMin = head;
	int max = INT_MIN;
	int min = INT_MAX;


	while (curMax != nullptr) {
		if (curMax->number > max) {
			max = curMax->number;
			firstLoopStart = curMax->next;
			secondLoopEnd = curMax;
			if (curMax->next == nullptr) {
				firstLoopStart = head;
			}
		}
		curMax = curMax->next;
	}

	while (curMin != nullptr)
	{
		if (curMin->number < min) {
			min = curMin->number;
			secondLoopStart = curMin;
			firstLoopEnd = curMin->prev;
			if (curMin->prev == nullptr) {
				firstLoopEnd = tail;
			}
		}
		curMin = curMin->next;
	}
	
	if (max == min) {
		firstLoopStart = nullptr;
		firstLoopEnd = nullptr;
		secondLoopStart = nullptr;
		secondLoopEnd = nullptr;
		return;
	}

	if (firstLoopStart->number == min && firstLoopEnd->number == max) {
		firstLoopStart = nullptr;
		firstLoopEnd = nullptr;
	}
}