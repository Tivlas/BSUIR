#include "pch.h"
#include "MyString.h"
#include <exception>
#include <cstring>

MyString::MyString(size_t size) :str(new char[size + 1]), sz(size)
{
	str[sz] = '\0';
}

MyString::MyString(size_t size, char ch) : str(new char[size + 1]), sz(size)
{
	for (size_t i = 0; i < sz; i++)
	{
		str[i] = ch;
	}
	str[sz] = '\0';
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
}

MyString::MyString(const char other[])
{
	size_t sz = std::strlen(other);
	this->str = new char[sz + 1];
	memcpy(this->str, other, sz + 1);
}



MyString::~MyString()
{
	delete[] str;
}

void* MyString::memcpy(void* dest, const void* src, size_t n)
{
	char* p1 = (char*)dest;
	const char* p2 = (const char*)src;
	while (n--) {
		*p1++ = *p2++;
	}
	return p1;
}

void* MyString::memmove(void* dest, const void* src, size_t n)
{

	char* p1 = (char*)dest;
	const char* p2 = (const char*)src;
	while (n--) {
		*p1++ = *p2++;
	}
	return p1;
}

char* MyString::strcpy(char* dest, const char* src)
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

char* MyString::strncpy(char* dest, const char* src, size_t n)
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

char* MyString::strcat(char* dest, const char* src)
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

char* MyString::strncat(char* dest, const char* src, size_t n)
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

int MyString::memcmp(const void* s1, const void* s2, size_t n)
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

int MyString::strcmp(const char* s1, const char* s2)
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

int MyString::strcoll(const char* s1, const char* s2)
{
	return std::strcoll(s1, s2);
}

int MyString::strncmp(const char* s1, const char* s2, size_t n)
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

size_t MyString::strxfrm(char* s1, const char* s2, size_t n)
{
	return std::strxfrm(s1, s2, n);
}

bool MyString::findChar(const char* str, char ch)
{
	while (*str) {
		if (*str == ch) {
			return true;
		}
		str++;
	}
	return false;
}

char* MyString::strtok(char* str, const char* delim)
{
	//static char* storage;
	char check1 = '\0';
	char first = *str;
	char* first_address = nullptr;
	char second;
	while (*str) {
		first_address = str;
		first = *str;
		if (*str && !findChar(delim, first)) {
			check1 = 'c';
			break;
		}
		str++;
	}
	if (check1 == '\0') {
		return nullptr;
	}
	char check2 = '\0';
	while (*(++first_address)) {
		second = *first_address;
		if (*first_address && findChar(delim, second)) {
			second = '\0';
			check2 = 'c';
			break;
		}
		first_address++;
	}
	if (check2 != '\0') {
		*(str) = '\0';
	}
	return str;

}


