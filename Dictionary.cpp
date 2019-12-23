#include <algorithm>

template <typename K, typename V>
class AVLTree final {
public:
	AVLTree(AVLTree<K, V>* parent = nullptr) : 
		left(nullptr), 
		right(nullptr), 
		parent(parent), 
		key(nullptr),
		value(nullptr),
		height(1) {}

	~AVLTree() {
		delete key;
		delete value;
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
			right != nullptr ? right->height : 0,
			 left != nullptr ?  left->height : 0
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
		AVLTree* root = this->left;

		bool hasParent = this->parent != nullptr;
		bool isLeft = hasParent && (this->parent->left == this);

		this->left = root->right;
		root->right = this;

		root->parent = this->parent;
		this->parent = root;

		if (this->left != nullptr) {
			this->left->parent = this;
		}

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
		bool isLeft = hasParent && (this->parent->left == this);
		
		this->right = root->left;
		root->left = this;

		root->parent = this->parent;
		this->parent = root;

		if (this->right != nullptr) {
			this->right->parent = this;
		}

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

	AVLTree* insert(const K& key, const V& value, bool *sizeChanged) {
		return this->root()->_insert(key, value, sizeChanged);
	}

	AVLTree* find(const K& key) {
		return this->root()->_find(key);
	}

	AVLTree* remove(const K& key, bool *sizeChanged) {
		auto node = this->root()->find(key);

		if (node == nullptr) {
			sizeChanged = false;
			return this;
		}

		auto parent = node->parent;
		bool isLeft = parent->left == node;

		if (node->left != nullptr && node->right != nullptr) {
			auto max = node->left->findMax();

			node->key = max->key;
			node->value = max->value;

			if (max->parent->left == max) {
				max->parent->left = nullptr;
			}
			else {
				max->parent->right = nullptr;
			}

			*sizeChanged = true;

			parent->balance();

			return parent;
		} 
		
		AVLTree<K, V>* childReplacement = nullptr;

		if (node->height == 1) {
			childReplacement = nullptr;

			if (node->parent == nullptr) {
				delete node->key;
				delete node->value;

				node->key = nullptr;
				node->value = nullptr;

				return node;
			}
		} else if (node->left == nullptr) {
			childReplacement = node->right;
		} else if (node->right == nullptr) {
			childReplacement = node->left;
		}

		if (isLeft) {
			parent->left = childReplacement;
		}
		else {
			parent->right = childReplacement;
		}

		delete node;

		parent->balance();

		return parent;
	}

	AVLTree* findMin() {
		if (key == nullptr) {
			return nullptr;
		}
		else if (left != nullptr) {
			return left->findMin();
		}
		else {
			return this;
		}
	}

	AVLTree* findMax() {
		if (key == nullptr) {
			return nullptr;
		}
		else if (right != nullptr) {
			return right->findMax();
		}
		else {
			return this;
		}
	}

	AVLTree* next() {
		if (right == nullptr) {
			auto next = parent;

			while ((*(next->key) < *key) && next->parent != nullptr) {			
				next = next->parent;
			}

			if (next->parent == nullptr && (*(next->key) < *key)) {
				return this;
			}

			return next;
		} else {
			return right->findMin();	
		}
	}

	AVLTree* prev() {
		if (left == nullptr) {
			auto next = parent;

			while ((*(next->key) > *key) && next->parent != nullptr) {
				next = next->parent;
			}

			if (next->parent == nullptr && (*(next->key) > *key)) {
				return this;
			}

			return next;
		}
		else {
			return left->findMax();
		}
	}

	AVLTree* root() {
		auto root = this;

		while (root->parent != nullptr) {
			root = root->parent;
		}

		return root;
	}

	K* key;
	V* value;
	int height;

private:
	AVLTree* _insert(const K& key, const V& value, bool *sizeChanged) {
		*sizeChanged = true;

		if (this->key == nullptr) {
			this->key = new K(key);
			this->value = new V(value);
		} 
		else if (*(this->key) == key) {
			delete this->value;
			this->value = new V(value);
			*sizeChanged = false;

			return this;
		} else if (*(this->key) > key) {
			if (left == nullptr) {
				left = new AVLTree(this);
			}

			return left->_insert(key, value, sizeChanged);
		}
		else {
			if (right == nullptr) {
				right = new AVLTree(this);
			}

			return right->_insert(key, value, sizeChanged);
		}

		this->balance();

		return this;
	}

	AVLTree<K, V>* _find(const K& key) {
		if (this->key == nullptr) {
			return nullptr;
		}
		else if (*(this->key) == key) {
			return this;
		}
		else if (*(this->key) > key) {
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
			sizeChanged = new bool();
		}

		~Dictionary() {
			delete sizeChanged;
			delete tree;
		}

		void put(const K& key, const V& value) {
			tree = tree->insert(key, value, sizeChanged);

			if (*sizeChanged) {
				dictionarySize++;
			}
		}

		void remove(const K& key) {
			tree = tree->remove(key, sizeChanged);

			if (*sizeChanged) {
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
				node = tree->insert(key, *(new V()), sizeChanged);
				dictionarySize++;
			}

			return *(node->value);
		}

		int size() {
			return dictionarySize;
		}

		class Iterator {
			public:
				const K& key() const {
					return current->key;
				}

				const V& get() const {
					return *(current->value);
				}

				void set(const V& value) {
					*(current->value) = value;
				}

				void next() {
					current = current->next();
					index++;
				}

				void prev() {
					current = current->prev();
					index--;
				}

				bool hasNext() const {
					return index < enclosingDictionary.size();
				}

				bool hasPrev() const {
					return index > 0;	
				}

				Iterator(Dictionary<K, V>& enclosing) : enclosingDictionary(enclosing), index(0) {
					auto root = enclosing.tree->root();

					firstNode = root->findMin();
					lastNode = root->findMax();

					current = firstNode;
				}

			private:
				Dictionary<K, V>& enclosingDictionary;
				AVLTree<K, V> *firstNode, *lastNode, *current;
				size_t index;
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

		bool *sizeChanged;
};
