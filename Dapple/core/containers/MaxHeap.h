#pragma once

#include "..\Core.h"
#include "Buffer.h"


template <typename T>
class MaxHeap
{
public:
	MaxHeap(int capacity = 10) : m_buffer(Buffer<T>(capacity + 1))
	{
	}

	int size() { return m_size; }
	bool empty() { return m_size == 0; }

	bool push(T item)
	{
		if (m_size + 1 >= m_buffer.size()) return false;

		m_buffer.insert(m_size + 1, item);
		m_size++;
		_perc_up(m_size);
		return true;
	}

	T pop()
	{
		if (m_size == 0)
		{
			throw std::out_of_range("Heap::pop: heap is empty");
		}

		T root = m_buffer[1];
		m_buffer[1] = m_buffer[m_size];
		m_size--;
		_sink_down(1);
		return root;
	}

private:
	void _swap(int a, int b)
	{
		T tmp = m_buffer[a];
		m_buffer[a] = m_buffer[b];
		m_buffer[b] = tmp;
	}

	void _perc_up(int i)
	{
		while ((i / 2) > 0)
		{
			if (m_buffer[i] > m_buffer[i / 2])
			{
				_swap(i / 2, i);
			}
			i = i / 2;
		}
	}

	void _sink_down(int i)
	{
		while ((i * 2) <= m_size)
		{
			int max = _max_child(i);
			if (m_buffer[i] > m_buffer[max])
			{
				_swap(i, max);
			}
			i = max;
		}
	}

	int _max_child(int i)
	{
		if (((i * 2) + 1) > m_size)
		{
			return i * 2;
		}
		else
		{
			if (m_buffer[i*2] > m_buffer[(i*2)+1])
			{
				return i * 2;
			}
			else
			{
				return (i * 2) + 1;
			}
		}
	}

private:
	Buffer<T> m_buffer;
	int m_size = 0;
};