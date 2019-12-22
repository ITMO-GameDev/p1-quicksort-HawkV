#include "pch.h"
#include <iostream>
#include <algorithm>

template <typename K, typename V>
class Node final {
	public:
		K key;
		V value;

		Node(K key, V value): key(key), value(value) {}
		Node(const Node<K, V>& node): key(node.key), value(node.value) { }
};

template <typename K, typename V>
class AVLTree final {
public:
	AVLTree(AVLTree<K, V>* parent = nullptr) : 
		left(nullptr), 
		right(nullptr), 
		parent(parent), 
		data(nullptr),
		height(1) {}

	~AVLTree() {
		delete data;
		delete left;
		delete right;
	}

	int balanceFactor()
	{
		return (right != nullptr? right->height: 0) -
				(left != nullptr?  left->height: 0);
	}

	void updateHeight() {
		height = std::max(
			 left != nullptr ?  left->height : 0,
			right != nullptr ? right->height : 0
		) + 1;
	}

	void balance() {
		auto parent = this->parent;

		this->updateHeight();
		auto balanceFactorValue = this->balanceFactor();

		if (balanceFactorValue == 2) {
			if (this->right->balanceFactor() < 0) {
				this->right->rotateRight();
			}

			this->rotateLeft();
		} else if (balanceFactorValue == -2) {
			if (this->left->balanceFactor() > 0) {
				this->left->rotateLeft();
			}

			this->rotateRight();
		}

		if (parent != nullptr) {
			parent->balance();
		}
	}

	void rotateRight()
	{
		AVLTree* root = this;
		bool hasParent = this->parent != nullptr;
		bool isLeft = hasParent && (this->parent->left == root);

		this->left = root->right;
		root->right = this;

		root->parent = this->parent;
		this->parent = root;

		this->updateHeight();
		root->updateHeight();

		if (hasParent) {
			if (isLeft) {
				root->parent->left = root;
			}
			else {
				root->parent->right = root;
			}

			root->parent->updateHeight();
		}
	}

	void rotateLeft()
	{
		AVLTree* root = this->right;
		bool hasParent = this->parent != nullptr;
		bool isLeft = hasParent && (this->parent->left == root);
		
		this->right = root->left;
		root->left = this;

		root->parent = this->parent;
		this->parent = root;

		this->updateHeight();
		root->updateHeight();

		if (hasParent) {
			if (isLeft) {
				root->parent->left = root;
			}
			else {
				root->parent->right = root;
			}

			root->parent->updateHeight();
		}
	}

	AVLTree* insert(const Node<K, V>& value) {
		return this->_root()->_insert(value);
	}

	AVLTree* find(const K& key) {
		return this->_root()->_find(key);
	}

	AVLTree* del(const K& key) {
		auto node = this->_root()->find(key);

		if (node == nullptr) {
			return nullptr;
		}

		auto parent = node->parent;
		bool isLeft = parent->left == node;

		if (node->height == 1) {
			if (isLeft) {
				parent->left = nullptr;
			}
			else {
				parent->right = nullptr;
			}

			delete node;
		}
		else if (node->left != nullptr && node->right != nullptr) {
			auto max = node->left->findMax();

			node->data = max->data;

			bool isLeftt = max->parent->left == max;
			
			if (isLeftt) {
				max->parent->left = nullptr;
			}
			else {
				max->parent->right = nullptr;
			}
		} else if (node->left == nullptr) {
			if (isLeft) {
				parent->left = node->right;
			}
			else {
				parent->right = node->right;
			}

			delete node;
		} else if (node->right == nullptr) {
			if (isLeft) {
				parent->left = node->left;
			}
			else {
				parent->right = node->left;
			}

			delete node;
		}

		parent->balance();

		return parent;
	}

	AVLTree* findMax() {
		if (data == nullptr) {
			return nullptr;
		} else if (right != nullptr) {
			return right->findMax();
		} else {
			return this;
		}
	}

	Node<K, V>* data;
	int height;

private:
	AVLTree* _root() {
		auto root = this;

		while (root->parent != nullptr) {
			root = root->parent;
		}

		return root;
	}

	AVLTree* _insert(const Node<K, V>& value) {
		if (data == nullptr) {
			data = new Node<K, V>(value);
		} 
		else if (data->key == value.key) {
			data->value = value.value;

			return nullptr;
		} else if (data->key > value.key) {
			if (left == nullptr) {
				left = new AVLTree(this);
			}

			return left->_insert(value);
		}
		else {
			if (right == nullptr) {
				right = new AVLTree(this);
			}

			return right->_insert(value);
		}

		this->balance();

		return this;
	}

	AVLTree<K, V>* _find(const K& key) {
		if (data == nullptr) {
			return nullptr;
		}
		else if (data->key == key) {
			return this;
		}
		else if (data->key > key) {
			return left != nullptr ? left->_find(key) : nullptr;
		}
		else {
			return right != nullptr ? right->_find(key) : nullptr;
		}
	}

	AVLTree<K, V> *left, *right, *parent;
};

template<typename K, typename V>
class Dictionary final {
	public:
		Dictionary(): dictionarySize(0) {
			tree = new AVLTree<K, V>();
		}

		~Dictionary() {
			delete tree;
		}

		void put(const K& key, const V& value) {
			auto result = tree->insert(Node<K, V>(key, value));

			if (result != nullptr) {
				dictionarySize++;
			}
		}

		void remove(const K& key) {
			auto result = tree->del(key);

			if (result != nullptr) {
				dictionarySize--;
			}
		}

		bool contains(const K& key) {
			return tree->find(key) != nullptr;
		}

		const V& operator[] (const K& key) const {
			auto node = tree->find(key);

			return node ? node->data->value : *(new V());
		}

		V& operator[] (const K& key) {
			auto node = tree->find(key);

			if (node == nullptr) {
				node = tree->insert(Node<K, V>(key, *(new V())));
			}

			return node->data->value;
		}

		int size() {
			return dictionarySize;
		}

		class Iterator {
			public:
				const K& key() const {
					return enclosingDictionary.tree->data->key;
				}

				const V& get() const {
					return enclosingDictionary.tree->data->value;
				}

				void set(const V& value) {
					enclosingDictionary.tree->data->value = value;
				}

				void next() {
				}

				void prev() {
				}

				bool hasNext() const {
				}

				bool hasPrev() const {
				}

				Iterator(Dictionary<K, V>& enclosing) : enclosingDictionary(enclosing), nodeIndex(0) {
					traverse(enclosing.tree);
				}

			private:
				Dictionary<K, V>& enclosingDictionary;
		};

		Iterator iterator() {
			return Iterator(*this);
		}

		const Iterator iterator() const {
			return Iterator(*this);
		}

	private:
		Dictionary(const Dictionary&);

		Dictionary& operator=(const Dictionary&);

		AVLTree<K, V>* tree;

		size_t dictionarySize;
};
