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
		if (this == nullptr || this->left == nullptr && this->right == nullptr)
			return 0;
		else if (this->left == nullptr)
			return this->right->height;
		else if (this->right == nullptr)
			return -this->left->height;
		else
			return this->right->height - this->left->height;
	}

	void updateHeight() {
		height = std::max(
			left != nullptr? left->height: 0,
			right != nullptr? right->height: 0
		) + 1;
	}

	void balance() {
		auto balanceFactorValue = balanceFactor();

		if (balanceFactorValue > 1)
		{
			if (left->balanceFactor() < 0)
			{
				left->rotateRight();
			}

			rotateLeft();
		}
		else if (balanceFactorValue < -1)
		{
			if (right->balanceFactor() > 0) 
			{
				right->rotateLeft();
			}

			rotateRight();
		}

	}

	void rotateRight()
	{
		AVLTree* root = this->left;

		this->left = root->right;
		root->right = this;

		root->parent = this->parent;
		this->parent = root;

		root->updateHeight();
		this->updateHeight();	
	}

	void rotateLeft()
	{
		AVLTree* root = this->right;
		
		this->right = root->left;
		root->left = this;

		root->parent = this->parent;
		this->parent = root;

		root->updateHeight();
		this->updateHeight();
	}

	void insert(const Node<K, V>& value) {
		if (data == nullptr) {
			data = new Node<K, V>(value);
		} else if (data->key > value.key) {
			if (this->left == nullptr) {
				this->left = new AVLTree(this);
			}

			this->left->insert(value);
		} else {
			if (this->right == nullptr) {
				this->right = new AVLTree(this);
			}

			this->right->insert(value);
		}

		updateHeight();
		balance();
	}

	AVLTree<K, V>* find(const K& value) {
		if (data == nullptr) {
			return nullptr;
		} else if (data->key == value) {
			return this;
		} else if (data->key < value) {
			if (this->left == nullptr) {
				return nullptr;
			}

			return this->left->find(value);
		} else {
			if (this->right == nullptr) {
				return nullptr;
			}

			return this->right->find(value);
		}
	}
	
	
	void inorder(AVLTree* root) {
		if (root != nullptr) {
			inorder(root->left);
			std::cout << root->data->key << " ";
			inorder(root->right);
		}
	}


	void remove(const K& value) {
		/*search for v

			if v is a leaf

				delete leaf v

			else if v has 1 child

				bypass v

			else replace v with successor*/
	}

	AVLTree<K, V>* findMin() {
		/*if this is null return empty

			if left != null

				go left
		//		else return this key
			*/
	}

	//AVLTree<K, V>* findMax() {
	//	/*if this is null return empty

	//		if right != null

	//			go right

	//		else return this key*/
	//}

	//bool hasNext() {
	//	return left != nullptr || right != nullptr
	//}

	//bool hasPrev() {
	//	return parent != nullptr
	//}

	//AVLTree<K, V>* prev() {
	//	/*if this.left != null return findMax(this.left)

	//	else

	//		p = this.parent, T = this

	//		while (p != null && T == p.left)

	//			T = p, p = T.parent

	//			if p is null return -1

	//			else return p 
	//			*/
	//}

	//AVLTree<K, V>* next() {
	//	/*if this.right != null return findMin(this.right)

	//	else

	//		p = this.parent, T = this

	//		while (p != null && T == p.right)

	//			T = p, p = T.parent

	//			if p is null return -1

	//			else return p 
	//			*/
	//}

	const Node<K, V>* data;
	int height;

private:
	AVLTree<K, V> *left, *right, *parent;
};

template<typename K, typename V>
class Dictionary final {
	public:
		Dictionary() {
	
		}

		~Dictionary() {
	
		}

		void put(const K& key, const V& value) {
		
		}

		void remove(const K& key) {
	
		}

		bool contains(const K& key) {
	
		}

		const V& operator[] (const K& key) const {
	
		}

		V& operator[] (const K& key) {
	
		}

		int size() {
	
		}

		class Iterator {
			public:
				const K& key() const {
				}

				const V& get() const {
				}

				void set(const V& value) {
				}

				void next() {
				}

				void prev() {
				}

				bool hasNext() const {
				}

				bool hasPrev() const {
				}

				Iterator(Dictionary<K, V>& enclosing) : enclosingDictionary(enclosing) {}

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

};
