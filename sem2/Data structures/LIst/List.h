#pragma once
#include <memory>
template <typename T>
struct Node
{
	Node(T data)
	{
		this->data = data;
	}
	T data;
	Node* next = nullptr;
	Node* prev = nullptr;
};

template <typename T>
class List {
	using pointer = Node<T>*;
public:
	template <typename U>
	class BidIterator {
		using pointer = Node<U>*;
	private:
		pointer ptr = nullptr;
	public:
		BidIterator(pointer ptr) : ptr(ptr) {}

		BidIterator& operator++() {
			ptr = ptr->next;
			return *this;
		}

		BidIterator& operator++(int) {
			BidIterator tmp = *this;
			++*this;
			return tmp;
		}

		BidIterator& operator--() {
			ptr = ptr->prev;
			return *this;
		}

		BidIterator& operator--(int) {
			BidIterator tmp = *this;
			--*this;
			return tmp;
		}

		U& operator*() {
			return ptr->data;
		}

		U* operator->() {
			return std::addressof(ptr->data);
		}
		
		bool operator==(const BidIterator& other) {
			return ptr == other.ptr;
		}

		bool operator!=(const BidIterator& other) {
			return ptr != other.ptr;
		}
	};
	
	using iterator = BidIterator<T>;
	using const_iterator = BidIterator<const T>;
private:
	pointer head = nullptr;
	pointer tail = nullptr;
	size_t sz = 0;
public:
	List() {};

	List(const List& other) {
		if (other.head == nullptr) {
			head = nullptr;
			tail = nullptr;
			sz = 0;
			return;
		}
		head = new Node<T>(other.head->data);
		tail = head;
		sz = 1;
		pointer tmp = other.head->next;
		while (tmp != nullptr) {
			tail->next = new Node<T>(tmp->data);
			tail->next->prev = tail;
			tail = tail->next;
			tmp = tmp->next;
			++sz;
		}
	}

	List& operator=(const List& other) {
		if (this == &other) {
			return *this;
		}
		clear();
		for (auto it = other.begin(); it != other.end(); ++it) {
			add(*it);
		}
		return *this;
	}

	~List() {
		pointer next = nullptr;
		while (head) {
			next = head->next;
			delete head;
			head = next;
		}
	}

	void add(const T& data) {
		pointer new_node = new Node<T>(data);
		if (head == nullptr) {
			head = new_node;
			tail = new_node;
		}
		else {
			tail->next = new_node;
			new_node->prev = tail;
			tail = new_node;
		}
		sz++;
	}

	void remove(const T& value) {
		pointer cur = head;
		while (cur) {
			if (cur->data == value) {
				if (cur == head) {
					head = cur->next;
					if (head) {
						head->prev = nullptr;
					}
					else {
						tail = nullptr;
					}
				}
				else if (cur == tail) {
					tail = cur->prev;
					tail->next = nullptr;
				}
				else {
					cur->prev->next = cur->next;
					cur->next->prev = cur->prev;
				}
				delete cur;
				sz--;
				return;
			}
			cur = cur->next;
		}
	}

	T* search(const T& value) {
		pointer cur = head;
		while (cur) {
			if (cur->data == value) {
				return std::addressof(cur->data);
			}
			cur = cur->next;
		}
		return nullptr;
	}

	bool empty() const {
		return sz == 0;
	}

	void clear() {
		pointer next = nullptr;
		while (head) {
			next = head->next;
			delete head;
			head = next;
		}
		sz = 0;
	}

	size_t size() const { return sz; }

	iterator begin() { return iterator(head); }

	iterator end() { return iterator(nullptr); }

	const_iterator cbegin() const { return const_iterator(head); }

	const_iterator cend() const { return const_iterator(nullptr); }
};
