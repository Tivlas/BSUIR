#pragma once
#include "unique_ptr.h"
template <typename T>
class Stack
{
private:
	struct Node {
		T data;
		unique_ptr<Node> link;
	};
	unique_ptr<Node> top;

public:
	Stack() {}

	bool empty() const {
		return top == nullptr;
	}

	void push(T data) {
		unique_ptr<Node> temp(new Node());
		temp->data = data;
		temp->link = std::move(top);
		top = std::move(temp);
	}

	void pop() {
		if (top == nullptr) return;
		unique_ptr<Node> temp(new Node());
		temp = std::move(top->link);
		top = std::move(temp);
	}

	const T& topValue() const {
		return top->data;
	}
};

