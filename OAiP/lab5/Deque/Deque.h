#pragma once
#include <algorithm>

template <typename T>
class Deque {
private:
	static constexpr size_t initial_buffer_size = 8;
	T** buffer = nullptr;
	size_t buffer_size = 0;
	static constexpr size_t bytes = sizeof(T);
	static constexpr size_t block_size = bytes <= 1 ? 16
		: bytes <= 2 ? 8
		: bytes <= 4 ? 4
		: bytes <= 8 ? 2
		: 1;

	size_t sz = 0;

public:
	template <typename U>
	class MyIterator {
		friend class Deque;
		using iterator_category = std::random_access_iterator_tag;
		using value_type = U;
		using difference_type = std::ptrdiff_t;
		using pointer = U*;
		using reference = U&;

	private:
		// futher buffer is an array of pointers to blocks (arrays of T)
		pointer* cur_node; // pointer to the current subarray in the buffer (pointer to pointer: T**)
		pointer current; // pointer to the current  element in the subarray
		pointer first; // pointer to the first element in current subarray in buffer
		pointer last; // pointer to the element after last in current subarray in buffer

	private:
		void set_node(pointer* new_node) // sets the current node to the new_node
		{
			cur_node = new_node;
			first = *new_node;
			last = first + block_size;
		}

	public:
		MyIterator() : cur_node(nullptr), current(nullptr), first(nullptr), last(nullptr) {}

		MyIterator(const MyIterator& other) : cur_node(other.cur_node), current(other.current), first(other.first), last(other.last) {}

		MyIterator(pointer* buffer_pointer, pointer cur) : cur_node(buffer_pointer), current(cur), first(*buffer_pointer), last(*buffer_pointer + block_size) {}

		MyIterator& operator++() {
			++current;
			if (current == last) {
				set_node(cur_node + 1);
				current = first;
			}
			return *this;
		}

		MyIterator operator++(int) {
			MyIterator tmp = *this;
			++(*this);
			return tmp;
		}

		MyIterator& operator--() {
			if (current == first) {
				set_node(cur_node - 1);
				current = last;
			}
			--current;
			return *this;
		}

		MyIterator operator--(int) {
			MyIterator tmp = *this;
			--(*this);
			return tmp;
		}

		MyIterator& operator+=(difference_type n) {
			difference_type offcet = n + current - first; // wanna move n times from the current position
			if (offcet >= 0 && offcet < block_size) {
				current += n;
			}
			else {
				difference_type offcet_blocks = 0;
				if (offcet)
				{
					offcet_blocks = offcet / block_size;
				}
				else
				{
					offcet_blocks = (offcet + 1) / block_size - 1;
				}
				set_node(cur_node + offcet_blocks);
				current = first + offcet - offcet_blocks * block_size;
			}
			return *this;
		}

		MyIterator& operator-=(difference_type n) {
			return *this += -n;
		}

		MyIterator operator+(difference_type n) {
			MyIterator tmp(*this);
			return tmp += n;
		}

		MyIterator operator-(difference_type n) {
			MyIterator tmp(*this);
			return tmp -= n;
		}

		reference operator[](size_t index) {
			return *(*this + index);
		}

		reference operator*() {
			return *current;
		}

		bool operator==(const MyIterator& other) {
			return  current == other.current;
		}

		bool operator!=(const MyIterator& other) {
			return !(*this == other);
		}

		MyIterator& operator=(const MyIterator& other) {
			cur_node = other.cur_node;
			current = other.current;
			first = other.first;
			last = other.last;
			return *this;
		}

		difference_type operator-(const MyIterator& other) const {
			difference_type dist = 0;
			MyIterator tmp = *this;
			while (tmp != other) {
				++dist;
				--tmp;
			}
			return dist;
		}
	};

	using iterator = MyIterator<T>;

private:
	iterator first, last, after_last_filled;


public:
	iterator begin() {
		return first;
	}

	iterator end() {
		return after_last_filled;
	}

	Deque() {
		create_deque(0);
	}

	explicit Deque(size_t count) : sz(count) {
		create_deque(count);
	}

	explicit Deque(size_t count, const T& value) : sz(count) {
		create_deque(count);
		std::fill(begin(), end(), value);
	}

	Deque(const Deque& other) : sz(other.sz) {
		create_deque(other.sz);
		std::copy(other.begin(), other.end(), this->begin());
	}

	Deque(Deque&& other) : sz(other.sz), buffer(other.buffer), buffer_size(other.buffer_size), first(other.first), last(other.last) {
		other.buffer = nullptr;
		other.buffer_size = 0;
		other.first = other.last;
	}

	~Deque() {
		for (T** cur = first.cur_node; cur <= last.cur_node; ++cur) {
			delete[] * cur;
		}
		delete[] buffer;
	}

	template <typename... Args>
	void emplace_back(Args&&... args) {
		if (after_last_filled.current != last.last) {
			new(after_last_filled.current) T(std::forward<Args>(args)...);
			++after_last_filled.current;
		}
		else {
			reserve_buffer_at_back(); // 1 by default
			*(last.cur_node + 1) = reinterpret_cast<T*>(new char[block_size * sizeof(T)]);
			new(after_last_filled.current) T(std::forward<Args>(args)...);
			last.set_node(last.cur_node + 1);
			last.current = last.first+1;
			++after_last_filled.current;
		}
		++sz;
	}

	void push_back(const T& value) {
		emplace_back(value);
	}

	void push_back(T&& value) {
		emplace_back(std::move(value));
	}


	template <typename... Args>
	void emplace_front(Args&&... args) {
		if (first.current != first.first) {
			--first.current;
			new(first.current) T(std::forward<Args>(args)...);
		}
		else {
			reserve_buffer_at_front(); // 1 by default
			*(first.cur_node - 1) = reinterpret_cast<T*>(new char[block_size * sizeof(T)]);
			first.set_node(first.cur_node - 1);
			first.current = first.last - 1;
			new(first.current) T(std::forward<Args>(args)...);
		}
		++sz;
	}

	void push_front(const T& value) {
		emplace_front(value);
	}

	void push_front(T&& value) {
		emplace_front(std::move(value));
	}


	void pop_back() {
		std::destroy_at(last.current);
		--last;
		--sz;
	}

	void pop_front() {
		std::destroy_at(first.current);
		++first;
		--sz;
	}


	void resize(size_t new_size) {
		while (sz < new_size) {
			emplace_back();
		}
		while (new_size < sz) {
			pop_back();
		}
	}

	void resize(size_t new_size, const T& value) {
		while (sz < new_size) {
			emplace_back(value);
		}
		while (new_size < sz) {
			pop_back();
		}
	}


	template <typename... Args>
	iterator emplace(iterator pos, Args&&... args) {
		size_t dist = pos - first;
		if (dist <= sz / 2) {
			emplace_front(std::forward<Args>(args)...);
			T temp = *begin();
			iterator start = begin();
			iterator end = begin() + dist;
			for (; start != end; ++start) {
				*start = *(start + 1);
			}
			*(begin() + dist) = temp;
			return begin() + dist;
		}
		else {
			size_t dist_from_end = end() - pos;
			emplace_back(std::forward<Args>(args)...);
			T temp = *(end() - 1);
			iterator last = end() - 1;
			iterator start = end() - dist_from_end - 1;
			for (; start != last; ++start) {
				*(start) = *(start + 1);
			}
			*(end() - 1) = temp;
			return end() - dist_from_end-1;
		}
	}

	iterator insert(iterator pos, const T& value) {
		return emplace(pos, value);
	}

	iterator insert(iterator pos, T&& value) {
		return emplace(pos, std::move(value));
	}

	iterator insert(iterator pos, size_t count, const T& value) {
		iterator it = emplace(pos, value);
		for (size_t i = 1; i < count; ++i) {
			it = emplace(it, value);
		}
		return it;
	}

	iterator erase(iterator first, iterator last) {
		size_t dist = std::distance(begin(), first);
		size_t count = std::distance(first, last);
		if (dist <= size() / 2) {
			std::move_backward(begin(), first, last);
			for (; count > 0; --count) {
				pop_front();
			}
		}
		else {
			std::move(last, end(), first);
			for (; count > 0; --count) {
				pop_back();
			}
		}
		return begin() + dist;
	}

	iterator erase(iterator pos) {
		return erase(pos, pos + 1);
	}


	size_t size() const { return sz; }

	bool empty() const { return sz == 0; }

	T& operator[](size_t index) {
		return first[index];
	}

	const T& operator[](size_t index) const {
		return first[index];
	}

	void clear() {
		while (!empty()) {
			pop_back();
		}
	}

private:

	void reallocate_buffer(size_t nodes_to_add, bool add_at_front) {
		size_t prev_num_of_nodes = last.cur_node - first.cur_node + 1;
		size_t new_num_of_nodes = prev_num_of_nodes + nodes_to_add;
		T** new_buffer_start;
		if (buffer_size > 2 * new_num_of_nodes) {
			new_buffer_start = buffer + (buffer_size - new_num_of_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_buffer_start < first.cur_node) {
				std::copy(first.cur_node, last.cur_node + 1, new_buffer_start);
			}
			else {
				std::copy_backward(first.cur_node, last.cur_node + 1, new_buffer_start + prev_num_of_nodes);
			}
		}
		else {
			size_t new_buffer_size = buffer_size + std::max(buffer_size, nodes_to_add) + 2;
			T** new_buffer = new T * [new_buffer_size];
			new_buffer_start = new_buffer + (new_buffer_size - new_num_of_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			std::copy(buffer, buffer + buffer_size, new_buffer_start);
			delete[] buffer;
			buffer = new_buffer;
			buffer_size = new_buffer_size;
		}
		first.set_node(new_buffer_start);
		last.set_node(new_buffer_start + prev_num_of_nodes - 1);
		after_last_filled.set_node( new_buffer_start + prev_num_of_nodes);
		after_last_filled.current = after_last_filled.first;
	}

	void reserve_buffer_at_back(size_t nodes_to_add = 1) {
		if (nodes_to_add > buffer_size - (last.cur_node - buffer) - 1) { // check if we wanna add more than there are free nodes
			reallocate_buffer(nodes_to_add, false);
		}
	}

	void reserve_buffer_at_front(size_t nodes_to_add = 1) {
		if ((std::ptrdiff_t)nodes_to_add > first.cur_node - buffer) {
			reallocate_buffer(nodes_to_add, true);
		}
	}

	void create_deque(size_t n) {
		size_t num_of_nodes = n / block_size + 1;
		buffer_size = std::max(initial_buffer_size, num_of_nodes + 2);
		buffer = new T * [buffer_size];
		T** buffer_start = buffer + (buffer_size - num_of_nodes) / 2;
		T** buffer_end = buffer_start + num_of_nodes - 1;
		for (T** cur = buffer_start; cur <= buffer_end; ++cur) {
			*cur = reinterpret_cast<T*>(new char[block_size * sizeof(T)]);
		}
		first.set_node(buffer_start);
		last.set_node(buffer_end);
		first.current = first.first;
		last.current = last.first + n % block_size;
		after_last_filled.set_node(buffer_start);
		after_last_filled.current = after_last_filled.first;
	}
};