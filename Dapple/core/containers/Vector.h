/*
* https://github.com/alessandrobelli/MyVectorCpp/blob/main/Vector.hpp
*/

#pragma once

#include <type_traits>
#include <stdexcept>

const int DEFAULT_VECTOR_CAPACITY = 8;

template <typename T>
class Vector
{
public:
	Vector()
	{
		m_size = 0;
		m_capacity = DEFAULT_VECTOR_CAPACITY;
		m_alignment = 0;
		m_elements = _alloc_buffer(m_capacity * sizeof(T));
	}

	Vector(int n, const T& value, size_t alignment = 0)
	{
		// Initialize members to a safe default before attempting allocation
		m_elements = nullptr;
		m_size = 0;
		m_capacity = 0;
		m_alignment = alignment;

		if (n == 0)
		{
			m_capacity = DEFAULT_VECTOR_CAPACITY;
			if (m_capacity > 0)
			{
				m_elements = _alloc_buffer(m_capacity * sizeof(T));
				// No elements to construct, size remains 0
			}
			return;
		}

		// n > 0 - allocate raw memory
		m_capacity = n * 2;
		m_elements = _alloc_buffer(m_capacity * sizeof(T));

		try
		{
			_construct_fill_elements(m_elements, n, value);
			m_size = n;
		}
		catch (...)
		{
			// Helper failed (and cleaned up its partial constructions)
			// Now the Vector constructor must clean up the raw buffer it allocated
			::operator delete[](m_elements);
			m_elements = nullptr; // Reset members to a safe, empty state
			m_capacity = 0;
			m_size = 0;
			throw; // Re-throw to indicate Vector construction failed
		}
	}

	Vector(const Vector& rhs)
	{
		m_elements = nullptr;
		m_size = 0;
		m_capacity = 0;
		m_alignment = rhs.m_alignment;

		if (rhs.m_size == 0)
		{
			if (rhs.m_capacity > 0)
			{
				// Allocate for capacity, but vector remains logically empty (size=0)
				try
				{
					m_capacity = rhs.m_capacity;
					m_elements = _alloc_buffer(m_capacity * sizeof(T));
				}
				catch (const std::bad_alloc&)
				{
					m_elements = nullptr;
					m_capacity = 0;
					throw;
				}
			}
			// If rhs.capacity was 0, elements is already nullptr, size 0, capacity 0
			return;
		}

		// rhs.size > 0
		int target_capacity = rhs.m_capacity;
		if (target_capacity < rhs.m_size)
		{
			target_capacity = rhs.m_size;
		}

		T* temp_elements = nullptr;
		try
		{
			temp_elements = _alloc_buffer(target_capacity * sizeof(T));

			// Call helper, it handles its own partial construction cleanup and re-throws
			_construct_copy_from_range(temp_elements, rhs.m_elements, rhs.m_size);

			// If all successful, commit to the Vector's members
			m_elements = temp_elements;
			m_size = rhs.m_size;
			m_capacity = target_capacity;
		}
		catch (...)
		{
			// This catches std::bad_alloc from the new[] above OR
			// an exception re-thrown by _construct_copy_from_range
			if (temp_elements != nullptr)
			{
				::operator delete[](temp_elements);
			}
			// elements, size, capacity remain nullptr/0 from initialization
			throw;
		}
	}

	~Vector()
	{
		_destroy_range(m_elements, m_size);
		::operator delete[](m_elements);
	}

	Vector(Vector&& other) noexcept
		: m_size(other.m_size), m_capacity(other.m_capacity), m_elements(other.m_elements)
	{
		// Leave 'other' in a valid, destructible, but empty state
		other.m_elements = nullptr;
		other.m_size = 0;
		other.m_capacity = 0;
	}

	void clear()
	{
		_destroy_range(m_elements, m_size);
		m_size = 0;
	}

	T* begin()
	{
		return m_elements;
	}

	T* end()
	{
		return m_elements + m_size;
	}

	T& at(int index)
	{
		if (index < 0 || index >= m_size)
		{
			throw std::out_of_range("Vector::at: index out of range");
		}
		return m_elements[index];
	}

	const T& at(int index) const
	{
		if (index < 0 || index >= m_size)
		{
			throw std::out_of_range("Vector::at: index out of range (const)");
		}
		return m_elements[index];
	}

	const T* begin() const
	{
		return m_elements;
	}

	const T* end() const
	{
		return m_elements + m_size;
	}

	const T* cbegin() const
	{
		return m_elements;
	}

	const T* cend() const
	{
		return m_elements + size;
	}

	int size() const
	{
		return m_size;
	}

	int capacity() const
	{
		return m_capacity;
	}

	bool isEmpty() const
	{
		return m_size == 0;
	}

	void shrinkToFit()
	{
		if (m_capacity == m_size)
			return;

		if (m_size == 0)
		{
			::operator delete[](m_elements);
			m_elements = nullptr;
			m_capacity = 0;
			return;
		}

		T* temp_elements = _alloc_buffer(m_size * sizeof(T));

		try
		{
			_construct_move_from_range(temp_elements, m_elements, m_size);
		}
		catch (...)
		{
			::operator delete[](temp_elements);
			throw; // For simplicity here, rethrowing. A robust shrink might not throw
		}

		_destroy_range(m_elements, m_size);
		::operator delete[](m_elements);

		m_elements = temp_elements;
		m_capacity = m_size;
	}

	void pushBack(const T& object)
	{
		if (m_size == m_capacity)
		{
			int new_capacity = (m_capacity == 0) ? DEFAULT_VECTOR_CAPACITY : m_capacity * 2;
			reserve(new_capacity);
		}

		try
		{
			new (m_elements + m_size) T(object);
			m_size++;
		}
		catch (...)
		{
			// If construction fails, size is not incremented, no specific cleanup needed here
			// as the slot was uninitialized
			throw;
		}
	}

	void pushBack(T&& object)
	{
		if (m_size == m_capacity)
		{
			int new_cap = (m_capacity == 0) ? DEFAULT_VECTOR_CAPACITY : m_capacity * 2;
			Reserve(new_cap);
		}

		try
		{
			new (m_elements + m_size) T(std::move(object));
			m_size++;
		}
		catch (...)
		{
			throw;
		}
	}

	void popBack()
	{
		if (m_size > 0)
		{
			if constexpr (!std::is_trivially_destructible_v<T>)
				m_elements[size - 1].~T();
			m_size--;
		}
	}

	void erase(int index)
	{
		for (int i = index; i < size - 1; i++)
		{
			m_elements[i] = std::move(m_elements[i + 1]);
		}

		if constexpr (!std::is_trivially_destructible_v<T>)
			m_elements[m_size - 1].~T();

		size--;
	}

	void unorderedErase(int index)
	{
		m_elements[index] = std::move(m_elements[size - 1]); // Move last element into the gap
		popBack();
	}

	void insert(int index, const T& value)
	{
		if (index < 0 || index > m_size)
		{
			throw std::out_of_range("Vector::insert: index out of range");
		}

		if (m_size == m_capacity)
		{
			int new_capacity = (m_capacity == 0) ? DEFAULT_VECTOR_CAPACITY : m_capacity * 2;
			if (new_capacity == 0)
				new_capacity = 1;

			T* temp_elements = _alloc_buffer(new_capacity * sizeof(T));
			int constructed_in_new = 0;
			try
			{
				// Move elements before index
				_construct_move_from_range(temp_elements, m_elements, index);
				constructed_in_new = index;

				// Construct the new value
				new (temp_elements + index) T(value);
				constructed_in_new++;

				// Move elements after index
				_construct_move_from_range(temp_elements + index + 1, m_elements + index, m_size - index);
				constructed_in_new += (m_size - index);
			}
			catch (...)
			{
				_destroy_range(temp_elements, constructed_in_new);
				::operator delete[](temp_elements);
				throw;
			}

			_destroy_range(m_elements, m_size);
			::operator delete[](m_elements);
			m_elements = temp_elements;
			m_capacity = new_capacity;
		}
		else
		{
			// No reallocation needed, just shift elements
			// This requires constructing into uninitialized memory if T is complex
			// or careful move assignment and destruction
			// Start from the end to make space
			if (index < m_size)
			{
				// If not inserting at the very end
				// construct the last element moved into the new slot
				new (m_elements + m_size) T(std::move(m_elements[m_size - 1]));
				// Move assign for intermediate elements
				for (int i = m_size - 1; i > index; i--)
				{
					m_elements[i] = std::move(m_elements[i - 1]);	// Assumes T is move-assignable
					// And m_elements[i] is already a valid object
					// This part is simpler if m_elements[i] is destructed
					// and then move-constructed
					// For now, direct move assignment
				}
				// Construct the new value at index using placement new
				// If m_elements[index] was already a valid object, it needs destructed first
				// Explicity call destructor for the T object at this memory location
				(m_elements + index)->~T();
				new (m_elements + index) T(value);
			}
			else
			{
				// Inserting at m_elements + m_size (which is uninitialized)
				new (m_elements + index) T(value);
			}
		}
		m_size++;
	}

	void insert(int index, T&& value)
	{
		// Similar logic to const T& version, but using std::move(value)
		// and T(std::move(other_element)) for constructions
		// This is non-trivial to get perfectly exception-safe without more helpers
		// For brevity here, a simplified adaptation

		if (index < 0 || index > m_size)
		{
			throw std::out_of_range("Vector::insert: index out of range");
		}

		if (m_size == m_capacity)
		{
			int new_capacity = (m_capacity == 0) ? DEFAULT_VECTOR_CAPACITY : m_capacity * 2;
			if (new_capacity == 0)
				new_capacity = 1;

			T* temp_elements = _alloc_buffer(new_capacity * sizeof(T));
			int constructed_in_new = 0;
			try
			{
				// Move elements before index
				_construct_move_from_range(temp_elements, m_elements, index);
				constructed_in_new = index;

				// Construct the new value
				new (temp_elements + index) T(std::move(value));
				constructed_in_new++;

				// Move elements after index
				_construct_move_from_range(temp_elements + index + 1, m_elements + index, m_size - index);
				constructed_in_new += (m_size - index);
			}
			catch (...)
			{
				_destroy_range(temp_elements, constructed_in_new);
				::operator delete[](temp_elements);
				throw;
			}

			_destroy_range(m_elements, m_size);
			::operator delete[](m_elements);
			m_elements = temp_elements;
			m_capacity = new_capacity;
		}
		else
		{
			// No reallocation needed, just shift elements
			// This requires constructing into uninitialized memory if T is complex
			// or careful move assignment and destruction
			// Start from the end to make space
			if (index < m_size)
			{
				// If not inserting at the very end
				// construct the last element moved into the new slot
				new (m_elements + m_size) T(std::move(m_elements[m_size - 1]));
				// Move assign for intermediate elements
				for (int i = m_size - 1; i > index; i--)
				{
					m_elements[i] = std::move(m_elements[i - 1]);	// Assumes T is move-assignable
					// And m_elements[i] is already a valid object
					// This part is simpler if m_elements[i] is destructed
					// and then move-constructed
					// For now, direct move assignment
				}
				// Construct the new value at index using placement new
				// If m_elements[index] was already a valid object, it needs destructed first
				// Explicity call destructor for the T object at this memory location
				(m_elements + index)->~T();
				new (m_elements + index) T(std::move(value));
			}
			else
			{
				// Inserting at m_elements + m_size (which is uninitialized)
				new (m_elements + index) T(std::move(value));
			}
		}
		m_size++;
	}

	void swap(Vector& other) noexcept
	{
		std::swap(m_elements, other.m_elements);
		std::swap(m_size, other.m_size);
		std::swap(m_capacity, other.m_capacity);
	}

	// Pre-allocate memory to ensure capacity for at least newCapacity elements
	// Use this to avoid multiple slow reallocations (and potential stutters)
	// if you know roughly how many elments you'll need
	void reserve(int new_capacity_arg)
	{
		if (new_capacity_arg <= m_capacity)
		{
			return;
		}

		T* temp_elements = _alloc_buffer(new_capacity_arg * sizeof(T));

		if (m_elements != nullptr)
		{
			try
			{
				_construct_move_from_range(temp_elements, m_elements, m_size);
			}
			catch (...)
			{
				::operator delete[](temp_elements);
				throw;
			}
			_destroy_range(m_elements, m_size);
			::operator delete[](m_elements);
		}

		m_elements = temp_elements;
		m_capacity = new_capacity_arg;
	}

	T& operator[](int index)
	{
		return m_elements[index];
	}

	const T& operator[](int index) const
	{
		return m_elements[index];
	}

	Vector& operator=(const Vector& rhs)
	{
		if (this == &rhs)
			return *this;

		Vector temp_copy(rhs);
		swap(temp_copy);
		return *this;
	}

	Vector& operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs) // Prevent self-assignment
		{
			_destroy_range(m_elements, m_size); // Call destructors for existing elements in this
			::operator delete[](m_elements);	// Deallocate this vector's raw memory

			// Now proceed to take rhs's resources
			m_elements = rhs.m_elements;
			m_size = rhs.m_size;
			m_capacity = rhs.m_capacity;

			rhs.m_elements = nullptr;
			rhs.m_size = 0;
			rhs.m_capacity = 0;
		}
		return *this;
	}

private:
	void _construct_fill_elements(T* target_buffer, int num_to_construct, const T& value_source)
	{
		int successfully_constructed_count = 0;
		try
		{
			for (int i = 0; i < num_to_construct; i++)
			{
				new (target_buffer + i) T(value_source);
				successfully_constructed_count++;
			}
		}
		catch (...)
		{
			for (int i = 0; i < successfully_constructed_count; i++)
			{
				(target_buffer + i)->~T();
			}
			throw;
		}
	}

	void _construct_copy_from_range(T* dest_buffer, const T* source_elements_start, int num_to_copy)
	{
		int constructed_count = 0;
		try
		{
			for (int i = 0; i < num_to_copy; i++)
			{
				// by calling T's copy constructor with the corresponding source element
				new (dest_buffer + i) T(*(source_elements_start + i));
				constructed_count++;
			}
		}
		catch (...)
		{
			for (int i = 0; i < constructed_count; i++)
			{
				// Explicity call destructor for the T object at this memory location
				(dest_buffer + i)->~T();
			}
			throw;
		}
	}

	void _destroy_range(T* buffer_start, int count)
	{
		if (!std::is_trivially_destructible_v<T>)
		{
			for (int i = 0; i < count; i++)
			{
				// Explicity call destructor for the T object at this memory location
				(buffer_start + i)->~T();
			}
		}
	}

	void _construct_move_from_range(T* dest_buffer, T* source_elements_start, int num_to_move)
	{
		int constructed_count = 0;
		try
		{
			for (int i = 0; i < num_to_move; i++)
			{
				new (dest_buffer + i) T(std::move(*(source_elements_start + i)));
				constructed_count++;
			}
		}
		catch (...)
		{
			_destroy_range(dest_buffer, constructed_count);
			throw;
		}
	}

	T* _alloc_buffer(size_t size)
	{
		if (m_alignment > 0)
		{
			return static_cast<T*>(::operator new[](size, std::align_val_t(m_alignment)));
		}
		return static_cast<T*>(::operator new[](size));
	}

private:
	size_t m_size;
	T* m_elements;
	size_t m_capacity;
	size_t m_alignment = 0;
};