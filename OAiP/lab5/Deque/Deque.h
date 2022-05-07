#pragma once
template <typename T>
class Deque {
private:
	static constexpr size_t initial_map_size = 8;
	T** map = nullptr;
	size_t map_size = 0;
	static constexpr size_t bytes = sizeof(T);
	static constexpr size_t block_size = bytes <= 1 ? 16
		: bytes <= 2 ? 8
		: bytes <= 4 ? 4
		: bytes <= 8 ? 2
		: 1;

	size_t sz = 0;

public:
	class iterator {
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;

	private:
		// futher buffer is an array of pointers to blocks (arrays of T)
		pointer* cur_node; // pointer to the current subarray in the buffer (pointer to pointer: T**)
		pointer current; // pointer to the current  element in the subarray
		pointer first; // pointer to the first element in current subarray in buffer
		pointer last; // pointer to the last element in current subarray in buffer

	private:
		void set_node(pointer* new_node) // sets the current node to the new_node
		{
			cur_node = new_node;
			first = *new_node;
			last = first + block_size;
		}

	public:
		iterator() : cur_node(nullptr), current(nullptr), first(nullptr), last(nullptr) {}

		iterator(const iterator& other) : cur_node(other.cur_node), current(other.current), first(other.first), last(other.last) {}

		iterator(pointer* map_pointer, pointer cur) : cur_node(map_pointer), current(cur), first(*map_pointer), last(*map_pointer + block_size) {}

		iterator& operator++() {
			if (current == last) {
				set_node(cur_node + 1);
				current = first;
			}
			else ++current;
			return *this;
		}

		iterator operator++(int) {
			iterator tmp(*this);
			++(*this);
			return tmp;
		}

		iterator& operator--() {
			if (current == first) {
				set_node(cur_node - 1);
				current = last;
			}
			else --current;
			return *this;
		}

		iterator operator--(int) {
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

		iterator& operator+=(difference_type n) {
			difference_type offcet = n + cur - first; // wanna move n times from the current position
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
				set_node(node + offcet_blocks);
				current = first + offcet - offcet_blocks * block_size;
			}
			return *this;
		}

		iterator& operator-=(difference_type n) {
			return *this += -n;
		}

		iterator operator+(difference_type n) {
			iterator tmp(*this);
			return tmp += n;
		}

		iterator operator-(difference_type n) {
			iterator tmp(*this);
			return tmp -= n;
		}

		reference operator[](size_t index) {
			return *(*this + index);
		}

		reference operator*() {
			return *current;
		}

		bool operator==(const iterator& other) {
			return  current == other.current;
		}

		bool operator!=(const iterator& other) {
			return !(*this == other);
		}
	};



	size_t size() const { return sz; }

	bool empty() const { return sz == 0; }
private:
	


};