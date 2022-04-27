#include "pch.h"
#include "MyString.h"

MyString::MyString()
{
}

MyString::MyString(size_t size) :str(new char[size + 1]), sz(size)
{
	str[sz] = '\0';
	cap = sz;
}

MyString::MyString(size_t size, char ch) : str(new char[size + 1]), sz(size)
{
	for (size_t i = 0; i < sz; i++)
	{
		str[i] = ch;
	}
	str[sz] = '\0';
	cap = sz;
}

MyString::MyString(const MyString& other)
{
	sz = other.sz;
	str = new char[sz + 1];
	for (size_t i = 0; i < sz; i++)
	{
		str[i] = other.str[i];
	}
	str[sz] = '\0';
	cap = other.cap;
}

MyString::MyString(const char other[])
{
	sz = my_strlen(other);
	this->str = new char[sz + 1];
	memcpy(this->str, other, sz + 1);
	cap = sz;
}

MyString::~MyString()
{
	delete[] str;
}

size_t MyString::size() const
{
	return sz;
}

size_t MyString::capacity() const
{
	return cap;
}

void MyString::reserve(size_t new_cap)
{
	if (new_cap <= cap) return;

	char* new_str = new char[new_cap];
	for (size_t i = 0; i < sz; i++) {
		new_str[i] = str[i];
	}
	delete[] str;
	str = new_str;
	cap = new_cap;
}

void MyString::resize(size_t new_size, const char ch = '\0')
{
	if (new_size > cap) reserve(new_size);
	if (new_size > sz) {
		for (size_t i = sz; i < new_size; i++) {
			str[i] = ch;
		}
	}
	sz = new_size;
}

char& MyString::operator[](size_t pos)
{
	return str[pos];
}

const char& MyString::operator[](size_t pos) const
{
	return str[pos];
}

char& MyString::at(size_t pos)
{
	if (pos >= sz)
		throw std::out_of_range("Out of range");
	return str[pos];
}

const char& MyString::at(size_t pos) const
{
	if (pos >= sz)
		throw std::out_of_range("Out of range");
	return str[pos];
}

void MyString::push_back(char ch)
{
	if (sz == cap) {
		if (cap == 0) reserve(1);
		else reserve(cap * 2);
	}
	str[sz] = ch;
	sz++;
	str[sz] = '\0';
}

void MyString::pop_back()
{
	if (sz == 0) {
		throw std::exception("Pop back empty string");
	}
	--sz;
	str[sz] = '\0';
}

void MyString::clear()
{
	sz = 0;
	str[sz] = '\0';
}

MyString& MyString::append(size_t count, char ch)
{
	if (cap - sz < count)
	{
		reserve(sz + count);
	}
	for (size_t i = 0; i < count; i++) {
		str[sz + i] = ch;
	}
	sz += count;
	str[sz] = '\0';
	return *this;
}

char& MyString::front()
{
	return str[0];
}

char& MyString::back()
{
	return str[sz - 1];
}

const char* MyString::data() const
{
	return str;
}

const char* MyString::c_str() const
{
	return str;
}

bool MyString::empty() const
{
	return sz == 0;
}

MyString::iterator MyString::insert(iterator pos, const char value)
{
	if (sz == cap) {
		if (cap == 0) reserve(1);
		else reserve(cap * 2);
	}
	for (size_t i = sz; i > pos - this->begin(); i--) {
		str[i] = str[i - 1];
	}
	str[pos - str] = value;
	sz++;
	str[sz] = '\0';
	return pos;
}

MyString::iterator MyString::insert(iterator pos, size_t count, const char value)
{
	size_t dist = std::distance(this->begin(), pos);
	if (sz + count > cap) {
		if (cap == 0) reserve(1);
		else reserve(cap * 2);
	}
	for (size_t i = sz - 1; i >= dist; --i) {
		str[i + count] = str[i];
	}
	for (size_t i = 0; i < count; ++i) {
		str[dist + i] = value;
	}
	sz += count;
	return iterator(str + dist);
}

MyString::iterator MyString::erase(iterator pos)
{
	size_t dist = std::distance(this->begin(), pos);
	for (size_t i = dist; i < sz - 1; ++i) {
		str[i] = str[i + 1];
	}
	--sz;
	return iterator(str + dist);
}

MyString::iterator MyString::erase(iterator first, iterator last)
{
	size_t dist = std::distance(this->begin(), first);
	size_t dist2 = std::distance(first, last);
	for (size_t i = dist; i < sz - dist2; ++i) {
		str[i] = str[i + dist2];
	}
	sz -= dist2;
	return iterator(str + dist);
}

MyString& MyString::operator+=(const MyString& other)
{
	if (sz + other.sz > cap) {
		reserve(sz + other.sz);
	}
	for (size_t i = 0; i < other.sz; i++) {
		str[sz + i] = other.str[i];
	}
	sz += other.sz;
	str[sz] = '\0';
	return *this;
}

MyString& MyString::operator+=(const char* other)
{
	size_t other_sz = my_strlen(other);
	if (sz + other_sz > cap) {
		reserve(sz + other_sz);
	}
	for (size_t i = 0; i < other_sz; i++) {
		str[sz + i] = other[i];
	}
	sz += other_sz;
	str[sz] = '\0';
	return *this;
}

MyString& MyString::operator+=(char ch)
{
	if (sz == cap) {
		if (cap == 0) reserve(1);
		else reserve(cap * 2);
	}
	str[sz] = ch;
	sz++;
	str[sz] = '\0';
	return *this;
}

MyString& MyString::operator=(const MyString& other)
{
	if (this == &other) return *this;
	delete[] str;
	sz = other.sz;
	if (cap < other.cap)
	{
		if (other.cap == 0) reserve(1);
		else reserve(other.cap);
	}
	for (size_t i = 0; i < other.sz; i++) {
		str[i] = other.str[i];
	}
	str[sz] = '\0';
	return *this;
}

MyString& MyString::operator=(const char* other)
{
	if (str == other) return *this;
	delete[] str;
	sz = my_strlen(other);
	cap = sz + 1;
	str = new char[sz + 1];
	for (size_t i = 0; i < sz; i++) {
		str[i] = other[i];
	}
	str[sz] = '\0';
	return *this;
}

MyString& MyString::operator=(char ch)
{
	if (cap == 0) {
		reserve(2);
	}
	sz = 1;
	str[0] = ch;
	str[1] = '\0';
	return *this;
}

MyString::operator std::string() const
{
	return std::string(str);
}

bool operator==(const MyString& left, const MyString& right)
{
	if (left.size() != right.size()) return false;
	for (size_t i = 0; i < left.size(); i++) {
		if (left[i] != right[i]) return false;
	}
	return true;
}

bool operator!=(const MyString& left, const MyString& right)
{
	return !(left == right);
}

bool operator<(const MyString& left, const MyString& right)
{
	for (size_t i = 0; i < left.size(); i++) {
		if (left[i] < right[i]) return true;
		if (left[i] > right[i]) return false;
	}
	return false;
}

bool operator>(const MyString& left, const MyString& right)
{
	return right < left;
}

bool operator<=(const MyString& left, const MyString& right)
{
	return !(left > right);
}

bool operator>=(const MyString& left, const MyString& right)
{
	return !(left < right);
}
