#include "windows.h"
#include <cassert>
#include <vector>

template <class T>
struct Node {
	T data;
	Node* next;

	Node(): next(nullptr) {}
};

template <class T>
class LinkedList {
public:
	Node<T>* head;

	LinkedList(): head(nullptr) {};

	void insert(Node<T>* prev, Node<T>* added) {
		if (prev == nullptr) {
			added->next = head;
			head = added;
		} else {
			added->next = prev->next;
			prev->next = added;
		}
	}

	void remove(Node<T>* previousNode, Node<T>* deleteNode) {
		if (previousNode == nullptr) {
			head = deleteNode->next;
		} else {
			previousNode->next = deleteNode->next;
		}
	}
};

class FixedSizeAllocator
{
public:
	FixedSizeAllocator() : head(nullptr), freeSpace(0) {}

	void* alloc(const size_t blockSize) {
		if (blockSize > freeSpace)
		{
			freeSpace = PAGE_SIZE;
			head = get_page();
		}

		auto tmp = head;
		head = static_cast<char*>(head) + blockSize;
		freeSpace -= blockSize;
		return tmp;
	}

	bool free(void* ptr) {
		for (auto i = pages.begin(); i != pages.end(); ++i)
		{
			if (*i == ptr) {
				VirtualFree(*i, 0, MEM_RELEASE);
			
				return true;
			}
		}

		return false;
	}

	~FixedSizeAllocator() {
		for (auto i = pages.begin(); i != pages.end(); ++i)
		{
			VirtualFree(*i, 0, MEM_RELEASE);
		}
	}

private:
	void* head;
	size_t freeSpace;

	void* get_page() {
		auto page = VirtualAlloc(nullptr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		pages.push_back(page);

		return page;
	}

	const size_t PAGE_SIZE = 4096;
	std::vector<void*> pages;
};

class BaseAllocator
{
protected:
	size_t freeSpace;
	size_t usedSpace;

	static const size_t get_padding(const size_t address, const size_t alignment) {
		size_t multiplier = (address / alignment) + 1;
		size_t alignedAddress = multiplier * alignment;
		size_t padding = alignedAddress - address;

		return padding;
	}

	static const size_t get_header_padding(
		const size_t baseAddress, 
		const size_t alignment,
		const size_t headerSize
	) {
		size_t padding = get_padding(baseAddress, alignment);
		size_t neededSpace = headerSize;

		if (neededSpace > padding)
		{
			neededSpace -= padding;    
			padding += alignment * ((neededSpace % alignment > 0) + (neededSpace / alignment));
		}

		return padding;
	}
public:
	BaseAllocator(size_t totalMemorySize) {
		freeSpace = totalMemorySize;
		usedSpace = 0;
	}

	virtual ~BaseAllocator() {
		freeSpace = 0;
	}

	virtual void* alloc(size_t size, size_t alignment = 0) = 0;

	virtual void free(void* ptr) = 0;

	virtual void init() = 0;
};

class FreeListAllocator : public BaseAllocator
{
private:
	struct FreeHeader
	{
		size_t blockSize;
	};

	struct AllocationHeader
	{
		size_t blockSize;
		char padding;
	};

	void* start = nullptr;
	LinkedList<FreeHeader> freeList;

public:
	FreeListAllocator(size_t totalMemorySize) : BaseAllocator(totalMemorySize) {}

	virtual ~FreeListAllocator() {
		VirtualFree(start, 0, MEM_RELEASE);
		start = nullptr;
	}

	void* alloc(size_t size, size_t alignment = 0) override {
		size_t allocationHeaderSize = sizeof(AllocationHeader);
		size_t freeHeaderSize = sizeof(FreeHeader);

		size_t padding;
		Node<FreeHeader> *affectedNode,
			*previousNode;

		this->find(size, alignment, padding, previousNode, affectedNode);

		size_t alignmentPadding = padding - allocationHeaderSize;
		size_t requiredSize = size + padding;

		const size_t rest = affectedNode->data.blockSize - requiredSize;

		if (rest > 0)
		{
			Node<FreeHeader>* newFreeNode = (Node<FreeHeader> *)((size_t)affectedNode + requiredSize);
			newFreeNode->data.blockSize = rest;
			freeList.insert(affectedNode, newFreeNode);
		}

		freeList.remove(previousNode, affectedNode);

		const size_t headerAddress = (size_t)affectedNode + alignmentPadding;
		const size_t dataAddress = headerAddress + allocationHeaderSize;

		((AllocationHeader *)headerAddress)->blockSize = requiredSize;
		((AllocationHeader *)headerAddress)->padding = alignmentPadding;

		usedSpace += requiredSize;

		return (void*)dataAddress;
	}

	void free(void* ptr) override {
		size_t currentAddress = (size_t)ptr;
		size_t headerAddress = currentAddress - sizeof(AllocationHeader);

		AllocationHeader* allocationHeader{ (AllocationHeader *)headerAddress };

		Node<FreeHeader>* freeNode = (Node<FreeHeader> *)(headerAddress);
		freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
		freeNode->next = nullptr;

		Node<FreeHeader>* it = freeList.head;
		Node<FreeHeader>* itPrev = nullptr;

		while (it != nullptr)
		{
			if (ptr < it)
			{
				freeList.insert(itPrev, freeNode);
				break;
			}
			itPrev = it;
			it = it->next;
		}

		usedSpace -= freeNode->data.blockSize;

		coalesce(itPrev, freeNode);
	}

	void init() override {
		if (start != nullptr)
		{
			free(start);
			start = nullptr;
		}
		start = VirtualAlloc(nullptr, this->freeSpace, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		this->reset();
	}

	virtual void reset() {
		usedSpace = 0;
		Node<FreeHeader>* firstNode = static_cast<Node<FreeHeader> *>(start);

		firstNode->data.blockSize = freeSpace;
		firstNode->next = nullptr;

		freeList.head = nullptr;
		freeList.insert(nullptr, firstNode);
	}
private:
	FreeListAllocator(FreeListAllocator& freeListAllocator);

	void coalesce(Node<FreeHeader>* previousNode, Node<FreeHeader>* freeNode) {
		if (freeNode->next != nullptr &&
			(size_t)freeNode + freeNode->data.blockSize == (size_t)freeNode->next)
		{
			freeNode->data.blockSize += freeNode->next->data.blockSize;
			freeList.remove(freeNode, freeNode->next);
		}

		if (previousNode != nullptr &&
			(size_t)previousNode + previousNode->data.blockSize == (size_t)freeNode)
		{
			previousNode->data.blockSize += freeNode->data.blockSize;
			freeList.remove(previousNode, freeNode);
		}
	}

	void find(size_t size, size_t alignment, size_t& padding, Node<FreeHeader>*& previousNode, Node<FreeHeader>*& foundNode) {
		auto smallestDiff = 18446744073709551615;

		Node<FreeHeader>* bestBlock = nullptr;
		Node<FreeHeader> *it = freeList.head,
						 *itPrev = nullptr;

		while (it != nullptr)
		{
			padding = get_header_padding((size_t)it, alignment, sizeof(AllocationHeader));
			const size_t requiredSpace = size + padding;
			if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
			{
				bestBlock = it;
			}
			itPrev = it;
			it = it->next;
		}

		previousNode = itPrev;
		foundNode = bestBlock;
	}
};

class MemoryAllocator {
private:
	bool BlockFSACheck(size_t size) {
		return size >= 16 && size <= 512 && (size & (size - 1)) == 0;
	}

	bool initialized, destroyed;

	size_t FLAMemorySize;

	FixedSizeAllocator* FSA;
	FreeListAllocator* FLA;
public:
	MemoryAllocator(size_t FLAMemorySize = 1024 * 1024 * 1024):
		FSA(nullptr), FLA(nullptr), initialized(false), destroyed(false), FLAMemorySize(FLAMemorySize) {}


	virtual void init() {
		FSA = new FixedSizeAllocator();
		FLA = new FreeListAllocator(FLAMemorySize);
		
		initialized = true;
	}

	virtual ~MemoryAllocator() {
		assert(destroyed);
	}

	virtual void* alloc(size_t size) {		
		if (BlockFSACheck(size)) {
			return FSA->alloc(size);
		} else if (size < TEN_MB) {
			return FLA->alloc(size);
		} else {
			return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		}
	}

	virtual void free(void* ptr) {
		if((uintptr_t)ptr >= TEN_MB) {
			VirtualFree(ptr, 0, MEM_RELEASE);
		} else if (!FSA->free(ptr)) {
			FLA->free(ptr);
		}
	}

	virtual void destroy() {
		delete FSA;
		delete FLA;
		
		destroyed = true;
	}
};
