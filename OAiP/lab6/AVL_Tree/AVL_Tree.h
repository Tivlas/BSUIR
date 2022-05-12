#pragma once
#include <utility>
template <typename T>
class AVL_Tree {
	using value_type = T;
private:
	struct AVL_Node {
		int key;
		value_type data;
		AVL_Node* left_child;
		AVL_Node* right_child;
		int height;

		AVL_Node(int key, value_type data) : key(key), data(data), left_child(nullptr), right_child(nullptr), height(1) {}
		~AVL_Node() = default;

		// METHODS
		static void clear(AVL_Node* root) {
			if (root != nullptr) {
				clear(root->left_child);
				clear(root->right_child);
				delete root;
			}
		}

		int max(int a, int b) {
			return a > b ? a : b;
		}

		int get_height(AVL_Node* node) {
			return node ? node->height : 0;
		}

		int get_balance_factor(AVL_Node* node) {
			return get_height(node->right_child) - get_height(node->left_child);
		}

		void update_height(AVL_Node* node) {
			node->height = max(get_height(node->left_child), get_height(node->right_child)) + 1;
		}

		AVL_Node* rotate_left(AVL_Node* node) {
			AVL_Node* r_ch = node->right_child;
			node->right_child = r_ch->left_child;
			r_ch->left_child = node;
			update_height(node);
			update_height(r_ch);
			return r_ch;
		}

		AVL_Node* rotate_right(AVL_Node* node) {
			AVL_Node* l_ch = node->left_child;
			node->left_child = l_ch->right_child;
			l_ch->right_child = node;
			update_height(node);
			update_height(l_ch);
			return l_ch;
		}

		AVL_Node* balance(AVL_Node* node) {
			update_height(node);
			if (get_balance_factor(node) == 2) {
				if (get_balance_factor(node->right_child) < 0) {
					node->right_child = rotate_right(node->right_child);
				}
				return rotate_left(node);
			}
			else if (get_balance_factor(node) == -2) {
				if (get_balance_factor(node->left_child) > 0) {
					node->left_child = rotate_left(node->left_child);
				}
				return rotate_right(node);
			}
			return node;
		}

		AVL_Node* insert(AVL_Node* root, int key, value_type data) {
			if (root == nullptr) {
				return new AVL_Node(key, data);
			}
			else if (key < root->key) {
				root->left_child = insert(root->left_child, key, data);
			}
			else if (key >= root->key) {
				root->right_child = insert(root->right_child, key, data);
			}
			return balance(root);
		}

		AVL_Node* find_min(AVL_Node* node) {
			return node->left_child ? find_min(node->left_child) : node;
		}

		AVL_Node* remove_min(AVL_Node* node) {
			if (node->left_child == nullptr) {
				return node->right_child;
			}
			node->left_child = remove_min(node->left_child);
			return balance(node);
		}

		AVL_Node* remove(AVL_Node* root, int _key) {
			if (!root) {
				return root;
			}
			if (_key < root->key) {
				root->left_child = remove(root->left_child, _key);
			}
			else if (_key > root->key) {
				root->right_child = remove(root->right_child, _key);
			}
			else {
				AVL_Node* left = root->left_child;
				AVL_Node* right = root->right_child;
				delete root;
				if (!right) {
					return left;
				}
				AVL_Node* min = find_min(right);
				min->right_child = remove_min(right);
				min->left_child = left;
				return balance(min);
			}
			return balance(root);
		}

		AVL_Node* search(AVL_Node* root, int search_key) {
			if (root == nullptr) {
				return nullptr;
			}
			else if (search_key < root->key) {
				return search(root->left_child, search_key);
			}
			else if (search_key > root->key) {
				return search(root->right_child, search_key);
			}
			else {
				return root;
			}
		}

		template <typename Container>
		void traverse_postorder(Container& c) {
			if (this->left_child) {
				this->left_child->traverse_postorder(c);
			}
			if (this->right_child) {
				this->right_child->traverse_postorder(c);
			}
			c.push_back(std::pair<int, value_type>(this->key, this->data));
		}

		template <typename Container>
		void traverse_preorder(Container& c) {
			c.push_back(std::pair<int, value_type>(this->key, this->data));
			if (this->left_child) {
				this->left_child->traverse_preorder(c);
			}
			if (this->right_child) {
				this->right_child->traverse_preorder(c);
			}
		}

		template <typename Container>
		void traverse_inorder(Container& c) {
			if (this->left_child) {
				this->left_child->traverse_inorder(c);
			}
			c.push_back(std::pair<int, value_type>(this->key, this->data));
			if (this->right_child) {
				this->right_child->traverse_inorder(c);
			}
		}
	};

private:
	AVL_Node* root;
public:
	AVL_Tree() : root(nullptr) {}

	~AVL_Tree() {
		AVL_Node::clear(root);
	}

	void clear() {
		AVL_Node::clear(root);
		root = nullptr;
	}
	
	void insert(int key, value_type value) {
		root = root->insert(root, key, value);
	}

	void remove(int remove_key) {
		root = root->remove(root, remove_key);
	}

	value_type search(int search_key) {
		AVL_Node* node = root->search(root, search_key);
		return node ? node->data : throw std::exception("Вершина с таким ключем не существует");
	}

	bool empty() {
		return root == nullptr;
	}

	template <typename Container>
	void traverse_postorder(Container& c) {
		if (root)
		{
			root->traverse_postorder(c);
		}
	}

	template <typename Container>
	void traverse_preorder(Container& c) {
		if (root)
		{
			root->traverse_preorder(c);
		}
	}

	template <typename Container>
	void traverse_inorder(Container& c) {
		if (root)
		{
			root->traverse_inorder(c);
		}
	}
};