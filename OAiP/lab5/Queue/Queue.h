#pragma once
#include <memory>

template <typename T>
class Queue {
private:
	template <typename U>
	struct Node {
		U data;
		std::shared_ptr<Node<U>> next = nullptr;
		Node() {}
		Node(const U& value) {
			data = value;
		}
	};

	std::shared_ptr<Node<T>> head = nullptr;
	std::shared_ptr<Node<T>> tail = nullptr;
	size_t sz = 0;
public:
	Queue() {}

	void push(const T& value) {
		std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(value);
		if (head == nullptr) {
			head = tail = newNode;
			tail->next = nullptr;
		}
		else {
			tail->next = newNode;
			tail = newNode;
			//tail->next = nullptr;
		}
		++sz;
	}

	void pop() {
		if (head == nullptr) {
			return;
		}
		head = head->next;
		--sz;
		if (!head) {
			tail = nullptr;
		}
	}

	T& front() {
		return head->data;
	}

	const T& front() const {
		return head->data;
	}

	T& back() {
		return tail->data;
	}

	const T& back() const {
		return tail->data;
	}

	bool empty() const {
		return head == nullptr;
	}

	size_t size() const {
		return sz;
	}

	Queue& operator = (const Queue& other) {
		head = other.head;
		tail = other.tail;
		sz = other.sz;
		return *this;
	}

	Queue(Queue&& other) {
		head = other.head;
		tail = other.tail;
		sz = other.sz;
		other.head = nullptr;
		other.tail = nullptr;
		other.sz = 0;
	}
};