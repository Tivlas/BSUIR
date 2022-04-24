#pragma once
#include <memory>
template <typename T>
class Vector {
public:
	//====================================================
	//================== ITERATORS =======================
	//====================================================
	template <typename T>
	class MyIterator : public std::iterator<std::random_access_iterator_tag, T> {
	private:
		T* ptr;
	public:
		MyIterator() {}

		MyIterator(T* ptr) : ptr(ptr) {}

		MyIterator operator++(int) {
			MyIterator tmp = *this;
			++ptr;
			return tmp;
		}

		MyIterator& operator++() {
			++ptr;
			return *this;
		}

		MyIterator operator--(int) {
			MyIterator tmp = *this;
			--ptr;
			return tmp;
		}

		MyIterator& operator--() {
			--ptr;
			return *this;
		}

		bool operator==(const MyIterator& other) const {
			return ptr == other.ptr;
		}

		bool operator!=(const MyIterator& other) const {
			return ptr != other.ptr;
		}

		MyIterator operator+(size_t n) {
			return iterator(ptr + n);
		}

		MyIterator operator-(size_t n) {
			return iterator(ptr - n);
		}

		MyIterator operator+=(size_t n) {
			ptr += n;
			return *this;
		}

		MyIterator operator-=(size_t n) {
			ptr -= n;
			return *this;
		}

		T& operator*() {
			return *ptr;
		}

		T* operator->() {
			return ptr;
		}
	};

	using iterator = MyIterator<T>;
	using const_iterator = MyIterator<const T>;
	using reverse_iterator = std::reverse_iterator<MyIterator<T>>;
	using const_reverse_iterator = std::reverse_iterator<MyIterator<const T>>;

	iterator begin() {
		return iterator(arr);
	}

	iterator end() {
		return iterator(arr + sz);
	}

	const_iterator cbegin() const {
		return const_iterator(arr);
	}

	const_iterator cend() const {
		return const_iterator(arr + sz);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(arr + sz - 1);
	}

	reverse_iterator rend() {
		return reverse_iterator(arr - 1);
	}

	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(arr + sz - 1);
	}

	const_reverse_iterator crend() const {
		return const_reverse_iterator(arr - 1);
	}

private:
	T* arr = nullptr;
	size_t sz = 0;
	size_t cap = 0;
public:
	Vector() {}

	explicit Vector(size_t n) {
		sz = n;
		cap = n;
		arr = new T[n];
	}

	explicit Vector(size_t n, const T& value) {
		arr = new T[n];
		sz = n;
		cap = n;
		for (size_t i = 0; i < sz; i++) {
			arr[i] = value;
		}
	}

	Vector(const Vector& other) {
		sz = other.sz;
		cap = other.cap;
		arr = new T[other.cap];
		for (size_t i = 0; i < other.sz; i++)
		{
			arr[i] = other.arr[i];
		}
	}

	~Vector() {
		this->clear();
	}

	size_t size() const {
		return sz;
	}

	size_t capacity() const {
		return cap;
	}

	T& operator[](size_t index) {
		return arr[index];
	}

	const T& operator[](size_t index) const {
		return arr[index];
	}

	T& at(size_t index) {
		if (index >= sz) {
			throw std::out_of_range("Index out of range");
		}
		return arr[index];
	}

	const T& at(size_t index) const {
		if (index >= sz) {
			throw std::out_of_range("Index out of range");
		}
		return arr[index];
	}

	void reserve(size_t new_cap) {
		if (new_cap <= cap) return;

		T* new_arr = reinterpret_cast<T*>(new char[new_cap * sizeof(T)]);
		for (size_t i = 0; i < sz; i++) {
			new(new_arr + i) T(arr[i]);
		}
		for (size_t i = 0; i < sz; ++i) {
			arr[i].~T();
		}
		delete[] reinterpret_cast<char*>(arr);
		arr = new_arr;
		cap = new_cap;
	}

	void resize(size_t count, const T& value = T()) {
		if (count > cap) reserve(count);
		if (count > sz) {
			for (size_t i = sz; i < count; i++) {
				new(arr + i)T(value);
			}
		}
		else {
			for (size_t i = count; i < sz; i++) {
				arr[i].~T();
			}
		}
		sz = count;
	}

	void assign(size_t n, const T& value) {
		this->clear();
		sz = n;
		if (n > cap) {
			cap = n;
		}
		arr = new T[cap];
		for (size_t i = 0; i < sz; ++i) {
			arr[i] = value;
		}
	}

	void push_back(T&& value) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		new(arr + sz) T(std::move(value));
	}

	void push_back(const T& value) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		new(arr + sz) T(value);
	}

	template <typename... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		size_t dist = std::distance(this->cbegin(), pos);
		for (size_t i = sz; i > dist; --i) {
			new(arr + i) T(std::move(arr[i - 1]));
		}
		new(arr + dist) T(std::forward<Args>(args)...);
		++sz;
		return iterator(arr + dist);
	}

	iterator erase(iterator pos) {
		size_t dist = std::distance(this->begin(), pos);
		arr[dist].~T();
		for (size_t i = dist; i < sz - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		arr[arr + sz - 1].~T();
		--sz;
		return iterator(arr + dist);
	}

	iterator erase(const_iterator pos) {
		size_t dist = std::distance(this->cbegin(), pos);
		arr[dist].~T();
		for (size_t i = dist; i < sz - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		arr[arr + sz - 1].~T();
		--sz;
		return iterator(arr + dist);
	}

	iterator erase(iterator first, iterator last) {  // [first, last)
		size_t dist = std::distance(this->begin(), first);
		size_t dist2 = std::distance(first, last);
		// destroy [first, last)
		for (size_t i = 0; i < dist2; ++i) {
			arr[dist + i].~T();
		}

		for (size_t i = dist; i < sz - dist2; ++i) {
			arr[i] = arr[i + dist2];
		}
		for (size_t i = sz - dist2; i < sz; ++i) {
			arr[i].~T();
		}
		sz -= dist2;
		return iterator(arr + dist);
	}

	iterator insert(iterator pos, const T& value) { // before pos
		size_t dist = std::distance(this->begin(), pos);
		if (sz == cap) {
			reserve(cap * 2);
		}
		for (size_t i = sz; i > dist; --i) {
			new(arr + i) T(std::move(arr[i - 1]));
		}
		new(arr + dist) T(value);
		++sz;
		return iterator(arr + dist);
	}

	iterator insert(iterator pos, T&& value) {
		size_t dist = std::distance(this->begin(), pos);
		if (sz == cap) {
			reserve(cap * 2);
		}
		for (size_t i = sz; i > dist; --i) {
			new(arr + i) T(std::move(arr[i - 1]));
		}
		new(arr + dist) T(std::move(value));
		++sz;
		return iterator(arr + dist);
	}

	iterator insert(iterator pos, size_t count, const T& value) { // insert count copies of value before pos
		size_t dist = std::distance(this->begin(), pos);
		if (sz + count > cap) {
			reserve(cap * 2);
		}
		for (size_t i = sz - 1; i > dist; --i) {
			new(arr + i + count) T(std::move(arr[i]));
		}
		for (size_t i = 0; i < count; ++i) {
			new(arr + dist + i) T(value);
		}
		sz += count;
		return iterator(arr + dist);
	}

	template <typename... Args>
	void emplace_back(Args&&... args) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		new(arr + sz) T(std::forward<Args>(args)...);
		++sz;
	}

	void pop_back() {
		if (sz == 0) {
			return;
		}
		arr[sz - 1].~T();
		--sz;
	}

	void clear() {
		for (size_t i = 0; i < sz; ++i) {
			arr[i].~T();
		}
		sz = 0;
		arr = nullptr;
	}

	bool empty() const {
		return sz == 0;
	}

	T& back() {
		return arr[sz - 1];
	}

	T& front() {
		return arr[0];
	}

	T* data() {
		return arr;
	}

	void swap(Vector& other) {
		std::swap(arr, other.arr);
		std::swap(sz, other.sz);
		std::swap(cap, other.cap);
	}

	//operator =
	Vector& operator=(Vector&& other) {
		clear();
		arr = other.arr;
		sz = other.sz;
		cap = other.cap;
		other.arr = nullptr;
		other.sz = 0;
		other.cap = 0;
		return *this;
	}

	Vector& operator=(const Vector& other) {
		clear();
		reserve(other.sz);
		for (size_t i = 0; i < other.sz; ++i) {
			new(arr + i) T(other.arr[i]);
		}
		sz = other.sz;
		return *this;
	}
};

