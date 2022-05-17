#pragma once
#include <functional>
#include <list>
#include <type_traits>
template <typename Key, typename T, typename Compare = std::less<Key>>
class AVL_Tree {
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using key_compare = Compare;
	using difference_type = std::ptrdiff_t;

public:
	struct AVL_Node;

	template <typename T>
	class Iterator {
		using val_t = T;
		using reference = val_t&;
		using pointer = val_t*;
	private:
		AVL_Node* cur = nullptr;
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
		Iterator& operator=(Iterator&& other) {
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
	};

	template <typename T>
	class List_Iterator {
	};

	using my_iterator = Iterator<value_type>;
	using const_iterator = Iterator<const value_type>;
	using my_reverse_iterator = std::reverse_iterator<my_iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using list_iterator = List_Iterator<value_type>;
	using const_list_iterator = List_Iterator<const value_type>;
	using list_reverse_iterator = std::reverse_iterator<list_iterator>;
	using const_list_reverse_iterator = std::reverse_iterator<const_list_iterator>;

	using iterator = std::conditional_t<std::is_same_v<key_type, mapped_type>, const_iterator, my_iterator>;
	using reverse_iterator = std::conditional_t<std::is_same_v<key_type, mapped_type>, const_reverse_iterator, my_reverse_iterator>;
private:

	// AVL_NODE
	struct AVL_Node
	{
		AVL_Node* left_ = nullptr;
		AVL_Node* right_ = nullptr;
		AVL_Node* parent_ = nullptr;
		int height_ = 1;
		value_type value_;
		AVL_Node(const value_type& value) : value_(value) {}
		AVL_Node(value_type&& value) : value_(std::move(value)) {}
	};

	key_compare  k_cmp;
	AVL_Node* fake_root_ = new AVL_Node(value_type(key_type(), mapped_type()));
	AVL_Node* root_ = nullptr;
	AVL_Node* begin_node_ = nullptr;
	size_type size_ = 0;
	std::list<value_type> elem_list;

public:

	AVL_Tree() : k_cmp(key_compare()) {
		fake_root_->left_ = root_;
	}

	AVL_Tree(const key_compare& k_cmp) : k_cmp(k_cmp) {
		fake_root_->left_ = root_;
	}

	// INSERT
	std::pair<iterator, bool> insert(const value_type& value) {
		AVL_Node* new_node = new AVL_Node(value);
		bool inserted = false;
		iterator it;
		root_ = help_insert(root_, new_node, inserted, it);
		if (inserted)
		{
			++size_;
			update_root_parent();
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
			++size_;
			update_root_parent();
		}
		return std::make_pair(it, inserted);
	}

	// ERASE
	size_type erase(const key_type& key) {
		if (root_ == nullptr) {
			return 0;
		}
		size_type count = 0;
		root_ = remove(root_, key, count);
		if (count != 0)
		{
			update_root_parent();
			--size_;
		}
		return count;
	}

	size_type size() const {
		return size_;
	}

	bool empty() const {
		return size_ == 0;
	}

	void clear() {
		delete fake_root_;
		fake_root_ = new AVL_Node(value_type(key_type(), mapped_type()));
		root_ = nullptr;
		begin_node_ = nullptr;
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
			return insert(value_type(key, mapped_type())).first->second;
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
		return iterator(fake_root_);
	}

	const_iterator cend() const {
		return const_iterator(fake_root_);
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
		return node->left_ ? find_min(node->left_) : node;
	}

	AVL_Node* remove_min(AVL_Node* node) {
		if (node->left_ == nullptr) {
			if (node->right_) {
				node->right_->parent_ = node->parent_;
			}
			return node->right_;
		}
		node->left_ = remove_min(node->left_);
		return balance(node);
	}

	AVL_Node* help_insert(AVL_Node* root, AVL_Node* node_to_insert, bool& inserted, iterator& it) {
		if (root == nullptr) {
			root = node_to_insert;
			inserted = true;
			it = iterator(root);
			return root;
		}
		else if (k_cmp(node_to_insert->value_.first, root->value_.first)) {
			root->left_ = help_insert(root->left_, node_to_insert, inserted, it);
		}
		else if (k_cmp(root->value_.first, node_to_insert->value_.first)) {
			root->right_ = help_insert(root->right_, node_to_insert, inserted, it);

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
		help_traverse_inorder(start_node->left_, cnt);
		cnt.push_back(start_node->value_);
		help_traverse_inorder(start_node->right_, cnt);
	}

	void update_root_parent() {
		if (root_) {
			root_->parent_ = fake_root_;
		}
	}

	AVL_Node* remove(AVL_Node* root, const key_type& key, size_type& count) {
		if (root == nullptr) {
			return root;
		}
		if (k_cmp(key, root->value_.first)) {
			root->left_ = remove(root->left_, key, count);
		}
		else if (k_cmp(root->value_.first, key)) {
			root->right_ = remove(root->right_, key, count);
		}
		else {
			AVL_Node* left = root->left_;
			AVL_Node* right = root->right_;
			AVL_Node* temp_parent = root->parent_;
			delete root;
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
			right->parent_ = min;
			min->left_ = left;
			left->parent_ = min;
			min->parent_ = temp_parent;
			count = 1;
			return balance(min);
		}
		return balance(root);
	}
};