#pragma once
#pragma once

#ifdef MyString_EXPORTS
#define MyString_API __declspec(dllexport)
#else
#define MyString_API __declspec(dllimport)
#endif

class MyString_API MyString
{
private:
	char* str = nullptr;
	size_t sz = 0;
	//size_t cap = 0;
public:
	MyString() {}
	MyString(size_t size);
	MyString(size_t size, char ch);
	//MyString(const char*);
	MyString(const MyString&);
	~MyString();

	void* memcpy(void* s1, const void* s2, size_t n);
	void* memmove(void* s1, const void* s2, size_t n);
	char* strcpy(char* dest, const char* src);
	char* strncpy(char* dest, const char* src, size_t n);
	char* strcat(char* dest, const char* src);
	char* strncat(char* dest, const char* src, size_t n);
	int memcmp(const void* s1, const void* s2, size_t n);
	int strcmp(const char* s1, const char* s2);
	int strcoll(const char* s1, const char* s2);
	int strncmp(const char* s1, const char* s2, size_t n);
	size_t strxfrm(char* s1, const char* s2, size_t n);
	char* strtok(char* s1, const char* s2);


};

