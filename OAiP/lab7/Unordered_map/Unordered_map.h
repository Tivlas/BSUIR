#pragma once
#include <functional>
#include <list>
#include <forward_list>
#include <vector>
template <typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
class Unordered_map {
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = size_t;
	using iterator = typename std::list<value_type>::iterator;
	using const_iterator = typename std::list<value_type>::const_iterator;
	using key_equal = KeyEqual;
	static const size_type min_bucket_count = 8;

	double cur_load_factor = 0;
	const double max_load_factor_ = 0.75;
private:
	Hash hasher;
	key_equal key_eq;
	std::list<value_type> list; // list of all inserted elements
	std::vector<std::forward_list<iterator>> buckets; // vector contains lists of iterators to elements in 'list'(field) with the same hash
	size_type size_;
	size_type bucket_count_;

public:
	Unordered_map() : hasher(Hash()), key_eq(key_equal()), size_(0), bucket_count_(min_bucket_count), buckets(min_bucket_count) {}

	Unordered_map(size_type bucket_count, const Hash& hash = Hash(), const key_equal& equal = key_equal()) : hasher(hash), key_eq(equal), bucket_count_(bucket_count), buckets(bucket_count), size_(0) {}

	Unordered_map(const Unordered_map& other) : hasher(other.hasher), key_eq(other.key_eq), list(other.list), buckets(other.buckets), size_(other.size_), bucket_count_(other.bucket_count_) {}

	Unordered_map(Unordered_map&& other) : hasher(std::move(other.hasher)), key_eq(std::move(other.key_eq)), list(std::move(other.list)), buckets(std::move(other.buckets)), size_(other.size_), bucket_count_(other.bucket_count_) {}

	Unordered_map& operator=(const Unordered_map& other) {
		if (this != std::addressof(other)) {
			hasher = other.hasher;
			key_eq = other.key_eq;
			list = other.list;
			buckets = other.buckets;
			size_ = other.size_;
			bucket_count_ = other.bucket_count_;
		}
		return *this;
	}

	Unordered_map& operator=(Unordered_map&& other) {
		if (this != std::addressof(other)) {
			hasher = std::move(other.hasher);
			key_eq = std::move(other.key_eq);
			list = std::move(other.list);
			buckets = std::move(other.buckets);
			size_ = other.size_;
			bucket_count_ = other.bucket_count_;
		}
		return *this;
	}

	size_type size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	void clear() {
		list.clear();
		buckets.clear();
		size_ = 0;
		bucket_count_ = min_bucket_count;
		buckets.resize(min_bucket_count);
	}

	void rehash(size_type count) {
		if (count <= bucket_count_) {
			return;
		}
		std::list<value_type> temp = std::move(list);
		clear();
		bucket_count_ = count;
		buckets.resize(bucket_count_);
		for (auto& elem : temp) {
			insert(elem);
		}
	}

	std::pair<iterator, bool> insert(const value_type& value) {
		size_type bucket_index = hasher(value.first) % bucket_count_;
		// according to documentation we have to check if we already have this key: 
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(value.first, (*it)->first)) {
				return std::make_pair(it, false);
			}
		}

		list.push_back(value);

		buckets[bucket_index].push_front(list.end() - 1);
		++size_;
		update_cur_load_factor();
		if (need_rehash()) {
			rehash(bucket_count_ << 1);
		}
		return std::make_pair(list.end() - 1, true);
	}

	std::pair<iterator, bool> insert(value_type&& value) {
		size_type bucket_index = hasher(value.first) % bucket_count_;
		// according to documentation we have to check if we already have this key: 
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(value.first, (*it)->first)) {
				return std::make_pair(it, false);
			}
		}

		list.push_back(std::move(value));

		buckets[bucket_index].push_front(list.end() - 1);
		++size_;
		update_cur_load_factor();
		if (need_rehash()) {
			rehash(bucket_count_ << 1);
		}
		return std::make_pair(list.end() - 1, true);
	}

	size_type erase(const key_type& key) {
		size_type bucket_index = hasher(key) % bucket_count_;
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(key, (*it)->first)) {
				std::erase(buckets[bucket_index], it);
				list.erase(*it);
				--size_;
				update_cur_load_factor();
				return 1;
			}
		}
		return 0;
	}

	iterator erase(iterator pos) {
		size_type bucket_index = hasher((*pos).first) % bucket_count_;
		std::erase(buckets[bucket_index], pos);
		list.erase(pos);
		--size_;
		update_cur_load_factor();
		return pos;
	}

	iterator erase(const_iterator first, const_iterator last) {
		size_type iteration_counter = 0;
		for (auto it = first; it != last; ++it) {
			size_type bucket_index = hasher((*it).first) % bucket_count_;
			std::erase(buckets[bucket_index], it);
			++iteration_counter;
		}
		size_ -= iteration_counter;
		update_cur_load_factor();
		return last;
	}

	iterator find(const key_type& key) {
		size_type bucket_index = hasher(key) % bucket_count_;
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(key, (*it)->first)) {
				return it;
			}
		}
		return list.end();
	}

	mapped_type& at(const key_type& key) {
		size_type bucket_index = hasher(key) % bucket_count_;
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(key, (*it)->first)) {
				return (*it)->second;
			}
		}
		throw std::out_of_range("key not found");
	}

	const mapped_type& at(const key_type& key) const {
		size_type bucket_index = hasher(key) % bucket_count_;
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(key, (*it)->first)) {
				return (*it)->second;
			}
		}
		throw std::out_of_range("key not found");
	}

	mapped_type& operator[](const key_type& key) {
		auto it = find(key);
		if (it != list.end()) {
			return (*it)->second;
		}
		else return insert(value_type(key, mapped_type())).first->second;
	}

	double load_factor() const {
		return cur_load_factor;
	}

	double max_load_factor() const {
		return max_load_factor_;
	}

	size_type bucket_count() const {
		return bucket_count_;
	}

	size_type bucket(const key_type& key) const {
		return hasher(key) % bucket_count_;
	}

	size_type bucket_size(size_type n) const {
		return buckets[n].size();
	}

	size_type count(const key_type& key) const {
		size_type bucket_index = hasher(key) % bucket_count_;
		size_type count = 0;
		for (auto it = buckets[bucket_index].begin(); it != buckets[bucket_index].end(); ++it) {
			if (key_eq(key, (*it)->first)) {
				++count;
				break;
			}
		}
		return count;
	}

	bool contains(const key_type& key) const {
		return count(key) != 0;
	}

	iterator begin() {
		return list.begin();
	}

	iterator end() {
		return list.end();
	}
	
	const_iterator cbegin() const {
		return list.cbegin();
	}

	const_iterator cend() const {
		return list.cend();
	}

private: // helper methods
	void update_cur_load_factor() {
		cur_load_factor = static_cast<double>(size_) / bucket_count_;
	}

	bool need_rehash() {
		return cur_load_factor > max_load_factor;
	}
};