#pragma once
#include <QString>
struct  Node
{
	friend class IntList;
private:
	int number = 0;
	Node* next = nullptr;
	Node* prev = nullptr;
	Node(int number) : number(number) {}
};

class IntList
{
private:
	Node* head = nullptr;
	Node* tail = nullptr;
	Node* firstLoopStart = nullptr;
	Node* firstLoopEnd = nullptr;
	Node* secondLoopStart = nullptr;
	Node* secondLoopEnd = nullptr;
	
	size_t sz = 0;
public:
	void createLoops();
	IntList() {}
	~IntList();
	void add(int number);
	QString printFirstLoop();
	QString printSecondLoop();
	QString printList();
	void clear();
	size_t size() const {
		return sz;
	}
};

