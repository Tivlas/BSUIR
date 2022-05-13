#pragma once
#include "List.h"
class HashTable {
private:
	List<int>* table;
	size_t _buckets_count; // number of buckets
	size_t _size; // number of items in the table

	int hash(int key) {
		return key % _buckets_count;
	}
public:
	HashTable(size_t size) {
		this->_buckets_count = size;
		table = new List<int>[size];
		_size = 0;
	}

	~HashTable() {
		delete[] table;
	}

	void insert(const int item) {
		int index = hash(item);
		table[index].add(item);
		++_size;
	}

	void remove(const int item) {
		size_t index = hash(item);
		int* delete_ptr = table[index].search(item);
		if (delete_ptr) {
			table[index].remove(*delete_ptr);
			--_size;
		}
		--_size;
	}

	int* search(int number) {
		size_t index = hash(number);
		return table[index].search(number);
	}

	size_t size()const {
		return _size;
	}

	size_t buckets_count() const {
		return _buckets_count;
	}

	const List<int>& get_bucket(size_t index) const {
		return table[index];
	}
};
