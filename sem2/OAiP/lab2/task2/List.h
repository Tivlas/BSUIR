#pragma once
#include "shared_week_ptrs.h"

template <typename T>
struct Node {
	T data;
	weak_ptr<Node<T>> prev;
	shared_ptr<Node<T>> next;
};

template <typename T>
class List
{
private:
public:
	shared_ptr<Node<T>> head;

	void push_front(T givenValue) {
		shared_ptr<Node<T>> pushedNode(new Node<T>());// temp node to push before head

		pushedNode->data = givenValue;
		pushedNode->next = head;
		pushedNode->prev = std::move(shared_ptr<Node<T>>()); // equal to: = nullptr

		if (head != nullptr) {
			head->prev = pushedNode;
		}
		head = std::move(pushedNode);
	}

	void push_back(T givenValue) {
		shared_ptr<Node<T>> pushedNode(new Node<T>());// temp node to push before head
		shared_ptr<Node<T>> lastNode = head;// tail

		pushedNode->data = givenValue;
		pushedNode->next = std::move(shared_ptr<Node<T>>()); // equal to: = nullptr

		if (head == nullptr)
		{
			pushedNode->prev = weak_ptr<Node<T>>(); // equal to: = nullptr
			head = pushedNode;
			return;
		}

		while (lastNode->next != nullptr) {
			lastNode = lastNode->next;
		}
		lastNode->next = pushedNode;

		pushedNode->prev = lastNode;
	}

	void pop_front() {
		if (head == nullptr)
		{
			return;
		}
		if (head->next == nullptr) {
			head.~shared_ptr();
			head = std::move(shared_ptr<Node<T>>()); // equal to: = nullptr
			return;
		}
		head->next->prev = std::move(weak_ptr<Node<T>>());

		head = head->next;
	}

	void pop_back() {
		if (head == nullptr) { return; }
		shared_ptr<Node<T>> lastNode = head;// tail

		while (lastNode->next != nullptr) { lastNode = lastNode->next; }

		if ((lastNode->prev).lock() == nullptr) {
			head.~shared_ptr();
			head = std::move(shared_ptr<Node<T>>()); // equal to: = nullptr
			return;
		}
		(lastNode->prev).lock()->next = std::move(shared_ptr<Node<T>>());

		lastNode = (lastNode->prev).lock();
	}
};
