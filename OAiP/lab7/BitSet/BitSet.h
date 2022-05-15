#pragma once
#include <iostream>
#include <string>
template <size_t N>
class BitSet {
private:
	size_t sz = (N + 63) / 64;
	uint64_t bits[(N + 63) / 64];
public:
	// helper class
	class reference {
	private:
		BitSet& bs;
		size_t pos;
	public:
		reference(BitSet& bs, size_t pos) : bs(bs), pos(pos) {}

		reference& operator=(bool b) {
			bs.set(pos, b);
			return *this;
		}

		reference& operator=(const reference& ref) {
			bs.set(pos, ref.bs.test(pos));
			return *this;
		}

		operator bool() const {
			return bs.test(pos);
		}

		reference& operator~() {
			bs.flip(pos);
			return *this;
		}

		reference& flip() {
			bs.flip(pos);
			return *this;
		}
	};
public:

	BitSet() {
		for (size_t i = 0; i < sz; i++) {
			bits[i] = 0ULL;
		}
	}

	BitSet(const BitSet& other) {
		for (size_t i = 0; i < sz; i++) {
			bits[i] = other.bits[i];
		}
	}

	BitSet(const char* str) {
		reset();
		size_t count = strlen(str);
		if (count > N) {
			count = N;
		}
		for (size_t i = 0; i < count; ++i) {
			if (str[count - i - 1] == '1') {
				set(i);
			}
		}
	}

	BitSet(const std::string& s) {
		reset();
		size_t count = s.size();
		if (count > N) {
			count = N;
		}
		for (size_t i = 0; i < count; ++i) {
			if (s[count - i - 1] == '1') {
				set(i);
			}
		}
	}

	BitSet(unsigned long long value) {
		reset();
		bits[0] = value;
		trim();
	}

	bool operator[](size_t pos) const {
		if (pos >= N) {
			throw std::out_of_range("BitSet index out of range");
		}
		return (bits[pos / 64] & (1ULL << (pos % 64))) != 0;
	}

	reference operator[](size_t pos) {
		if (pos >= N) {
			throw std::out_of_range("BitSet index out of range");
		}
		return reference(*this, pos);
	}

	bool all() const noexcept {
		if (N == 0) {
			return true;
		}
		bool no_padding = N % 64 == 0;
		for (size_t i = 0; i < sz - 1 + no_padding; ++i) {
			if (bits[i] != ~0ULL) {
				return false;
			}
		}
		return no_padding || bits[sz - 1] == (1ULL << (N % 64)) - 1;
	}

	bool any() const noexcept {
		for (size_t i = 0; i < sz; i++) {
			if (bits[i] != 0ULL) {
				return true;
			}
		}
		return false;
	}

	bool none() const noexcept {
		return !any();
	}

	size_t count() const noexcept {
		size_t c = 0;
		for (size_t i = 0; i < sz; i++) {
			c += __popcnt64(bits[i]);
		}
		return c;
	}

	BitSet& flip() noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] = ~bits[i];
		}
		trim();
		return *this;
	}

	BitSet& flip(size_t pos) {
		bits[pos / 64] ^= (1ULL << (pos % 64));
		return *this;
	}

	BitSet& reset() noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] = 0ULL;
		}
		return *this;
	}

	BitSet& reset(size_t pos) {
		bits[pos / 64] &= ~(1ULL << (pos % 64));
		return *this;
	}

	BitSet& set() noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] = ULLONG_MAX;
		}
		trim();
		return *this;
	}

	BitSet& set(size_t pos, bool value = true) {
		if (value) {
			bits[pos / 64] |= (1ULL << (pos % 64));
		}
		else {
			bits[pos / 64] &= ~(1ULL << (pos % 64));
		}
		return *this;
	}

	size_t size() const noexcept {
		return N;
	}

	bool test(size_t pos) const {
		if (pos >= N) {
			throw std::out_of_range("invalid bitset position");
		}
		return (bits[pos / 64] & (1ULL << (pos % 64))) != 0;
	}

	std::string to_string(char zero = '0', char one = '1') const {
		std::string s;
		s.reserve(N);
		for (size_t pos = N; pos > 0;) {
			s.push_back(test(--pos) ? one : zero);
		}
		return s;
	}

	unsigned long long to_ullong() const {
		if (N == 0) {
			return 0;
		}
		else {
			if (N > 64) {
				for (size_t i = 1; i < sz; i++) {
					if (bits[i] != 0) {
						throw std::overflow_error("BitSet to_ullong overflow error");
					}
				}
			}
			return bits[0];
		}
	}

	unsigned long to_ulong() const {
		bool bits_zero = N == 0;
		bool bits_small = N <= 32;
		bool bits_large = N > 64;
		if (bits_zero) {
			return 0;
		}
		else if (bits_small) {
			return static_cast<unsigned long>(bits[0]);
		}
		else {
			if (bits_large) {
				for (size_t i = 1; i < sz; i++) {
					if (bits[i] != 0) {
						throw std::overflow_error("BitSet to_ulong overflow error");
					}
				}
			}
			if (bits[0] > ULONG_MAX) {
				throw std::overflow_error("BitSet to_ulong overflow error");
			}
			return static_cast<unsigned long>(bits[0]);
		}
	}

	bool operator==(const BitSet& other) const noexcept {
		return memcmp(&bits[0], &other.bits[0], sizeof(bits)) == 0;
	}

	bool operator!=(const BitSet& other) const noexcept {
		return !(*this == other);
	}

	BitSet& operator &=(const BitSet& other) noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] &= other.bits[i];
		}
		return *this;
	}

	BitSet& operator |=(const BitSet& other) noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] |= other.bits[i];
		}
		return *this;
	}

	BitSet& operator ^=(const BitSet& other) noexcept {
		for (size_t i = 0; i < sz; i++) {
			bits[i] ^= other.bits[i];
		}
		return *this;
	}

	BitSet operator~() const noexcept {
		BitSet result(*this);
		result.flip();
		return result;
	}

	BitSet& operator<<=(size_t pos) noexcept {
		const std::ptrdiff_t elem_shift = pos / 64;
		if (elem_shift != 0) {
			for (std::ptrdiff_t i = sz - 1; i >= 0; --i) {
				bits[i] = elem_shift <= i ? bits[i - elem_shift] : 0;
			}
		}
		if ((pos %= 64) != 0) {
			for (std::ptrdiff_t i = sz - 1; i > 0; --i) {
				bits[i] = (bits[i] << pos) | (bits[i - 1] >> (64 - pos));
			}
			bits[0] <<= pos;
		}
		trim();
		return *this;
	}

	BitSet& operator>>=(size_t pos) noexcept {
		const std::ptrdiff_t elem_shift = pos / 64;
		if (elem_shift != 0) {
			for (std::ptrdiff_t i = 0; i < sz; ++i) {
				bits[i] = elem_shift <= sz - 1 - i ? bits[i + elem_shift] : 0;
			}
		}
		if ((pos %= 64) != 0) {
			for (std::ptrdiff_t i = 0; i < sz - 1; ++i) {
				bits[i] = (bits[i] >> pos) | (bits[i + 1] << (64 - pos));
			}
			bits[sz - 1] >>= pos;
		}
		trim();
		return *this;
	}

	BitSet operator<<(size_t pos) const noexcept {
		BitSet temp(*this);
		temp <<= pos;
		return temp;
	}

	BitSet operator>>(size_t pos) const noexcept {
		BitSet temp(*this);
		temp >>= pos;
		return temp;
	}
private:
	// helper methods
	void trim() {  // helper for set()
		bool need_to_correct_last_element_bits = N == 0 || N % 64 != 0;
		if (need_to_correct_last_element_bits) {
			bits[sz - 1] &= (1ULL << N % 64) - 1;
		}
	}
};

template <size_t N>
BitSet<N> operator&(const BitSet<N>& left, const BitSet<N>& right) noexcept {
	BitSet<N> res(left);
	res &= right;
	return res;
}

template <size_t N>
BitSet<N> operator|(const BitSet<N>& left, const BitSet<N>& right) noexcept {
	BitSet<N> res(left);
	res |= right;
	return res;
}

template <size_t N>
BitSet<N> operator^(const BitSet<N>& left, const BitSet<N>& right) noexcept {
	BitSet<N> res(left);
	res ^= right;
	return res;
}

template <size_t N>
std::ostream& operator<<(std::ostream& os, const BitSet<N>& bs) {
	os << bs.to_string();
	return os;
}

template <size_t N>
std::istream& operator>>(std::istream& is, BitSet<N>& bs) {
	std::string str;
	is >> str;
	bs.set(str);
	return is;
}

