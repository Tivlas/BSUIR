#pragma once
template <typename T1, typename T2>
struct MyPair {
public:
	T1 first;
	T2 second;

	MyPair() {}

	MyPair(const T1& first, const T2& second) : first(first), second(second) {}

	MyPair(T1&& first, T2&& second) : first(std::forward<T1>(first)), second(std::forward<T2>(second)) {}

	MyPair(const MyPair& other) : first(other.first), second(other.second) {}

	MyPair(MyPair&& other) : first(std::move(other.first)), second(std::move(other.second)) {}

	MyPair& operator=(const MyPair& other) {
		first = other.first;
		second = other.second;
		return *this;
	}

	MyPair& operator=(MyPair&& other) {
		first = std::move(other.first);
		second = std::move(other.second);
		return *this;
	}

	void swap(MyPair& other) {
		if (this != std::addressof(other))
		{
			std::swap(first, other.first);
			std::swap(second, other.second);
		}
	}
};

template <typename T1, typename T2>
MyPair<T1, T2> make_pair(T1& t, T2& u) {
	return MyPair<T1, T2>(t, u);
}


template <typename T1, typename T2>
MyPair<T1, T2> make_pair(T1&& t, T2&& u) {
	return MyPair<T1, T2>(std::forward<T1>(t), std::forward<T2>(u));
}


template <typename T1, typename T2>
void swap(MyPair<T1, T2>& left, MyPair<T1, T2>& right) {
	left.swap(right);
}


template <typename T1, typename T2>
bool operator==(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return left.first == right.first && left.second == right.second;
}


template <typename T1, typename T2>
bool operator!=(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return !(left == right);
}


template <typename T1, typename T2>
bool operator<(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return left.first < right.first || (!(right.first < left.first) && left.second < right.second);
}


template <typename T1, typename T2>
bool operator>(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return right < left;
}


template <typename T1, typename T2>
bool operator>=(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return !(left < right);
}


template <typename T1, typename T2>
bool operator<=(const MyPair<T1, T2>& left, const MyPair<T1, T2>& right) {
	return !(right < left);
}