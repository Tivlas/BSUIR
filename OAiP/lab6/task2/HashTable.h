#pragma once
#include "List.h"
class HashTable {
private:
	size_t default_size = 8;
	List<int>* table;
	size_t _buckets_count; // number of buckets
	size_t num_of_filled_buckets = 0;
	size_t _size; // number of items in the table

	int hash(int key) {
		double temp = key * 0.6180339887;
		temp = temp - (int)temp;
		return (int)(temp * _buckets_count);
	}

	int rehash(int key, size_t new_buckets_count) {
		double temp = key * 0.6180339887;
		temp = temp - (int)temp;
		return (int)(temp * new_buckets_count);
	}
public:
	HashTable() {
		_buckets_count = default_size;
		_size = 0;
		table = new List<int>[_buckets_count];
	}
	
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
		if (table[index].empty()) {
			++num_of_filled_buckets;
		}
		table[index].add(item);
		if(num_of_filled_buckets > _buckets_count * 0.75) {
			resize(_buckets_count * 2);
		}
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

	void resize(size_t new_buckets_count) {
		List<int>* new_table = new List<int>[new_buckets_count];
		for (size_t i = 0; i < _buckets_count; ++i) {
			for (auto it = table[i].begin(); it != table[i].end(); ++it) {
				int index = rehash(*it, new_buckets_count);
				new_table[index].add(*it);
			}
		}
		delete[] table;
		table = new_table;
		_buckets_count = new_buckets_count;
	}

	size_t buckets_count() const {
		return _buckets_count;
	}

	List<int>& get_bucket(size_t index) const {
		return table[index];
	}
};
