#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>

#include "CircularQueue.h"

template <typename T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue()
		: m_queue(CircularQueue<T>())
	{
		m_mutex = new std::mutex();
		m_notEmpty = new std::condition_variable();
		m_notFull = new std::condition_variable();
	}

	explicit ThreadSafeQueue(size_t size)
		: m_queue(CircularQueue<T>(size))
	{
		m_mutex = new std::mutex();
		m_notEmpty = new std::condition_variable();
		m_notFull = new std::condition_variable();
	}

	ThreadSafeQueue(const ThreadSafeQueue& other)
	{
		m_queue = CircularQueue<T>(other.m_queue);
		m_mutex = other.m_mutex;
		m_notEmpty = other.m_notEmpty;
		m_notFull = other.m_notFull;
	}

	ThreadSafeQueue(ThreadSafeQueue&& other) noexcept
		: m_mutex(other.m_mutex), m_notEmpty(other.m_notEmpty), m_notFull(other.m_notFull)
	{
		m_queue = CircularQueue<T>(std::move(other.m_queue));
	}

	~ThreadSafeQueue()
	{
		delete m_mutex;
		delete m_notEmpty;
		delete m_notFull;
	}

	bool empty() const { return m_queue.empty(); }

	bool full() const { return m_queue.full(); }

	int size() const { return m_queue.size(); }

	void close()
	{
		m_closed = true;
		m_notFull->notify_all();
		m_notEmpty->notify_all();
	}

	bool enqueue(const T& item)
	{
		std::unique_lock lock(*m_mutex);
		while (m_queue.full())
		{
			m_notFull->wait(lock);
			if (m_closed) return false;
		}

		bool result = m_queue.enqueue(item);
		if (result) m_notEmpty->notify_one();
		return result;
	}

	bool enqueue(T&& item)
	{
		std::unique_lock lock(*m_mutex);
		while (m_queue.full())
		{
			m_notFull->wait(lock);
			if (m_closed) return false;
		}

		bool result = m_queue.enqueue(std::move(item));
		if (result) m_notEmpty->notify_one();
		return result;
	}

	std::optional<T> dequeue()
	{
		std::optional<T> item;
		std::unique_lock lock(*m_mutex);
		while (m_queue.empty())
		{
			m_notEmpty->wait(lock);
			if (m_closed) return item;
		}

		item = m_queue.dequeue();
		m_notFull->notify_one();
		return item;
	}

	ThreadSafeQueue& operator= (const ThreadSafeQueue& rhs)
	{
		if (this == &rhs)
			return *this;

		m_queue = rhs.m_queue;
		m_mutex = rhs.m_mutex;
		m_notEmpty = rhs.m_notEmpty;
		m_notFull = rhs.m_notFull;
	}

private:
	CircularQueue<T> m_queue;
	std::mutex* m_mutex;
	std::condition_variable* m_notEmpty;
	std::condition_variable* m_notFull;
	bool m_closed = false;
};