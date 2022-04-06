#pragma once
#include <QChar>
struct Node {
public:
	Node(QChar data) : data(data) {}

	QChar getData() {
		return data;
	}

	void setIndices(size_t i, size_t j) {
		i_index = i;
		j_index = j;
	}

	size_t get_I_Index() {
		return i_index;
	}

	size_t get_J_Index() {
		return j_index;
	}
private:
	size_t i_index = 0; // bracket index
	size_t j_index = 0;
	QChar data;
	Node* next = nullptr;
	friend class Stack;
};

class Stack
{
private:
	Node* top = nullptr;
public:
	Stack() {};

	~Stack() {
		Node* next = nullptr;
		while (top) {
			next = top->next;
			delete top;
			top = next;
		}
	};

	bool empty() {
		return top == nullptr;
	}

	void pop();

	void push(Node* newNode);

	Node* peek();
};

