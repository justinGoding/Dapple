/*
* https://github.com/alessandrobelli/MyVectorCpp/blob/main/Vector.hpp
*/

#pragma once

#include <type_traits>
#include <stdexcept>

const int DEFAULT_BUFFER_SIZE = 8;

template <typename T>
class Buffer
{
public:
	Buffer()
	{
		m_size = DEFAULT_BUFFER_SIZE;
		m_alignment = 0;
		m_elements = _alloc_buffer(m_size * sizeof(T));
	}

	Buffer(int size, size_t alignment = 0)
	{
		// Initialize members to a safe default before attempting allocation
		m_elements = nullptr;
		m_size = 0;
		m_alignment = alignment;

		if (size == 0)
		{
			return;
		}

		// size > 0 : allocate raw memory
		m_size = size;
		m_elements = _alloc_buffer(m_size * sizeof(T));
	}

	Buffer(int size, const T& value, size_t alignment = 0)
	{
		// Initialize members to a safe default before attempting allocation
		m_elements = nullptr;
		m_size = 0;
		m_alignment = alignment;

		if (size == 0)
		{
			return;
		}

		// size > 0 : allocate raw memory
		m_size = size;
		m_elements = _alloc_buffer(m_size * sizeof(T));

		try
		{
			_construct_fill_elements(m_elements, size, value);
		}
		catch (...)
		{
			// Helper failed (and cleaned up its partial constructions)
			// Now the Buffer constructor must clean up the raw buffer it allocated
			::operator delete[](m_elements);
			m_elements = nullptr; // Reset members to a safe, empty state
			m_size = 0;
			throw; // Re-throw to indicate Buffer construction failed
		}
	}

	Buffer(const Buffer& other)
	{
		m_elements = nullptr;
		m_size = 0;
		m_alignment = other.m_alignment;

		if (other.m_size == 0)
		{
			return;
		}

		// other.size > 0
		int target_size = other.m_size;

		T* temp_elements = nullptr;
		try
		{
			temp_elements = _alloc_buffer(target_size * sizeof(T));

			// Call helper, it handles its own partial construction cleanup and re-throws
			_construct_copy_from_range(temp_elements, other.m_elements, other.m_size);

			// If all successful, commit to the Vector's members
			m_elements = temp_elements;
			m_size = other.m_size;
		}
		catch (...)
		{
			// This catches std::bad_alloc from the new[] above OR
			// an exception re-thrown by _construct_copy_from_range
			if (temp_elements != nullptr)
			{
				::operator delete[](temp_elements);
			}
			// elements and size remain nullptr/0 from initialization
			throw;
		}
	}

	Buffer(Buffer&& other) noexcept
		: m_size(other.m_size), m_elements(other.m_elements)
	{
		// Leave 'other' in a valid, destructible, but empty state
		other.m_elements = nullptr;
		other.m_size = 0;
	}

	~Buffer()
	{
		_destroy_range(m_elements, m_size);
		::operator delete[](m_elements);
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

	void swap(Buffer& other) noexcept
	{
		std::swap(m_elements, other.m_elements);
		std::swap(m_size, other.m_size);
	}

	T& operator[](int index)
	{
		return m_elements[index];
	}

	const T& operator[](int index) const
	{
		return m_elements[index];
	}

	Buffer& operator=(const Buffer& rhs)
	{
		if (this == &rhs)
			return *this;

		Buffer temp_copy(rhs);
		swap(temp_copy);
		return *this;
	}

	Buffer& operator=(Buffer&& rhs) noexcept
	{
		if (this != &rhs) // Prevent self-assignment
		{
			_destroy_range(m_elements, m_size); // Call destructors for existing elements in this
			::operator delete[](m_elements);	// Deallocate this buffer's raw memory

			// Now proceed to take rhs's resources
			m_elements = rhs.m_elements;
			m_size = rhs.m_size;

			rhs.m_elements = nullptr;
			rhs.m_size = 0;
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
				// Explicitly call destructor for the T object at this memory location
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
	size_t m_alignment = 0;
};