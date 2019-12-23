template<typename T>
class Array final
{
	public: 
		Array(int capacity = 16): capacity(capacity), arraySize(0) {
			container = new T[capacity];
		}

		~Array() {
			delete[] container;
		}

		void insert(const T& value) {
			insert(arraySize, value);
		}

		void insert(int index, const T& value) {
			if (arraySize == capacity) {
				expand();
			}
			
			if (index != arraySize) {
				moveRight(index);
			}

			container[index] = value;
			arraySize++;
		}

		void remove(int index) {
			moveLeft(index);
			arraySize--;

			if (arraySize == capacity / resizingConstant) {
				shrink();
			}
		}

		const T& operator[](int index) const {
			return container[index];
		}

		T& operator[](int index) {
			return container[index];
		}

		int size() const {
			return arraySize;
		}

		class Iterator {
			public:
				const T& get() const {
					return enclosingArray[currentIndex];
				}

				void set(const T& value) {
					enclosingArray[currentIndex] = value;
				}

				void insert(const T& value) {
					 enclosingArray.insert(currentIndex, value);
				}

				void remove() {
					enclosingArray.remove(currentIndex);
				}

				void next() {
					currentIndex++;
				}

				void prev() {
					currentIndex--;
				}

				void toIndex(int index) {
					currentIndex = index;
				}

				bool hasNext() const {
					return currentIndex <= enclosingArray.arraySize - 1;
				}

				bool hasPrev() const {
					return currentIndex > 0;
				}

				Iterator(Array<T>& enclosing) : enclosingArray(enclosing), currentIndex(0) {}

			private: 
				Array<T>& enclosingArray;
				size_t currentIndex;
		};

		Iterator iterator() {
			return Iterator(*this);
		}

		const Iterator iterator() const {
			return Iterator(*this);
		}

	private:
		Array(const Array&);

		Array& operator=(const Array&);

		void moveLeft(int index) {
			for (size_t i = index; i < arraySize; i++)
			{
				container[i] = std::move(container[i + 1]);
			}
		}

		void moveRight(int index) {
			for (size_t i = arraySize; i > index; i--)
			{
				container[i] = std::move(container[i - 1]);
			}
		}

		void resize(size_t newCapacity) {
			T* newContainer = new T[newCapacity];

			for (size_t i = 0; i < arraySize; i++)
			{
				newContainer[i] = std::move(container[i]);
			}

			delete[] container;

			container = newContainer;
		}

		void expand() {
			resize(capacity * resizingConstant);
		}

		void shrink() {
			resize(capacity / resizingConstant);
		}

		T* container;

		size_t capacity;
		size_t arraySize;

		const size_t resizingConstant = 2;
};
