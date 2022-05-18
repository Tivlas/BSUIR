#pragma once
#include <functional>
#include <list>
#include <type_traits>
#include <algorithm>
template <typename Key, typename T, typename Compare = std::less<Key>>
class AVL_Tree {
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using key_compare = Compare;

public:
	struct AVL_Node;

	class Iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		AVL_Node* cur = nullptr;
		AVL_Node* get_node() const {
			return cur;
		}
	public:
		Iterator() {}
		Iterator(AVL_Node* cur) : cur(cur) {}
		Iterator(const Iterator& other) : cur(other.cur) {}
		Iterator(Iterator&& other) : cur(other.cur) {
			other.cur = nullptr;
		}
		Iterator& operator=(const Iterator& other) {
			cur = other.cur;
			return *this;
		}
		Iterator& operator=(Iterator&& other) noexcept {
			cur = other.cur;
			other.cur = nullptr;
			return *this;
		}

		reference operator*() {
			return cur->value_;
		}

		pointer operator->() {
			return std::addressof(cur->value_);
		}

		Iterator& operator++() {
			cur = cur->next(cur);
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp(*this);
			cur = cur->next(cur);
			return tmp;
		}

		Iterator& operator--() {
			cur = cur->prev(cur);
			return *this;
		}

		Iterator operator--(int) {
			Iterator tmp(*this);
			cur = cur->prev(cur);
			return tmp;
		}

		bool operator==(const Iterator& other) const {
			return cur == other.cur;
		}

		bool operator!=(const Iterator& other) const {
			return !(cur == other.cur);
		}
	};

	class Const_Iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = const value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		AVL_Node* cur = nullptr;
		AVL_Node* get_node() const {
			return cur;
		}
	public:
		Const_Iterator() {}
		Const_Iterator(AVL_Node* cur) : cur(cur) {}
		Const_Iterator(const Const_Iterator& other) : cur(other.cur) {}
		Const_Iterator(Const_Iterator&& other) : cur(other.cur) {
			other.cur = nullptr;
		}
		Const_Iterator& operator=(const Const_Iterator& other) {
			cur = other.cur;
			return *this;
		}
		Const_Iterator& operator=(Const_Iterator&& other) {
			cur = other.cur;
			other.cur = nullptr;
			return *this;
		}

		reference operator*() {
			return cur->value_;
		}

		pointer operator->() {
			return std::addressof(cur->value_);
		}

		Const_Iterator& operator++() {
			cur = cur->next(cur);
			return *this;
		}

		Const_Iterator operator++(int) {
			Const_Iterator tmp(*this);
			cur = cur->next(cur);
			return tmp;
		}

		Const_Iterator& operator--() {
			cur = cur->prev(cur);
			return *this;
		}

		Const_Iterator operator--(int) {
			Const_Iterator tmp(*this);
			cur = cur->prev(cur);
			return tmp;
		}

		bool operator==(const Const_Iterator& other) const {
			return cur == other.cur;
		}

		bool operator!=(const Const_Iterator& other) const {
			return !(cur == other.cur);
		}
	};

	class Reverse_Iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		Iterator it;
	public:
		Reverse_Iterator() {}
		Reverse_Iterator(Iterator it) {
			this->it = --it;
		}
		Reverse_Iterator(const Reverse_Iterator& other) : it(other.it) {}
		Reverse_Iterator& operator=(const Reverse_Iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *it;
		}

		pointer operator->() {
			return std::addressof(*it);
		}

		Reverse_Iterator& operator++() {
			--it;
			return *this;
		}

		Reverse_Iterator operator++(int) {
			Reverse_Iterator tmp(*this);
			--it;
			return tmp;
		}

		Reverse_Iterator& operator--() {
			++it;
			return *this;
		}

		Reverse_Iterator operator--(int) {
			Reverse_Iterator tmp(*this);
			++it;
			return tmp;
		}

		bool operator==(const Reverse_Iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const Reverse_Iterator& other) const {
			return !(it == other.it);
		}
	};

	class Const_Reverse_Iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = const value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		Const_Iterator it;
	public:
		Const_Reverse_Iterator() {}
		Const_Reverse_Iterator(Const_Iterator it) {
			this->it = --it;
		}
		Const_Reverse_Iterator(const Const_Reverse_Iterator& other) : it(other.it) {}
		Const_Reverse_Iterator& operator=(const Const_Reverse_Iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *it;
		}

		pointer operator->() {
			return std::addressof(*it);
		}

		Const_Reverse_Iterator& operator++() {
			--it;
			return *this;
		}

		Const_Reverse_Iterator operator++(int) {
			Const_Reverse_Iterator tmp(*this);
			--it;
			return tmp;
		}

		Const_Reverse_Iterator& operator--() {
			++it;
			return *this;
		}

		Const_Reverse_Iterator operator--(int) {
			Const_Reverse_Iterator tmp(*this);
			++it;
			return tmp;
		}

		bool operator==(const Const_Reverse_Iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const Const_Reverse_Iterator& other) const {
			return !(it == other.it);
		}
	};

	class list_iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		typename std::list<AVL_Node*>::iterator it;
	public:
		list_iterator() {}
		list_iterator(typename std::list<AVL_Node*>::iterator it) : it(it) {}
		list_iterator(const list_iterator& other) : it(other.it) {}
		list_iterator& operator=(const list_iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *(*it);
		}

		pointer operator->() {
			return std::addressof((*it)->value_);
		}

		list_iterator& operator++() {
			++it;
			return *this;
		}

		list_iterator operator++(int) {
			list_iterator tmp(*this);
			++it;
			return tmp;
		}

		list_iterator& operator--() {
			--it;
			return *this;
		}

		list_iterator operator--(int) {
			list_iterator tmp(*this);
			--it;
			return tmp;
		}

		bool operator==(const list_iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const list_iterator& other) const {
			return !(it == other.it);
		}
	};

	class const_list_iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = const value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		typename std::list<AVL_Node*>::const_iterator it;
	public:
		const_list_iterator() {}
		const_list_iterator(typename std::list<AVL_Node*>::const_iterator it) : it(it) {}
		const_list_iterator(const const_list_iterator& other) : it(other.it) {}
		const_list_iterator& operator=(const const_list_iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *(*it);
		}

		pointer operator->() {
			return std::addressof((*it)->value_);
		}

		const_list_iterator& operator++() {
			++it;
			return *this;
		}

		const_list_iterator operator++(int) {
			const_list_iterator tmp(*this);
			++it;
			return tmp;
		}

		const_list_iterator& operator--() {
			--it;
			return *this;
		}

		const_list_iterator operator--(int) {
			const_list_iterator tmp(*this);
			--it;
			return tmp;
		}

		bool operator==(const const_list_iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const const_list_iterator& other) const {
			return !(it == other.it);
		}
	};

	class list_reverse_iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		typename std::list<AVL_Node*>::reverse_iterator it;
	public:
		list_reverse_iterator() {}
		list_reverse_iterator(typename std::list<AVL_Node*>::reverse_iterator it) : it(it) {}
		list_reverse_iterator(const list_reverse_iterator& other) : it(other.it) {}
		list_reverse_iterator& operator=(const list_reverse_iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *(*it);
		}

		pointer operator->() {
			return std::addressof((*it)->value_);
		}

		list_reverse_iterator& operator++() {
			++it;
			return *this;
		}

		list_reverse_iterator operator++(int) {
			list_reverse_iterator tmp(*this);
			++it;
			return tmp;
		}

		list_reverse_iterator& operator--() {
			--it;
			return *this;
		}

		list_reverse_iterator operator--(int) {
			list_reverse_iterator tmp(*this);
			--it;
			return tmp;
		}

		bool operator==(const list_reverse_iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const list_reverse_iterator& other) const {
			return !(it == other.it);
		}
	};

	class const_list_reverse_iterator {
		friend class AVL_Tree;
		using iterator_category = std::bidirectional_iterator_tag;
		using val_t = const value_type;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		typename std::list<AVL_Node*>::const_reverse_iterator it;
	public:
		const_list_reverse_iterator() {}
		const_list_reverse_iterator(typename std::list<AVL_Node*>::const_reverse_iterator it) : it(it) {}
		const_list_reverse_iterator(const const_list_reverse_iterator& other) : it(other.it) {}
		const_list_reverse_iterator& operator=(const const_list_reverse_iterator& other) {
			it = other.it;
			return *this;
		}

		reference operator*() {
			return *(*it);
		}

		pointer operator->() {
			return std::addressof((*it)->value_);
		}

		const_list_reverse_iterator& operator++() {
			++it;
			return *this;
		}

		const_list_reverse_iterator operator++(int) {
			const_list_reverse_iterator tmp(*this);
			++it;
			return tmp;
		}

		const_list_reverse_iterator& operator--() {
			--it;
			return *this;
		}

		const_list_reverse_iterator operator--(int) {
			const_list_reverse_iterator tmp(*this);
			--it;
			return tmp;
		}

		bool operator==(const const_list_reverse_iterator& other) const {
			return it == other.it;
		}

		bool operator!=(const const_list_reverse_iterator& other) const {
			return !(it == other.it);
		}
	};


	using const_iterator = Const_Iterator;
	using const_reverse_iterator = Const_Reverse_Iterator;
	using iterator = std::conditional_t<std::is_same_v<key_type, mapped_type>, const_iterator, Iterator>;
	using reverse_iterator = std::conditional_t<std::is_same_v<key_type, mapped_type>, const_reverse_iterator, Reverse_Iterator>;
private:
	// AVL_NODE
	struct AVL_Node
	{
		friend class AVL_Tree;
		value_type value_;
		AVL_Node(const value_type& value, bool is_not_fake = true) : value_(value), is_not_fake_(is_not_fake) {}
		AVL_Node(value_type&& value, bool is_not_fake = true) : value_(std::move(value)), is_not_fake_(is_not_fake) {}

		bool operator== (const AVL_Node& other) const {
			return parent_ == other.parent_;
		}

		bool operator!= (const AVL_Node& other) const {
			return !(*this == other);
		}
	private:
		bool is_not_fake_ = true;
		AVL_Node* left_ = nullptr;
		AVL_Node* right_ = nullptr;
		AVL_Node* parent_ = nullptr;
		int height_ = 1;

		AVL_Node* next(AVL_Node* node) {
			if (node->right_) {
				if (node->right_->is_not_fake_ == false) {
					return node->right_;
				}
				node = node->right_;
				while (node->left_ && node->left_->is_not_fake_ == true) {
					node = node->left_;
				}
			}
			else {
				while (node->parent_ && node->parent_->right_ == node) {
					node = node->parent_;
				}
				node = node->parent_;
			}
			return node;
		}

		AVL_Node* prev(AVL_Node* node) {
			if (node->left_) {
				if (node->left_->is_not_fake_ == false) {
					return node->left_;
				}
				node = node->left_;
				while (node->right_ && node->right_->is_not_fake_ == true) {
					node = node->right_;
				}
			}
			else {
				while (node->parent_ && node->parent_->left_ == node) {
					node = node->parent_;
				}
				node = node->parent_;
			}
			return node;
		}
	};

	key_compare k_cmp;
	AVL_Node* root_ = nullptr;
	AVL_Node* begin_node_ = nullptr;
	AVL_Node* fake_begin_node_ = nullptr;
	AVL_Node* end_node_ = nullptr;
	AVL_Node* fake_end_node_ = nullptr;
	size_type size_ = 0;
	std::list<AVL_Node*> elem_list_;
public:
	AVL_Tree() : k_cmp(key_compare()) {
		fake_begin_node_ = new AVL_Node(value_type(), false);
		fake_end_node_ = new AVL_Node(value_type(), false);
		fake_begin_node_->height_ = 0;
		fake_end_node_->height_ = 0;
	}

	AVL_Tree(const key_compare& k_cmp) : k_cmp(k_cmp) {
		fake_begin_node_ = new AVL_Node(value_type(), false);
		fake_end_node_ = new AVL_Node(value_type(), false);
		fake_begin_node_->height_ = 0;
		fake_end_node_->height_ = 0;
	}

	std::pair<iterator, bool> insert(const value_type& value) {
		AVL_Node* new_node = new AVL_Node(value);
		bool inserted = false;
		iterator it;
		root_ = help_insert(root_, new_node, inserted, it);
		if (inserted)
		{
			list_insert(elem_list_, it.get_node());
			update_begin_end_nodes(it.get_node());
			update_root_parent();
			++size_;
		}
		return std::make_pair(it, inserted);
	}

	std::pair<iterator, bool> insert(value_type&& value) {
		AVL_Node* new_node = new AVL_Node(std::move(value));
		bool inserted = false;
		iterator it;
		root_ = help_insert(root_, new_node, inserted, it);
		if (inserted)
		{
			list_insert(elem_list_, it.get_node());
			update_begin_end_nodes(it.get_node());
			update_root_parent();
			++size_;
		}
		return std::make_pair(it, inserted);
	}

	size_type erase(const key_type& key) {
		if (root_ == nullptr) {
			return 0;
		}
		size_type count = 0;
		root_ = remove(root_, key, count);
		if (count != 0)
		{
			list_erase(elem_list_, key);
			update_begin_end_nodes_after_erase();
			update_root_parent();
			--size_;
		}
		return count;
	}

	iterator erase(iterator pos) {
		key_type key = pos.get_node()->value_.first;
		size_type count = 0;
		iterator* it = new iterator();
		root_ = remove(root_, key, count, it);
		list_erase(elem_list_, key);
		update_begin_end_nodes_after_erase();
		update_root_parent();
		--size_;
		return *it;
	}

	iterator erase(iterator first, iterator last) {
		while (first != last) {
			first = erase(first);
		}
		return last;
	}

	size_type size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	void clear() {
		root_ = nullptr;
		begin_node_ = nullptr;
		end_node_ = nullptr;
		fake_begin_node_ = nullptr;
		fake_end_node_ = nullptr;
		size_ = 0;
	}

	iterator find(const key_type& key) {
		AVL_Node* cur = root_;
		while (cur != nullptr) {
			if (k_cmp(key, cur->value_.first)) {
				cur = cur->left_;
			}
			else if (k_cmp(cur->value_.first, key)) {
				cur = cur->right_;
			}
			else {
				return iterator(cur);
			}
		}
		return end();
	}

	const_iterator find(const key_type& key) const {
		AVL_Node* cur = root_;
		while (cur != nullptr) {
			if (k_cmp(key, cur->value_.first)) {
				cur = cur->left_;
			}
			else if (k_cmp(cur->value_.first, key)) {
				cur = cur->right_;
			}
			else {
				return const_iterator(cur);
			}
		}
		return cend();
	}

	bool contains(const key_type& key) const {
		return find(key) != cend();
	}

	size_type count(const key_type& key) const {
		if (root_ == nullptr) {
			return 0;
		}
		if (find(key) != cend()) {
			return 1;
		}
		return 0;
	}

	mapped_type& at(const key_type& key) {
		auto it = find(key);
		if (it == end()) {
			throw std::out_of_range("No such key");
		}
		return it->second;
	}

	const mapped_type& at(const key_type& key) const {
		auto it = find(key);
		if (it == cend()) {
			throw std::out_of_range("No such key");
		}
		return it->second;
	}

	// OPERATOR[]
	mapped_type& operator[](const key_type& key) {
		auto it = find(key);
		if (it == end()) {
			auto iter = insert({ key, mapped_type() });
			return iter.first->second;
		}
		return it->second;
	}

	const mapped_type& operator[](const key_type& key) const {
		auto it = find(key);
		if (it == cend()) {
			auto iter = insert({ key, mapped_type() });
			return iter.first->second;
		}
		return it->second;
	}

	template <typename Container> // the method works with a container that allows u to do push_back
	void traverse_inorder(Container& cnt) const {
		help_traverse_inorder(root_, cnt);
	}

	iterator begin() {
		return iterator(begin_node_);
	}

	const_iterator cbegin() const {
		return const_iterator(begin_node_);
	}

	iterator end() {
		return iterator(fake_end_node_);
	}

	const_iterator cend() const {
		return const_iterator(fake_end_node_);
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator crend() const {
		return const_reverse_iterator(cbegin());
	}

	list_iterator list_begin() {
		return list_iterator(elem_list_.begin());
	}

	list_iterator list_end() {
		return list_iterator(elem_list_.end());
	}

	const_list_iterator list_cbegin() const {
		return const_list_iterator(elem_list_.cbegin());
	}

	const_list_iterator list_cend() const {
		return const_list_iterator(elem_list_.cend());
	}

	list_reverse_iterator list_rbegin() {
		return list_reverse_iterator(elem_list_.rbegin());
	}

	list_reverse_iterator list_rend() {
		return list_reverse_iterator(elem_list_.rend());
	}

	const_list_reverse_iterator list_crbegin() const {
		return const_list_reverse_iterator(elem_list_.crbegin());
	}

	const_list_reverse_iterator list_crend() const {
		return const_list_reverse_iterator(elem_list_.crend());
	}

private:
	// HELPER METHODS

	int max(int a, int b) {
		return a > b ? a : b;
	}

	int get_height(AVL_Node* node) {
		return node ? node->height_ : 0;
	}

	int get_balance_factor(AVL_Node* node) {
		return get_height(node->right_) - get_height(node->left_);
	}

	void update_height(AVL_Node* node) {
		node->height_ = max(get_height(node->left_), get_height(node->right_)) + 1;
	}

	AVL_Node* rotate_left(AVL_Node* node_to_rotate_around) {
		AVL_Node* right_child = node_to_rotate_around->right_;
		node_to_rotate_around->right_ = right_child->left_;
		if (right_child->left_) {
			right_child->left_->parent_ = node_to_rotate_around;
		}
		right_child->left_ = node_to_rotate_around;
		right_child->parent_ = node_to_rotate_around->parent_;
		if (node_to_rotate_around->parent_) {
			if (node_to_rotate_around->parent_->left_ == node_to_rotate_around) {
				node_to_rotate_around->parent_->left_ = right_child;
			}
			else {
				node_to_rotate_around->parent_->right_ = right_child;
			}
		}
		node_to_rotate_around->parent_ = right_child;
		update_height(node_to_rotate_around);
		update_height(right_child);
		return right_child;
	}

	AVL_Node* rotate_right(AVL_Node* node_to_rotate_around) {
		AVL_Node* left_child = node_to_rotate_around->left_;
		node_to_rotate_around->left_ = left_child->right_;
		if (left_child->right_) {
			left_child->right_->parent_ = node_to_rotate_around;
		}
		left_child->right_ = node_to_rotate_around;
		left_child->parent_ = node_to_rotate_around->parent_;
		if (node_to_rotate_around->parent_) {
			if (node_to_rotate_around->parent_->left_ == node_to_rotate_around) {
				node_to_rotate_around->parent_->left_ = left_child;
			}
			else {
				node_to_rotate_around->parent_->right_ = left_child;
			}
		}
		node_to_rotate_around->parent_ = left_child;
		update_height(node_to_rotate_around);
		update_height(left_child);
		return left_child;
	}

	AVL_Node* balance(AVL_Node* node_to_balance) {
		update_height(node_to_balance);
		if (get_balance_factor(node_to_balance) == 2) {
			if (get_balance_factor(node_to_balance->right_) < 0) {
				node_to_balance->right_ = rotate_right(node_to_balance->right_);
			}
			return rotate_left(node_to_balance);
		}
		else if (get_balance_factor(node_to_balance) == -2) {
			if (get_balance_factor(node_to_balance->left_) > 0) {
				node_to_balance->left_ = rotate_left(node_to_balance->left_);
			}
			return rotate_right(node_to_balance);
		}
		return node_to_balance;
	}

	AVL_Node* find_min(AVL_Node* node) {
		return (node->left_ && node->left_->is_not_fake_ != false) ? find_min(node->left_) : node;
	}

	AVL_Node* remove_min(AVL_Node* node) {
		if (node->left_ == nullptr || node->left_->is_not_fake_ == false) {
			if (node->right_) {
				node->right_->parent_ = node->parent_;
			}
			return node->right_;
		}
		node->left_ = remove_min(node->left_);
		return balance(node);
	}

	AVL_Node* remove(AVL_Node* root, const key_type& key, size_type& count, iterator* it = new iterator()) {
		if (root == nullptr || root->is_not_fake_ == false) {
			return nullptr;
		}
		if (k_cmp(key, root->value_.first)) {
			root->left_ = remove(root->left_, key, count, it);
		}
		else if (k_cmp(root->value_.first, key)) {
			root->right_ = remove(root->right_, key, count, it);
		}
		else {
			AVL_Node* left = root->left_;
			if (left && left->is_not_fake_ == false) {
				left = nullptr;
			}
			AVL_Node* right = root->right_;
			if (right && right->is_not_fake_ == false) {
				right = nullptr;
			}
			AVL_Node* temp_parent = root->parent_;
			*it = iterator(root->next(root));
			root = nullptr; // will be deleted from the elem_list_
			count = 1;
			if (!right) {
				if (left)
				{
					left->parent_ = temp_parent;
				}
				return left;
			}
			if (!left) {
				right->parent_ = temp_parent;
				return right;
			}
			AVL_Node* min = find_min(right);
			min->right_ = remove_min(right);
			if (right != min)
			{
				right->parent_ = min;
			}
			else {
				right->parent_ = temp_parent;
			}
			min->left_ = left;
			left->parent_ = min;
			min->parent_ = temp_parent;
			return balance(min);
		}
		return balance(root);
	}

	AVL_Node* help_insert(AVL_Node* root, AVL_Node* node_to_insert, bool& inserted, iterator& it, AVL_Node* parent = nullptr) {
		if (root == nullptr || root->is_not_fake_ == false) {
			root = node_to_insert;
			root->parent_ = parent;
			inserted = true;
			it = iterator(root);
			return root;
		}
		else if (k_cmp(node_to_insert->value_.first, root->value_.first)) {
			root->left_ = help_insert(root->left_, node_to_insert, inserted, it, root);
		}
		else if (k_cmp(root->value_.first, node_to_insert->value_.first)) {
			root->right_ = help_insert(root->right_, node_to_insert, inserted, it, root);

		}
		else {
			inserted = false;
			it = iterator(root);
		}
		return balance(root);
	}

	template <typename Container>
	void help_traverse_inorder(AVL_Node* start_node, Container& cnt) const {
		if (start_node == nullptr) {
			return;
		}
		if (start_node->left_ && start_node->left_->is_not_fake_ != false)
		{
			help_traverse_inorder(start_node->left_, cnt);
		}
		cnt.push_back(start_node->value_);
		if (start_node->right_ && start_node->right_->is_not_fake_ != false)
		{
			help_traverse_inorder(start_node->right_, cnt);
		}

	}

	void update_root_parent() {
		if (root_) {
			root_->parent_ = nullptr;
		}
	}

	void update_begin_end_nodes(AVL_Node* new_node) {
		if (begin_node_ == nullptr) {
			begin_node_ = new_node;
		}
		else if (new_node->value_.first < begin_node_->value_.first) {
			begin_node_ = new_node;
		}
		if (end_node_ == nullptr) {
			end_node_ = new_node;
		}
		else if (new_node->value_.first > end_node_->value_.first) {
			end_node_ = new_node;
		}
		end_node_->right_ = fake_end_node_;
		begin_node_->left_ = fake_begin_node_;
		fake_end_node_->parent_ = end_node_;
		fake_begin_node_->parent_ = begin_node_;
	}

	void update_begin_end_nodes_after_erase() {
		begin_node_ = elem_list_.front();
		end_node_ = elem_list_.back();
		fake_end_node_->parent_ = end_node_;
		fake_begin_node_->parent_ = begin_node_;
		end_node_->right_ = fake_end_node_;
		begin_node_->left_ = fake_begin_node_;
	}

	void list_insert(std::list<AVL_Node*>& list, AVL_Node* node_to_insert) {
		if (list.empty()) list.push_back(node_to_insert);
		else {
			auto it = list.begin();
			while (it != list.end() && k_cmp((*it)->value_.first, node_to_insert->value_.first)) {
				++it;
			}
			list.insert(it, node_to_insert);
		}
	}

	void list_erase(std::list<AVL_Node*>& list, const key_type& key) {
		auto it = list.begin();
		while (it != list.end() && (*it)->value_.first != key) {
			++it;
		}
		if (it != list.end()) {
			list.erase(it);
		}
	}
};