//===---------------------------------------------------------
//                       DYNARRAY
//===---------------------------------------------------------
//
// Slightly adjusted custom implementation for
// std::experimental::dynarray container.
// This version of this container in contrast is not
// designed to be used with stack allocation schemes
// and so requires heap memory (like std::vector)
// to store its elements.
// This change of behaviour implies minor
// adjustments to the API.
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// The header-only declaration and definition is
// contained entirely in this single header file.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
// Author Robin Freyler (C) 2016
// Licence: MIT
//===---------------------------------------------------------

#ifndef UTILS_DYNARRAY_HPP
#define UTILS_DYNARRAY_HPP

// headers used by declaration site
#include <cstddef>
#include <memory>
#include <iterator>
#include <initializer_list>

// headers used by definition site
#include <algorithm>
#include <stdexcept>

//============================================================
// DECLARATION
//============================================================

namespace utils {
	/// From cppreference.com:
	/// ( http://en.cppreference.com/w/cpp/container/dynarray )
	///
	/// dynarray is a sequence container that encapsulates arrays with a size
	/// that is fixed at construction and does not change throughout the lifetime of the object.
	/// 
	/// The elements are stored contiguously, which means that elements can be
	/// accessed not only through iterators, but also using offsets on regular pointers to elements.
	/// This means that a pointer to an element of a dynarray may be passed to any
	/// function that expects a pointer to an element of an array.
	///
	/// There is a special case for a zero-length array
	/// (the number of elements was specified as zero during construction).
	/// In that case, array.begin() == array.end(), which is some unique value.
	/// The effect of calling front() or back() on a zero-sized dynarray is undefined.
	template<typename T>
	class dynarray {
	public:

	//============================================================
	// Type aliases
	//============================================================

		using value_type             = T;
		using size_type              = size_t;
		using difference_type        = std::ptrdiff_t;
		using reference              = value_type &;
		using const_reference        = value_type const&;
		using pointer                = value_type *;
		using const_pointer          = value_type const*;
		using iterator               = pointer;
		using const_iterator         = const_pointer;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	//============================================================
	// Constructors
	//============================================================

	// (1) construct by count
	//============================================================
		explicit dynarray(size_type count);

		template<class Alloc>
		dynarray(size_type count, Alloc const& alloc);

	// (2) construct by count and copied value
	//============================================================
		dynarray(size_type count, T const& value);

		template<class Alloc>
		dynarray(size_type count, T const& value, Alloc const& alloc);

	// (3) copy-construct
	//============================================================
		dynarray(dynarray const& other);

		template<class Alloc>
		dynarray(dynarray const& other, Alloc const& alloc);

	// (4) move-construct
	//============================================================
		dynarray(dynarray && other);

	// (5) construct by initializer list
	//============================================================
		dynarray(std::initializer_list<T> init);

		template<class Alloc>
		dynarray(std::initializer_list<T> list, Alloc const& alloc);

	//============================================================
	// Assignment Operator
	//============================================================

		/// Copy-Assigns from the specified \other dynarray instance.
		/// Throws an invalid_argument exception when the sizes of both
		/// dynarrays are unequal.
		///
		/// Not: It is not easily possible to use a copy reconstruction
		/// for dynarrays of unequal sizes since there is no certain
		/// knowledge of an allocator to allocate the required memory
		/// for this procedure.
		auto operator=(dynarray const& other) -> dynarray &;

		/// Move-Assigns from the specified \other dynarray instance.
		auto operator=(dynarray && other) -> dynarray &;

		/// Copy-Assigns from the specified \list initializer_list instance.
		/// Throws an invalid_argument exception when the sizes of both
		/// containers are unequal.
		///
		/// Note: It is not easily possible to use a copy reconstruction
		/// for dynarrays of unequal sizes since there is no certain
		/// knowledge of an allocator to allocate the required memory
		/// for this procedure.
		auto operator=(std::initializer_list<T> list) -> dynarray &;

	//============================================================
	// Access API
	//============================================================

		/// Access the element at the specified position \pos with bounds checking.
		/// Throws out_of_bounds exception if \pos was illegal.
		auto at(size_type pos) -> reference;

		/// Read-only access to the element at the specified position \pos with bounds checking.
		/// Throws out_of_bounds exception if \pos was illegal.
		auto at(size_type pos) const -> const_reference;

		/// Access the element at the specified position \pos without bounds checking.
		auto operator[](size_type pos) -> reference;

		/// Read-only ccess the element at the specified position \pos without bounds checking.
		auto operator[](size_type pos) const -> const_reference;

		/// Access the first element.
		auto front() -> reference;

		/// Read-only access the first element.
		auto front() const -> const_reference;

		/// Access the last element.
		auto back() -> reference;

		/// Read-only access the last element.
		auto back() const -> const_reference;

		/// Returns a raw-pointer to the underlying data buffer.
		auto data() -> pointer;

		/// Returns a read-only raw-pointer to the underlying data buffer.
		auto data() const -> const_pointer;

	//============================================================
	// Capacity API
	//============================================================

		/// Returns `true` if this dynarray is empty and `false` otherwise.
		auto empty() const -> bool;

		/// Returns the count of elements in this dynarray.
		auto size() const -> size_type;

	//============================================================
	// Mutate API
	//============================================================

		/// Fills this dynarray with elements equal to the specified \value.
		void fill(T const& value);

	//============================================================
	// Iterator API
	// Compatible with: cplusplus.com/reference/iterator/
	//============================================================

		/// Returns an iterator to the first element in this dynarray.
		auto begin()        -> iterator;

		/// Returns a read-only iterator to the first element in this dynarray.
		auto begin() const  -> const_iterator;

		/// Returns a read-only iterator to the first element in this dynarray.
		auto cbegin() const -> const_iterator;


		/// Returns an iterator to the position behind the last element in this dynarray.
		auto end()        -> iterator;

		/// Returns a read-only iterator to the position behind the last element in this dynarray.
		auto end() const  -> const_iterator;

		/// Returns a read-only iterator to the position behind the last element in this dynarray.
		auto cend() const -> const_iterator;

		/// Returns an iterator to the first element in this dynarray
		/// in respective to the reverse order of elements.
		auto rbegin()        -> reverse_iterator;

		/// Returns a read-only iterator to the first element in this dynarray
		/// in respective to the reverse order of elements.
		auto rbegin() const  -> const_reverse_iterator;

		/// Returns a read-only iterator to the first element in this dynarray
		/// in respective to the reverse order of elements.
		auto crbegin() const -> const_reverse_iterator;

		/// Returns an iterator to the position behind the last element
		/// in this dynarray in respective to the reverse order of elements.
		auto rend()        -> reverse_iterator;

		/// Returns a read-only iterator to the position behind the last element
		/// in this dynarray in respective to the reverse order of elements.
		auto rend() const  -> const_reverse_iterator;

		/// Returns a read-only iterator to the position behind the last element
		/// in this dynarray in respective to the reverse order of elements.
		auto crend() const -> const_reverse_iterator;

	//============================================================
	// Member Variables
	//============================================================

	private:
		std::unique_ptr<T[]> m_data;
		size_type            m_size;
	};
}

//============================================================
// IMPLEMENTATION
//============================================================

// (1) construct by count
//============================================================
template<typename T>
utils::dynarray<T>::dynarray(size_type count):
	m_data{new T[count]},
	m_size{count}
{}

template<typename T>
template<class Alloc>
utils::dynarray<T>::dynarray(size_type count, Alloc const& alloc):
	m_data{alloc.allocate(count)},
	m_size{count}
{}

// (2) construct by count and copied value
//============================================================
template<typename T>
utils::dynarray<T>::dynarray(size_type count, T const& value):
	m_data{new T[count]},
	m_size{count}
{
	std::fill(begin(), end(), value);
}

template<typename T>
template<class Alloc>
utils::dynarray<T>::dynarray(size_type count, T const& value, Alloc const& alloc):
	m_data{alloc.allocate(count)},
	m_size{count}
{
	std::fill(begin(), end(), value);
}

// (3) copy-construct
//============================================================
template<typename T>
utils::dynarray<T>::dynarray(dynarray const& other):
	m_data{new T[other.size()]},
	m_size{other.size()}
{
	std::copy(other.begin(), other.end(), begin());
}

template<typename T>
template<class Alloc>
utils::dynarray<T>::dynarray(dynarray const& other, Alloc const& alloc):
	m_data{alloc.allocate(other.size())},
	m_size{other.size()}
{
	std::copy(other.begin(), other.end(), begin());
}

// (4) move-construct
//============================================================
template<typename T>
utils::dynarray<T>::dynarray(dynarray && other):
	m_data{std::move(other.m_data)},
	m_size{other.size()}
{}

// (5) construct by initializer list
//============================================================
template<typename T>
utils::dynarray<T>::dynarray(std::initializer_list<T> list):
	m_data{new T[list.size()]},
	m_size{list.size()}
{
	std::copy(list.begin(), list.end(), begin());
}

template<typename T>
template<class Alloc>
utils::dynarray<T>::dynarray(std::initializer_list<T> list, Alloc const& alloc):
	m_data{alloc.allocate(list.size())},
	m_size{list.size()}
{
	std::copy(list.begin(), list.end(), begin());
}

//============================================================
// Assignment Operator
//============================================================

template<typename T>
auto utils::dynarray<T>::operator=(dynarray const& other) -> dynarray & {
	if (size() != other.size()) {
		using namespace std::string_literals;
		throw std::invalid_argument{
			"cannot copy-assign dynarray of size "s +
			std::to_string(other.size()) +
			" into dynarray of size " +
			std::to_string(size())
		};
	}
	std::copy(other.begin(), other.end(), begin());
	return *this;
}

template<typename T>
auto utils::dynarray<T>::operator=(dynarray && other) -> dynarray & {
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	return *this;
}

template<typename T>
auto utils::dynarray<T>::operator=(std::initializer_list<T> list) -> dynarray & {
	if (size() != list.size()) {
		using namespace std::string_literals;
		throw std::invalid_argument{
			"cannot copy-assign initializer_list of size "s +
			std::to_string(list.size()) +
			" into dynarray of size " +
			std::to_string(size())
		};
	}
	std::copy(list.begin(), list.end(), begin());
	return *this;
}

//============================================================
// Access API
//============================================================

template<typename T>
auto utils::dynarray<T>::at(size_type pos) -> reference {
	if (pos >= size()) {
		using namespace std::string_literals;
		throw std::out_of_range{
			"cannot access element at position "s + 
			std::to_string(pos) + 
			" from a dynarray with size " +
			std::to_string(size())
		};
	}
	return m_data[pos];
}

template<typename T>
auto utils::dynarray<T>::at(size_type pos) const -> const_reference {
	if (pos >= size()) {
		using namespace std::string_literals;
		throw std::out_of_range{
			"cannot access element at position "s + 
			std::to_string(pos) + 
			" from a dynarray with size " +
			std::to_string(size())
		};
	}
	return m_data[pos];
}

template<typename T>
auto utils::dynarray<T>::operator[](size_type pos) -> reference {
	return m_data[pos];
}

template<typename T>
auto utils::dynarray<T>::operator[](size_type pos) const -> const_reference {
	return m_data[pos];
}

template<typename T>
auto utils::dynarray<T>::front() -> reference {
	return m_data[0];
}

template<typename T>
auto utils::dynarray<T>::front() const -> const_reference {
	return m_data[0];
}

template<typename T>
auto utils::dynarray<T>::back() -> reference {
	return m_data[size() - 1];
}

template<typename T>
auto utils::dynarray<T>::back() const -> const_reference {
	return m_data[size() - 1];
}

template<typename T>
auto utils::dynarray<T>::data() -> pointer {
	return m_data.data();
}

template<typename T>
auto utils::dynarray<T>::data() const -> const_pointer {
	return m_data.data();
}

//============================================================
// Capacity API
//============================================================

template<typename T>
auto utils::dynarray<T>::empty() const -> bool {
	return m_size == 0;
}

template<typename T>
auto utils::dynarray<T>::size() const -> size_type {
	return m_size;
}

//============================================================
// Mutate API
//============================================================

template<typename T>
void utils::dynarray<T>::fill(T const& value) {
	std::fill(begin(), end(), value);
}

//============================================================
// Iterator API
//============================================================

template<typename T>
auto utils::dynarray<T>::begin() -> iterator {
	return m_data.data();
}

template<typename T>
auto utils::dynarray<T>::begin() const -> const_iterator {
	return m_data.data();
}

template<typename T>
auto utils::dynarray<T>::cbegin() const -> const_iterator {
	return m_data.data();
}


template<typename T>
auto utils::dynarray<T>::end() -> iterator {
	return m_data.data() + size();
}

template<typename T>
auto utils::dynarray<T>::end() const -> const_iterator {
	return m_data.data() + size();
}

template<typename T>
auto utils::dynarray<T>::cend() const -> const_iterator {
	return m_data.data() + size();
}


template<typename T>
auto utils::dynarray<T>::rbegin() -> reverse_iterator {
	return reverse_iterator{end()};
}

template<typename T>
auto utils::dynarray<T>::rbegin() const -> const_reverse_iterator {
	return reverse_iterator{end()};
}

template<typename T>
auto utils::dynarray<T>::crbegin() const -> const_reverse_iterator {
	return reverse_iterator{cend()};
}


template<typename T>
auto utils::dynarray<T>::rend() -> reverse_iterator {
	return reverse_iterator{begin()};
}

template<typename T>
auto utils::dynarray<T>::rend() const -> const_reverse_iterator {
	return reverse_iterator{begin()};
}

template<typename T>
auto utils::dynarray<T>::crend() const -> const_reverse_iterator {
	return reverse_iterator{cbegin()};
}

#endif // UTILS_DYNARRAY_HPP