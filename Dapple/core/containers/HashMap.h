#pragma once

#include "Vector.h"

#include <algorithm>

template <typename K, typename V>
class HashMap
{
private:
	struct Entry
	{
		Entry() {}
		Entry(K key, V value, int psl = 0) : kv(std::pair<K, V>(key, value)), psl(psl) {}
		Entry(const Entry& other) : kv(std::pair<K, V>(other.kv)), psl(other.psl) {}

		Entry& operator=(const Entry& rhs)
		{
			if (this == &rhs)
				return *this;

			this->kv = rhs.kv;
			this->psl = rhs.psl;
			return *this;
		}

		K& key() { return kv.first; }
		V& value() { return kv.second; }
		const K& key() const { return kv.first; }
		const V& value() const { return kv.second; }

		std::pair<K, V> kv;
		int psl = -1;
	};

public:
	HashMap(int capacity = 8) : m_capacity(capacity), m_size(0)
	{
		m_table = Vector<Entry>(m_capacity, Entry());
	}

	bool insert(const K& key, const V& value)
	{
		// TODO: Log warning for resize
		//if (m_size >= m_capacity / 2) _resize();
		if (m_size >= m_capacity) return false;

		int index = _hash(key);
		Entry new_entry = Entry(key, value);
		while (m_table[index].psl != -1 && m_table[index].key() != new_entry.key())
		{
			// Robin hood probe
			if (new_entry.psl > m_table[index].psl)
			{
				m_maxPSL = (std::max)(new_entry.psl, m_maxPSL);
				_swap_entries(new_entry, m_table[index]);
			}

			index = (index + 1) % m_table.size();
			new_entry.psl++;
		}

		if (m_table[index].psl == -1)
		{
			m_table[index].kv = new_entry.kv;
			m_table[index].psl = new_entry.psl;
			m_maxPSL = (std::max)(new_entry.psl, m_maxPSL);
			m_size++;
		}
		else
		{
			m_table[index].kv.second = new_entry.value(); // Update existing value
		}
		return true;
	}

	V& at(const K& key)
	{
		int index = _hash(key);
		int psl = 0;
		while (m_table[index].psl != -1 && psl <= m_maxPSL)
		{
			if (m_table[index].key() == key)
			{
				return m_table[index].value();
			}
			index = (index + 1) % m_table.size();
			psl++;
		}
		throw std::out_of_range("HashMap::find: key not in table");
	}

	const V& at(const K& key) const
	{
		int index = _hash(key);
		int psl = 0;
		while (m_table[index].psl != -1 && psl <= m_
			
			
			
			
			PSL)
		{
			if (m_table[index].key() == key)
			{
				return m_table[index].value();
			}
			index = (index + 1) % m_table.size();
			psl++;
		}
		throw std::out_of_range("HashMap::find: key not in table");
	}

	bool contains(const K& key)
	{
		int index = _hash(key);
		int psl = 0;
		while (m_table[index].psl != -1 && psl <= m_maxPSL)
		{
			if (m_table[index].key() == key)
			{
				return true;
			}
			index = (index + 1) % m_table.size();
			psl++;
		}
		return false;
	}

	void remove(const K& key)
	{
		int index = _hash(key);
		int psl = 0;
		while (m_table[index].psl != -1 && psl <= m_maxPSL)
		{
			if (m_table[index].key() == key)
			{
				m_table[index].psl = -1;
				m_size--;
				return;
			}
			index = (index + 1) % m_table.size();
			psl++;
		}
		return;
	}

	int size() { return m_size; }
	bool empty() { return m_size == 0; }


	// TODO: ensure these are using move returns
	Vector<K> keys()
	{
		Vector<K> keys = Vector<K>();
		keys.reserve(m_size);
		for (const Entry& e : m_table)
		{
			if (e.psl != -1)
				keys.pushBack(e.key());
		}
		return keys;
	}

	Vector<std::pair<K, V>> contents()
	{
		Vector<std::pair<K, V>> pairs = Vector<std::pair<K, V>>();
		pairs.reserve(m_size);
		for (const Entry& e : m_table)
		{
			if (e.psl != -1)
				pairs.pushBack(e.kv);
		}
		return pairs;
	}

	V& operator[](int index)
	{
		return at(index);
	}

	const V& operator[](int index) const
	{
		return at(index);
	}

private:
	int _hash(const K& key) const
	{
		return std::hash<K>{}(key) % m_capacity;
	}

	void _resize()
	{
		m_capacity *= 2;
		m_table.reserve(m_capacity);
	}

	void _swap_entries(Entry& a, Entry& b)
	{
		Entry tmp = Entry(a);
		a = b;
		b = tmp;
	}

private:
	Vector<Entry> m_table;
	int m_capacity;
	int m_size;
	int m_maxPSL = 0;
};