#pragma once
#include "AVL_Tree.h"
template <typename Key, typename T, typename Compare = std::less<Key>>
class map {
	using value_type = std::pair<const Key, T>;
	using key_type = Key;
	using mapped_type = T;
	using key_compare = Compare;

	using Tree = AVL_Tree<key_type, mapped_type, key_compare>;
	using size_type = typename Tree::size_type;
public:
	using iterator = typename Tree::iterator;
	using const_iterator = typename Tree::const_iterator;
	using reverse_iterator = typename Tree::reverse_iterator;
	using const_reverse_iterator = typename Tree::const_reverse_iterator;
	using list_iterator = typename Tree::list_iterator;
	using const_list_iterator = typename Tree::const_list_iterator;
	using list_reverse_iterator = typename Tree::list_reverse_iterator;
	using const_list_reverse_iterator = typename Tree::const_list_reverse_iterator;

private:
	Tree tree;
public:
	map() = default;
	~map() = default;

	iterator begin() { return tree.begin(); }
	const_iterator cbegin() const { return tree.cbegin(); }
	iterator end() { return tree.end(); }
	const_iterator cend() const { return tree.cend(); }
	reverse_iterator rbegin() { return tree.rbegin(); }
	const_reverse_iterator crbegin() const { return tree.crbegin(); }
	reverse_iterator rend() { return tree.rend(); }
	const_reverse_iterator crend() const { return tree.crend(); }

	list_iterator list_begin() { return tree.list_begin(); }
	const_list_iterator list_cbegin() const { return tree.list_cbegin(); }
	list_iterator list_end() { return tree.list_end(); }
	const_list_iterator list_cend() const { return tree.list_cend(); }
	list_reverse_iterator list_rbegin() { return tree.list_rbegin(); }
	const_list_reverse_iterator list_crbegin() const { return tree.list_crbegin(); }
	list_reverse_iterator list_rend() { return tree.list_rend(); }
	const_list_reverse_iterator list_crend() const { return tree.list_crend(); }

	bool empty() const { return tree.empty(); }
	size_type size() const { return tree.size(); }

	iterator find(const key_type& key) { return tree.find(key); }
	const_iterator find(const key_type& key) const { return tree.find(key); }

	mapped_type& operator[](const key_type& key) {
		return tree[key];
	}

	const mapped_type& operator[](const key_type& key) const {
		return tree[key];
	}

	mapped_type& at(const key_type& key) {
		return tree.at(key);
	}

	const mapped_type& at(const key_type& key) const {
		return tree.at(key);
	}

	size_type count(const key_type& key) const {
		return tree.count(key);
	}

	void clear() { tree.clear(); }

	std::pair<iterator, bool> insert(const value_type& value) { return tree.insert(value); }

	std::pair<iterator, bool> insert(value_type&& value) { return tree.insert(value)); }

	size_type erase(const key_type& key) { return tree.erase(key); }

	iterator erase(iterator pos) { return tree.erase(pos); }

	iterator erase(iterator first, iterator last) { return tree.erase(first, last); }

	bool contains(const key_type& key) const { return tree.contains(key); }

};