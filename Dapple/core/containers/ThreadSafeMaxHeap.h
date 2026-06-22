#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>

#include "MaxHeap.h"

template <typename T>
class ThreadSafeMaxHeap
{
public:
	ThreadSafeMaxHeap(size_t capacity = 10) : m_heap(MaxHeap<T>(capacity))
	{
		m_mutex = new std::mutex();
		m_notEmpty = new std::condition_variable();
		m_notFull = new std::condition_variable();
	}

	ThreadSafeMaxHeap(const ThreadSafeMaxHeap& other)
	{
		m_heap = MaxHeap<T>(other.m_heap);
		m_mutex = other.m_mutex;
		m_notEmpty = other.m_notEmpty;
		m_notFull = other.m_notFull;
	}

	ThreadSafeMaxHeap(ThreadSafeMaxHeap&& other) noexcept
		: m_mutex(other.m_mutex), m_notEmpty(other.m_notEmpty), m_notFull(other.m_notFull)
	{
		m_heap = MaxHeap<T>(std::move(other.m_heap));
	}

	~ThreadSafeMaxHeap()
	{
		delete m_mutex;
		delete m_notEmpty;
		delete m_notFull;
	}

	void close()
	{
		m_closed = true;
		m_notFull->notify_all();
		m_notEmpty->notify_all();
	}

	bool push(const T& item)
	{
		std::unique_lock lock(*m_mutex);
		while (m_heap.full())
		{
			m_notFull->wait(lock);
			if (m_closed) return false;
		}

		bool result = m_heap.push(item);
		if (result) m_notEmpty->notify_one();
		return result;
	}

	bool push(T&& item)
	{
		std::unique_lock lock(*m_mutex);
		while (m_heap.full())
		{
			m_notFull->wait(lock);
			if (m_closed) return false;
		}

		bool result = m_heap.push(std::move(item));
		if (result) m_notEmpty->notify_one();
		return result;
	}

	std::optional<T> pop()
	{
		std::optional<T> item;
		std::unique_lock lock(*m_mutex);
		while (m_heap.empty())
		{
			m_notEmpty->wait(lock);
			if (m_closed) return item;
		}

		item = m_heap.pop();
		m_notFull->notify_one();
		return item;
	}

private:
	MaxHeap<T> m_heap;
	std::mutex* m_mutex;
	std::condition_variable* m_notEmpty;
	std::condition_variable* m_notFull;
	bool m_closed = false;
};