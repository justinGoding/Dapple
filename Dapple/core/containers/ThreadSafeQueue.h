#pragma once

#include <condition_variable>
#include <mutex>

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

	void close()
	{
		m_closed = true;
		m_notFull->notify_all();
		m_notEmpty->notify_all();
	}

	bool enqueue(const T& object)
	{
		std::unique_lock lock(*m_mutex);
		while (m_queue.isFull())
		{
			m_notFull->wait(lock);
			if (m_closed) exit(1);
		}

		bool result = m_queue.enqueue(object);
		m_notEmpty->notify_one();
		return result;
	}

	/*bool enqueue(T&& object)
	{
		std::unique_lock lock(*m_mutex);
		while (m_queue.isFull())
		{
			m_notFull->wait(lock);
		}

		bool result = m_queue.enqueue(std::move(object));
		m_notEmpty->notify_one();
		return result;
	}*/

	T dequeue()
	{
		std::unique_lock lock(*m_mutex);
		while (m_queue.isEmpty())
		{
			m_notEmpty->wait(lock);
			if (m_closed) return T();
		}

		T item = m_queue.dequeue();
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