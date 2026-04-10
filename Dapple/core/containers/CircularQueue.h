#pragma once

#include "Buffer.h"

#define DEFAULT_SIZE 10;

template <typename T>
class CircularQueue
{
public:
	CircularQueue() 
	{
		m_buffer = new Buffer<T>(m_size);
	}

	CircularQueue(size_t size) : m_size(size)
	{
		m_buffer = new Buffer<T>(m_size);
	}

	CircularQueue(const CircularQueue& other)
	{
		m_buffer = &Buffer<T>(other.m_buffer);
		m_size = other.m_size;
		m_front = other.m_front;
		m_back = other.m_back;
	}

	CircularQueue(CircularQueue&& other) noexcept
		: m_size(other.m_size), m_front(other.m_front), m_back(other.m_back), m_buffer(other.m_buffer)
	{
		// Leave 'other' in a valid, destructible, but empty state
		other.m_buffer = nullptr;
		other.m_size = 0;
		other.m_front = 0;
		other.m_back = 0;
	}

	~CircularQueue()
	{
		delete m_buffer;
	}

	bool isEmpty() const { return m_front == -1; }

	bool isFull() const { return (m_back + 1) % m_size == m_front; }

	bool enqueue(const T& object)
	{
		if (isFull()) return false;

		m_back = (m_back + 1) % m_size;
		m_buffer->at(m_back) = object;
		if (m_front == -1) m_front = 0;
		return true;
	}

	bool enqueue(T&& object)
	{
		if (isFull()) return false;

		m_back = (m_back + 1) % m_size;
		m_buffer->at(m_back) = T(std::move(object));
		if (m_front == -1) m_front = 0;
		return true;
	}

	T& dequeue()
	{
		if (isEmpty()) throw std::out_of_range("CircularQueue::dequeue: queue is empty");

		T& temp = m_buffer->at(m_front);
		if (m_front == m_back)
			m_front = m_back = -1;
		else
			m_front = (m_front + 1) % m_size;
		return temp;
	}

	const T& peek()
	{
		if (isEmpty()) throw std::out_of_range("CircularQueue::peek: queue is empty");

		return m_buffer->at(m_front);
	}

	int size()
	{
		if (isEmpty()) return 0;
		if (m_back >= m_front) return m_back - m_front + 1;
		else return m_size - (m_front - m_back) + 1;
	}

private:
	int m_front = -1; 
	int m_back = -1;
	size_t m_size = DEFAULT_SIZE;
	Buffer<T>* m_buffer = nullptr;
};