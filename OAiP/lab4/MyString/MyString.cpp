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

char& MyString::at(size_t pos)
{
	if (pos >= sz)
		throw std::out_of_range("Out of range");
	return str[pos];
}

void MyString::push_back(char ch)
{
	if (sz == cap) {
		reserve(cap * 2);
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


