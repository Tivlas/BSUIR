#pragma once
#include <QChar>
template <typename T>
class Array
{
private:
	size_t capacity = 0;
	size_t size_ = 0;
	T* elements = nullptr;

	void grow() {
		capacity *= 2;
		T* temp = new T[capacity];
		for (int i = 0; i < size_; i++)
		{
			temp[i] = elements[i];
		}
		delete[] elements;
		elements = temp;
	}
public:
	Array() {
		size_ = 0;
		capacity = 15;
		elements = new T[capacity];
	}

	~Array() {
		delete[] elements;
	}

	T& operator[](size_t index) const {
		return elements[index];
	}

	void push_back(T element) {
		if (size_ == capacity)
		{
			grow();
		}
		elements[size_] = element;
		++size_;
	}

	void pop_back() {
		if (size_ > 0) {
			--size_;
		}
	}

	size_t size() {
		return size_;
	}

	void forget() {
		size_ = 0;
	}
};