#pragma once
#include "Array.h"

template <typename T>
class Stack
{
private:
	Array<T> elements;
public:
	Stack() : elements() {}

	~Stack() {}

	void push(T element) {
		elements.push_back(element);
	}
	
	void pop(){
		elements.pop_back();
	}
	
	T peek() {
		return elements[elements.size() - 1];
	}
	
	bool empty() {
		return elements.size() == 0 ? true : false;
	}
};


