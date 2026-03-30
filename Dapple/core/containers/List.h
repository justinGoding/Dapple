#pragma once

#include <iostream>
#include <cstddef>
#include <iterator>

#include "..\memory\PoolAllocator.h"

// IMPORTANT: sizeof(T) must be >= sizeof(void*)

template <typename T>
class Node
{
public:
	Node(T data)
	{
		this->data = data;
		next = nullptr;
	}

private:
	T data;
	Node<T>* next;
	template <typename U>
	friend class List;
};

template <typename T>
class List
{
public:
	struct Iterator
	{
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::forward_iterator_tag;

		Node<T>* m_ptr;

		Iterator() = default;

		Iterator(Node<T>* start)
			: m_ptr(start) {
		}

		reference operator*() const { return m_ptr->data; }
		pointer operator->() const { return m_ptr; }

		// Pre-increment
		Iterator& operator++()
		{
			m_ptr = m_ptr->next;
			return *this;
		}

		// Post-increment
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }
		bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }
	};

	List()
	{
		m_allocator = PoolAllocator(8);
		m_head = nullptr;
	}

	List(T data, size_t chunks_per_block = 8)
	{
		m_allocator = PoolAllocator(chunks_per_block);
		m_head = reinterpret_cast<Node<T>*>(m_allocator.alloc(sizeof(Node<T>)));
		*m_head = Node<T>(data);
	}

	int size() { return m_size; }
	bool empty() { return m_size == 0; }

	T removeFront()
	{
		Node<T>* node = m_head;
		m_head = m_head->next;
		return node->data;
	}

	void insert(T data)
	{
		Node<T>* node = reinterpret_cast<Node<T>*>(m_allocator.alloc(sizeof(Node<T>)));
		*node = Node<T>(data);
		if (m_head == nullptr)
		{
			m_head = node;
		}
		else
		{
			node->next = m_head;
			m_head = node;
		}
	}

	void print()
	{
		int count = 0;
		Node<T>* curr = m_head;
		while (curr != nullptr)
		{
			std::cout << count << ": " << curr->data << std::endl;
			count++;
			curr = curr->next;
		}
	}

	List::Iterator begin() { return Iterator(m_head); }
	List::Iterator end() { return nullptr; }
	const List::Iterator begin() const { return Iterator(m_head); }
	const List::Iterator end() const { return nullptr; }

private:
	Node<T>* m_head;
	int m_size = 0;

	PoolAllocator m_allocator;

public:
};

static_assert(std::forward_iterator<List<int>::Iterator>);