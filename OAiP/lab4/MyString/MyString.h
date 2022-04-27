#pragma once
#include <cstring>
#include <stdexcept>

#ifdef MyString_EXPORTS
#define MyString_API __declspec(dllimport)
#else
#define MyString_API __declspec(dllexport)
#endif

class MyString_API MyString
{
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
	char& at(size_t pos);
	void push_back(char ch);
	void pop_back();
	void clear();
	MyString& append(size_t count, char ch);
	char& front();
	char& back();
	const char* data() const;
	const char* c_str() const;
	bool empty() const;

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
};

void* my_memcpy(void* dest, const void* src, size_t n)
{
	char* p1 = (char*)dest;
	const char* p2 = (const char*)src;
	while (n--) {
		*p1++ = *p2++;
	}
	return p1;
}

void* my_memmove(void* dest, const void* src, size_t n)
{
	char* p1 = (char*)dest;
	const char* p2 = (const char*)src;
	while (n--) {
		*p1++ = *p2++;
	}
	return p1;
}

char* my_strcpy(char* dest, const char* src)
{
	char* p1 = dest;
	const char* p2 = src;
	while (*p2) {
		if (*p1 == '\0' && *p2 != '\0') {
			throw std::exception("strcpy: dest is too small");
		}
		*p1++ = *p2++;
	}
	*p1 = '\0';
	return p1;
}

char* my_strncpy(char* dest, const char* src, size_t n)
{
	size_t already_copied = 0;
	char* p1 = dest;
	const char* p2 = src;
	while (n-- && *p2) {
		if (*p1 == '\0' && already_copied < n)
		{
			throw std::exception("strncpy: dest is too small");
		}
		*p1++ = *p2++;
		++already_copied;
	}
	if (!*p2) {
		*p1 = '\0';
		++already_copied;
	}
	else if (already_copied != n) // if the source string is shorter than n
	{
		while (already_copied < n) {
			*p1++ = '\0';
			++already_copied;
		}
	}
	return p1;
}

char* my_strcat(char* dest, const char* src)
{
	char* p1 = dest;
	while (*p1) {
		p1++;
	}
	const char* p2 = src;
	while (*p2) {
		*p1++ = *p2++;
	}
	*p1 = '\0';
	return p1;
}

char* my_strncat(char* dest, const char* src, size_t n)
{
	char* p1 = dest;
	while (*p1) {
		p1++;
	}
	const char* p2 = src;
	while (n-- && *p2) {
		*p1++ = *p2++;
	}
	*p1 = '\0';
	return p1;
}

int  my_memcmp(const void* s1, const void* s2, size_t n)
{
	const char* p1 = (const char*)s1;
	const char* p2 = (const char*)s2;
	while (n--) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}
		p1++;
		p2++;
	}
	return 0;
}

int  my_strcmp(const char* s1, const char* s2)
{
	while (*s1 && *s2) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int  my_strcoll(const char* s1, const char* s2)
{
	return std::strcoll(s1, s2);
}

int  my_strncmp(const char* s1, const char* s2, size_t n)
{
	while (n-- && *s1 && *s2) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

size_t  my_strxfrm(char* s1, const char* s2, size_t n)
{
	return std::strxfrm(s1, s2, n);
}

bool  findChar(const char* str, char ch)
{
	while (*str) {
		if (*str == ch) {
			return true;
		}
		str++;
	}
	return false;
}

//char* my_strtok(char* str, const char* delim)
//{
//	char check1 = '\0';
//	char first = *str;
//	char* first_address = nullptr;
//	char second;
//	while (*str) {
//		first_address = str;
//		first = *str;
//		if (*str && !findChar(delim, first)) {
//			check1 = 'c';
//			break;
//		}
//		str++;
//	}
//	if (check1 == '\0') {
//		return nullptr;
//	}
//	char check2 = '\0';
//	while (*(++first_address)) {
//		second = *first_address;
//		if (*first_address && findChar(delim, second)) {
//			second = '\0';
//			check2 = 'c';
//			break;
//		}
//		first_address++;
//	}
//	if (check2 != '\0') {
//		*(str) = '\0';
//	}
//	return str;
//}

void* my_memcet(void* dest, int ch, size_t count) {
	char* p1 = (char*)dest;
	unsigned char value = (unsigned char)ch;
	while (count--) {
		*p1++ = value;
	}
	return p1;
}\

//char* my_strerror(int errnum) {
//	return std::strerror(errnum);
//}

size_t my_strlen(const char* str) {
	size_t len = 0;
	while (*str) {
		len++;
		str++;
	}
	return len;
}



