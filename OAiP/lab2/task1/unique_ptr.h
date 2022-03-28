#pragma once
#include <type_traits>
template <typename T>
class unique_ptr
{
private:
	T* ptr = nullptr;
public:
	explicit unique_ptr(T* ptr) noexcept : ptr(ptr) {}
	unique_ptr() {}

	~unique_ptr()
	{
		delete ptr;
	}


	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;

	unique_ptr(unique_ptr&& other) :ptr(other.ptr) {
		other.ptr = nullptr;
	}

	unique_ptr& operator=(unique_ptr&& other) {
		if (&other == this)
		{
			return *this;
		}
		delete ptr;
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	T& operator*() const {
		return *ptr;
	}
	T* operator->() const {
		return ptr;
	}


	T* release() noexcept {
		T* ptrCopy = ptr;
		ptr = nullptr;
		return ptrCopy;
	}

	void reset(T* pt = nullptr) noexcept
	{
		delete ptr;
		ptr = pt;
	}

	void swap(unique_ptr& other) noexcept {
		std::swap(this->ptr, other.ptr);
	}

	T* get() const noexcept {
		return ptr;
	}

	explicit operator bool() const noexcept {
		return ptr;
	}
};

// unique_ptr<T[]>: specialization for arrays
template <typename T>
class unique_ptr<T[]> {
private:
	T * ptr = nullptr;
public:
	explicit unique_ptr(T* ptr) noexcept : ptr(ptr) {}
	unique_ptr() {}

	~unique_ptr()
	{
		delete[] ptr;
	}

	unique_ptr(const unique_ptr&) = delete;
	
	unique_ptr& operator=(const unique_ptr&) = delete;

	unique_ptr(unique_ptr&& other) :ptr(other.ptr) {
		other.ptr = nullptr;
	}

	unique_ptr& operator=(unique_ptr&& other) {
		if (&other == this)
		{
			return *this;
		}
		delete[] ptr;
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	T& operator*() const {
		return *ptr;
	}

	T* operator->() const {
		return ptr;
	}

	T& operator[](size_t i) {
		return ptr[i];
	}

	T* release() noexcept {
		T* ptrCopy = ptr;
		ptr = nullptr;
		return ptrCopy;
	}

	void reset(T* pt = nullptr) noexcept
	{
		delete[] ptr;
		ptr = pt;
	}

	void swap(unique_ptr& other) noexcept {
		std::swap(this->ptr, other.ptr);
	}

	T* get() const noexcept {
		return ptr;
	}

	explicit operator bool() const noexcept {
		return ptr;
	}
};

template<typename T, typename U>
bool operator==(const unique_ptr<T>& x, const unique_ptr<U>& y)
{
	return x.get() == y.get();
}

template<typename T>
bool operator==(const unique_ptr<T>& x, std::nullptr_t)
{
	return x.get() == nullptr;
}

template<typename T, typename U>
bool operator!=(const unique_ptr<T>& x, const unique_ptr<U>& y)
{
	return x.get() != y.get();
}

template <typename T, typename...Args>
unique_ptr<T> make_unique(Args&&... args) {
	return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
unique_ptr<T> make_unique(size_t size) {
	return unique_ptr<T>(new typename std::remove_extent<T>::type[size]);
}