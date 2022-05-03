#pragma once
#include <stdexcept>

#define MYSTRING __declspec(dllexport)

class MYSTRING MyString
{
public:
	//====================================================
	//================== ITERATORS =======================
	//====================================================

	class MyIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = char;
		using difference_type = std::ptrdiff_t;
		using pointer = char*;
		using reference = char&;
	private:
		pointer ptr;
	public:
		MyIterator() {}

		MyIterator(pointer ptr) : ptr(ptr) {}

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
			return MyIterator(ptr + n);
		}

		MyIterator operator-(size_t n) {
			return MyIterator(ptr - n);
		}

		MyIterator operator+=(size_t n) {
			ptr += n;
			return *this;
		}

		MyIterator operator-=(size_t n) {
			ptr -= n;
			return *this;
		}

		reference operator*() {
			return *ptr;
		}

		pointer operator->() {
			return ptr;
		}

		size_t operator-(const MyIterator& other) {
			return ptr - other.ptr;
		}
	};

	class MyConstIterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = const char;
		using difference_type = std::ptrdiff_t;
		using pointer = const char*;
		using reference = const char&;
	private:
		pointer ptr;
	public:
		MyConstIterator() {}

		MyConstIterator(pointer ptr) : ptr(ptr) {}

		MyConstIterator operator++(int) {
			MyConstIterator tmp = *this;
			++ptr;
			return tmp;
		}

		MyConstIterator& operator++() {
			++ptr;
			return *this;
		}

		MyConstIterator operator--(int) {
			MyConstIterator tmp = *this;
			--ptr;
			return tmp;
		}

		MyConstIterator& operator--() {
			--ptr;
			return *this;
		}

		bool operator==(const MyConstIterator& other) const {
			return ptr == other.ptr;
		}

		bool operator!=(const MyConstIterator& other) const {
			return ptr != other.ptr;
		}

		MyConstIterator operator+(size_t n) {
			return MyConstIterator(ptr + n);
		}

		MyConstIterator operator-(size_t n) {
			return MyConstIterator(ptr - n);
		}

		MyConstIterator operator+=(size_t n) {
			ptr += n;
			return *this;
		}

		MyConstIterator operator-=(size_t n) {
			ptr -= n;
			return *this;
		}

		reference operator*() {
			return *ptr;
		}

		pointer operator->() {
			return ptr;
		}

		size_t operator-(const MyConstIterator& other) {
			return ptr - other.ptr;
		}
	};

	using iterator = MyIterator;
	using const_iterator = MyConstIterator;
	using  reverse_iterator = std::reverse_iterator<MyIterator>;
	using  const_reverse_iterator = std::reverse_iterator<MyConstIterator>;

	iterator begin() {
		return iterator(str);
	}

	iterator end() {
		return str ? iterator(str + sz) : iterator(str);
	}

	const_iterator cbegin() const {
		return const_iterator(str);
	}

	const_iterator cend() const {
		return str ? const_iterator(str + sz) : const_iterator(str);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}

	const_reverse_iterator crend() const {
		return const_reverse_iterator(cbegin());
	}

	//====================================================
	//================== END OF ITERATORS =======================
	//====================================================
private:
	char* str = nullptr;
	size_t sz = 0;
	size_t cap = 0;
public:
	MyString();
	MyString(size_t);
	MyString(size_t, char);
	MyString(const MyString&);
	MyString(const char[]);
	~MyString();
	size_t size() const;
	size_t capacity() const;
	void reserve(size_t new_cap);
	void resize(size_t new_size, const char ch);
	char& operator[](size_t pos);
	const char& operator[](size_t pos) const;
	char& at(size_t pos);
	const char& at(size_t pos) const;
	void push_back(char ch);
	void pop_back();
	void clear();
	MyString& append(size_t count, char ch);
	char& front();
	char& back();
	const char* data() const;
	const char* c_str() const;
	bool empty() const;
	iterator insert(iterator pos, const char value);
	iterator insert(iterator pos, size_t count, const char value);
	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);
	MyString& operator+=(const MyString& other);
	MyString& operator+=(const char* other);
	MyString& operator+=(char ch);
	MyString& operator=(const MyString& other);
	MyString& operator=(const char* other);
	MyString& operator=(char ch);

	explicit operator std::string() const;
};


bool operator==(const MyString& left, const MyString& right);
bool operator!=(const MyString& left, const MyString& right);
bool operator<(const MyString& left, const MyString& right);
bool operator>(const MyString& left, const MyString& right);
bool operator<=(const MyString& left, const MyString& right);
bool operator>=(const MyString& left, const MyString& right);


extern "C" MYSTRING size_t my_strlen(const char* start);

extern "C" MYSTRING void* my_memcpy(void* dest, const void* src, size_t n);

extern "C" MYSTRING void* my_memmove(void* dest, const void* src, size_t n);

extern "C++" MYSTRING char* my_strcpy(char* dest, const char* src);

extern "C++" MYSTRING char* my_strncpy(char* dest, const char* src, size_t n);

extern "C" MYSTRING char* my_strcat(char* dest, const char* src);

extern "C" MYSTRING char* my_strncat(char* dest, const char* src, size_t n);

extern "C" MYSTRING int  my_memcmp(const void* s1, const void* s2, size_t n);

extern "C" MYSTRING int  my_strcmp(const char* s1, const char* s2);

extern "C" MYSTRING int  my_strncmp(const char* s1, const char* s2, size_t n);

extern "C" MYSTRING void* my_memset(void* dest, int ch, size_t count);

extern "C" MYSTRING size_t my_strxfrm(char* dest, const char* src, size_t n);

extern "C" MYSTRING char* my_strtok(char* str, const char* delim);



