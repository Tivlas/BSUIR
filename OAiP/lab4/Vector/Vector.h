#pragma once
#include <memory>
template <typename T, typename Alloc = std::allocator<T>>
class Vector {
public:
	//====================================================
	//================== ITERATORS =======================
	//====================================================
	template <bool IsConst>
	class common_iterator {
	private:
		std::conditional_t<IsConst, const T*, T*> ptr;
	public:
		common_iterator(T* ptr) : ptr(ptr) {}

		common_iterator<IsConst>& operator++() {
			++ptr;
			return *this;
		}

		common_iterator<IsConst>& operator--() {
			--ptr;
			return *this;
		}

		bool operator==(const common_iterator& other) const {
			return ptr == other.ptr;
		}

		bool operator!=(const common_iterator& other) const {
			return ptr != other.ptr;
		}

		common_iterator<IsConst> operator+(size_t n) {
			return iterator(ptr + n);
		}

		common_iterator<IsConst> operator-(size_t n) {
			return iterator(ptr - n);
		}

		common_iterator<IsConst> operator+=(size_t n) {
			ptr += n;
			return *this;
		}

		common_iterator<IsConst> operator-=(size_t n) {
			ptr -= n;
			return *this;
		}

		std::conditional_t<IsConst, const T&, T&> operator*() {
			return *ptr;
		}

		std::conditional_t<IsConst, const T*, T*> operator->() {
			return ptr;
		}
	};

	using iterator = common_iterator<false>;
	using const_iterator = common_iterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	iterator iter;
	const_iterator citer;
	reverse_iterator riter;
	const_reverse_iterator criter;

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
	Alloc alloc = Alloc();
public:
	Vector() {}

	explicit Vector(size_t n, const T& value = T(), const Alloc& alloc = Alloc()) :alloc(alloc) {
		arr = alloc.allocate(n);
		for (size_t i = 0; i < n; ++i) {
			alloc.construct(arr + i, value);
		}
		sz = n;
		cap = n;
	}

	Vector(const Vector& other) :alloc(other.alloc) {
		arr = alloc.allocate(other.sz);
		for (size_t i = 0; i < other.sz; ++i) {
			alloc.construct(arr + i, other.arr[i]);
		}
		sz = other.sz;
		cap = other.cap;
	}

	~Vector() {
		for (size_t i = 0; i < sz; ++i) {
			alloc.destroy(arr + i);
		}
		alloc.deallocate(arr, cap);
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

		T* new_arr = alloc.allocate((int)new_cap);
		for (size_t i = 0; i < sz; i++) {
			alloc.construct(new_arr + i, arr[i]);
		}
		for (size_t i = 0; i < sz; ++i) {
			alloc.destroy(arr + i);
		}
		alloc.deallocate(arr, cap);
		arr = new_arr;
		cap = new_cap;
	}

	void resize(size_t count, const T& value = T()) {
		if (count > cap) reserve(count);
		if (count > sz) {
			for (size_t i = sz; i < count; i++) {
				alloc.construct(arr + i, value);
			}
		}
		else {
			for (size_t i = count; i < sz; i++) {
				alloc.destroy(arr + i);
			}
		}
		sz = count;
	}

	void assign(size_t n, const T& value) {
		this->clear();
		reserve(n);
		for (size_t i = 0; i < n; ++i) {
			//new(arr + i) T(value);
			alloc.construct(arr + i, value);
		}
		sz = n;
	}

	void push_back(T&& value) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		alloc.construct(arr + sz, std::move(value));
	}

	void push_back(const T& value) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		alloc.construct(arr + sz, value);
	}

	template <typename... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		size_t dist = std::distance(this->cbegin(), pos);
		for (size_t i = sz; i > dist; --i) {
			alloc.construct(arr + i, arr[i - 1]);
		}
		alloc.construct(arr + dist, std::forward<Args>(args)...);
		++sz;
		return iterator(arr + dist);
	}

	iterator erase(iterator pos) {
		size_t dist = std::distance(this->begin(), pos);
		alloc.destroy(arr + dist);
		for (size_t i = dist; i < sz - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		alloc.destroy(arr + sz - 1);
		--sz;
		return iterator(arr + dist);
	}

	iterator erase(const_iterator pos) {
		size_t dist = std::distance(this->cbegin(), pos);
		alloc.destroy(arr + dist);
		for (size_t i = dist; i < sz - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		alloc.destroy(arr + sz - 1);
		--sz;
		return iterator(arr + dist);
	}

	iterator erase(iterator first, iterator last) {  // [first, last)
		size_t dist = std::distance(this->begin(), first);
		size_t dist2 = std::distance(first, last);
		// destroy [first, last)
		for (size_t i = 0; i < dist2; ++i) {
			alloc.destroy(arr + dist + i);
		}

		for (size_t i = dist; i < sz - dist2; ++i) {
			arr[i] = arr[i + dist2];
		}
		for (size_t i = sz - dist2; i < sz; ++i) {
			alloc.destroy(arr + i);
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
			alloc.construct(arr + i, arr[i - 1]);
		}
		alloc.construct(arr + dist, value);
		++sz;
		return iterator(arr + dist);
	}

	iterator insert(iterator pos, T&& value) {
		size_t dist = std::distance(this->begin(), pos);
		if (sz == cap) {
			reserve(cap * 2);
		}
		for (size_t i = sz; i > dist; --i) {
			alloc.construct(arr + i, arr[i - 1]);
		}
		alloc.construct(arr + dist, std::move(value));
		++sz;
		return iterator(arr + dist);
	}

	iterator insert(iterator pos, size_t count, const T& value) { // insert count copies of value before pos
		size_t dist = std::distance(this->begin(), pos);
		if (sz + count > cap) {
			reserve(cap * 2);
		}
		for (size_t i = sz - 1; i > dist; --i) {
			alloc.construct(arr + i + 5, arr[i]);
		}
		for (size_t i = 0; i < count; ++i) {
			alloc.construct(arr + dist + i, value);
		}
		sz += count;
		return iterator(arr + dist);
	}

	template <typename... Args>
	void emplace_back(Args&&... args) {
		if (sz == cap) {
			reserve(cap * 2);
		}
		alloc.construct(arr + sz, std::forward<Args>(args)...);
		++sz;
	}

	void pop_back() {
		if (sz == 0) {
			return;
		}
		alloc.destroy(arr + sz - 1);
		--sz;
	}

	void clear() {
		for (size_t i = 0; i < sz; ++i) {
			alloc.destroy(arr + i);
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
		std::swap(alloc, other.alloc);
	}


};

