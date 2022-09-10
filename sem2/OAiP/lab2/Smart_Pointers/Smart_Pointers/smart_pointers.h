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
	T* ptr = nullptr;
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


//+++++++++++++++++++++++++++++++++++++++++++++++
template<typename T>
struct ControlBlock
{
private:
	T* ptr = nullptr; // pointer to object managed by shared_ptr
	size_t shared_counter = 0;
	size_t weak_counter = 0;
	template<typename U> friend class weak_ptr;
	template<typename U> friend class shared_ptr;
};

//+++++++++++++++++++++++++++++++++++++++++++++++
// shared_ptr                                   +
//+++++++++++++++++++++++++++++++++++++++++++++++

template<typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
private:
	template<typename Y> friend class weak_ptr;

	ControlBlock<T>* ptrCB = nullptr; // pointer to ControlBlock<T>
public:
	shared_ptr() {}

	explicit shared_ptr(T* ptr) : ptrCB(new ControlBlock<T>)
	{
		ptrCB->ptr = ptr;
		++ptrCB->shared_counter;
	}

	explicit shared_ptr(const weak_ptr<T>& ptr) : ptrCB(ptr.ptrCB)
	{
		if (ptrCB)
		{
			++ptrCB->shared_counter;
		}
	}

	shared_ptr(const shared_ptr<T>& other) noexcept {
		ptrCB = other.ptrCB;
		if (ptrCB) {
			++ptrCB->shared_counter;
		}
	}

	shared_ptr& operator=(const shared_ptr<T>& other) noexcept {
		if (ptrCB) {
			--ptrCB->shared_counter;
			if (ptrCB->shared_counter == 0 && ptrCB->weak_counter == 0) {
				delete ptrCB;
			}
		}
		ptrCB = other.ptrCB;
		if (ptrCB) {
			++ptrCB->shared_counter;
		}
		return *this;
	}

	shared_ptr(shared_ptr<T>&& other) noexcept {
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
	}

	shared_ptr& operator=(shared_ptr<T>&& other) noexcept {
		if (ptrCB) {
			--ptrCB->shared_counter;
			if (ptrCB->shared_counter == 0 && ptrCB->weak_counter == 0) {
				delete ptrCB;
			}
		}
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
		return *this;
	}

	T* operator->() const
	{
		return ptrCB->ptr;
	}
	T& operator*() const
	{
		return *(ptrCB->ptr);
	}
	explicit operator bool() const noexcept
	{
		return (ptrCB ? (ptrCB->ptr != nullptr) : 0);
	}

	T* get() const noexcept {
		return (ptrCB ? ptrCB->ptr : nullptr);
	}

	size_t use_count()const noexcept {

		if (ptrCB)
		{
			return ptrCB->shared_counter;
		}
		return 0;
	}

	bool unique() const noexcept
	{
		return this->use_count() == 1;
	}


	void reset() noexcept {
		shared_ptr().swap(*this);
	}

	void reset(T* ptr) {
		shared_ptr(ptr).swap(*this);
	}

	void swap(shared_ptr& other) noexcept {
		std::swap(ptrCB, other.ptrCB);
	}

	bool owner_before(const weak_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	bool owner_before(const shared_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	~shared_ptr() {
		if (!ptrCB || !ptrCB->ptr) {
			return;
		}

		--ptrCB->shared_counter;
		if (ptrCB->shared_counter > 0) {
			return;
		}

		if (ptrCB->weak_counter == 0) {
			delete ptrCB->ptr;
			delete ptrCB;
			return;
		}

		delete ptrCB->ptr;
		ptrCB->ptr = nullptr;
	}
};

// shared_ptr<T[]> specialization for arrays
template <typename T>
class shared_ptr<T[]> {
private:
	template<typename Y> friend class weak_ptr;

	ControlBlock<T>* ptrCB = nullptr;
public:
	shared_ptr() {}

	explicit shared_ptr(T* ptr) : ptrCB(new ControlBlock<T>)
	{
		ptrCB->ptr = ptr;
		++ptrCB->shared_counter;
	}

	explicit shared_ptr(const weak_ptr<T>& ptr) : ptrCB(ptr.CB_ptr)
	{
		if (ptrCB)
		{
			++ptrCB->shared_counter;
		}
	}

	shared_ptr(const shared_ptr<T>& other) noexcept {
		ptrCB = other.ptrCB;
		if (ptrCB) {
			++ptrCB->shared_counter;
		}
	}

	shared_ptr& operator=(const shared_ptr<T>& other) noexcept {
		if (ptrCB) {
			--ptrCB->shared_counter;
			if (ptrCB->shared_counter == 0 && ptrCB->weak_counter == 0) {
				delete ptrCB;
			}
		}
		ptrCB = other.ptrCB;
		if (ptrCB) {
			++ptrCB->shared_counter;
		}
		return *this;
	}

	shared_ptr(shared_ptr<T>&& other) noexcept {
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
	}

	shared_ptr& operator=(shared_ptr<T>&& other) noexcept {
		if (ptrCB) {
			--ptrCB->shared_counter;
			if (ptrCB->shared_counter == 0 && ptrCB->weak_counter == 0) {
				delete ptrCB;
			}
		}
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
		return *this;
	}

	T* operator->() const
	{
		return ptrCB->ptr;
	}
	T& operator*() const
	{
		return *(ptrCB->ptr);
	}
	T& operator[](size_t i) const noexcept
	{
		return ptrCB->ptr[i];
	}

	explicit operator bool() const noexcept
	{
		return (ptrCB ? (ptrCB->ptr != nullptr) : 0);
	}

	T* get() const noexcept {
		return (ptrCB ? ptrCB->ptr : nullptr);
	}

	size_t use_count()const noexcept {

		if (ptrCB)
		{
			return ptrCB->shared_counter;
		}
		return 0;
	}

	bool unique() const noexcept
	{
		return this->use_count() == 1;
	}

	void reset() noexcept {
		shared_ptr().swap(*this);
	}

	void reset(T* ptr) {
		shared_ptr(ptr).swap(*this);
	}

	void swap(shared_ptr& other) noexcept {
		std::swap(ptrCB, other.ptrCB);
	}

	bool owner_before(const weak_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	bool owner_before(const shared_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	~shared_ptr() {
		if (!ptrCB || !ptrCB->ptr) {
			return;
		}

		--ptrCB->shared_counter;
		if (ptrCB->shared_counter > 0) {
			return;
		}

		if (ptrCB->weak_counter == 0) {
			delete[] ptrCB->ptr;
			delete ptrCB;
			return;
		}

		delete[] ptrCB->ptr;
		ptrCB->ptr = nullptr;

	}
};

template< typename T, typename U >
bool operator==(const shared_ptr<T>& x, const shared_ptr<U>& y) noexcept
{
	return x.get() == y.get();
}

template< typename T>
bool operator==(const shared_ptr<T>& x, nullptr_t) noexcept
{
	return x.get() == nullptr;
}

template<typename T>
bool operator!=(const shared_ptr<T>& x, nullptr_t) noexcept
{
	return x.get() != nullptr;
}

template< typename T, typename U >
bool operator!=(const shared_ptr<T>& x, const shared_ptr<U>& y) noexcept
{
	return x.get() != y.get();
}

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
	return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
shared_ptr<T> make_shared(size_t size) {
	return shared_ptr<T>(new typename std::remove_extent<T>::type[size]);
}

//++++++++++++++++++++++++++++++++++++++++++
// week_ptr                                +
//++++++++++++++++++++++++++++++++++++++++++

template <typename T>
class weak_ptr {
private:
	ControlBlock<T>* ptrCB = nullptr;
	template<typename Y> friend class shared_ptr;

public:
	weak_ptr() {}

	weak_ptr(const weak_ptr<T>& other) noexcept {
		ptrCB = other.ptrCB;
		if (ptrCB)
		{
			++ptrCB->weak_counter;
		}
	}

	weak_ptr& operator=(const weak_ptr<T>& other) noexcept
	{
		if (ptrCB)
		{
			--ptrCB->weak_counter;
		}
		ptrCB = other.ptrCB;
		if (ptrCB)
		{
			++ptrCB->weak_counter;
		}
		return *this;
	}

	weak_ptr(const shared_ptr<T>& other) noexcept
	{
		ptrCB = other.ptrCB;
		if (ptrCB)
		{
			++ptrCB->weak_counter;
		}
	}

	weak_ptr& operator=(const shared_ptr<T>& other) noexcept
	{
		if (ptrCB)
		{
			--ptrCB->weak_counter;
		}
		ptrCB = other.ptrCB;
		if (ptrCB)
		{
			++ptrCB->weak_counter;
		}
		return *this;
	}

	weak_ptr(weak_ptr<T>&& other) {
		if (ptrCB)
		{
			--ptrCB->weak_counter;
		}
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
	}

	weak_ptr& operator=(weak_ptr<T>&& other) noexcept
	{
		if (ptrCB)
		{
			--ptrCB->weak_counter;
		}
		ptrCB = other.ptrCB;
		other.ptrCB = nullptr;
		return *this;
	}

	void reset() noexcept {
		if (ptrCB)
		{
			--ptrCB->weak_counter;
		}
		ptrCB = nullptr;
	}

	void swap(weak_ptr& other) noexcept {
		std::swap(ptrCB, other.ptrCB);
	}

	size_t use_count() const
	{
		if (ptrCB)
		{
			return ptrCB->shared_counter;
		}
		return 0;
	}

	bool expired()
	{
		return (!ptrCB || ptrCB->shared_counter == 0);
	}

	shared_ptr<T> lock() noexcept {
		return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
	}

	bool owner_before(const weak_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	bool owner_before(const shared_ptr<T>& other) const noexcept
	{
		return ptrCB < other.ptrCB;
	}

	~weak_ptr()
	{
		if (!ptrCB)
		{
			return;
		}
		--ptrCB->weak_counter;
		if (ptrCB->shared_counter == 0 && ptrCB->weak_counter == 0)
		{
			delete ptrCB;
			ptrCB = nullptr;
		}
	}
};
