// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2014 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "mistream.h"
#include "mostream.h"

namespace ustl {

/// \class array array.h stl.h
/// \ingroup Sequences
///
/// \brief A fixed-size array of \p N \p Ts.
///
template <typename T, size_t N>
class array {
public:
    using value_type		= T;
    using size_type		= unsigned;
    using pointer		= value_type*;
    using const_pointer		= const value_type*;
    using reference		= value_type&;
    using const_reference	= const value_type&;
    using iterator		= pointer;
    using const_iterator	= const_pointer;
    using reverse_iterator	= ::ustl::reverse_iterator<iterator>;
    using const_reverse_iterator = ::ustl::reverse_iterator<const_iterator>;
    using initlist_t		= std::initializer_list<value_type>;
public:
    constexpr auto&		operator+= (initlist_t v)		{ for (size_type i = 0; i < N; ++i) _v[i] += v.begin()[i]; return *this; }
    constexpr auto&		operator-= (initlist_t v)		{ for (size_type i = 0; i < N; ++i) _v[i] -= v.begin()[i]; return *this; }
    constexpr auto&		operator*= (initlist_t v)		{ for (size_type i = 0; i < N; ++i) _v[i] *= v.begin()[i]; return *this; }
    constexpr auto&		operator/= (initlist_t v)		{ for (size_type i = 0; i < N; ++i) _v[i] /= v.begin()[i]; return *this; }
    constexpr auto		operator+ (initlist_t v) const		{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] + v.begin()[i]; return result; }
    constexpr auto		operator- (initlist_t v) const		{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] - v.begin()[i]; return result; }
    constexpr auto		operator* (initlist_t v) const		{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] * v.begin()[i]; return result; }
    constexpr auto		operator/ (initlist_t v) const		{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] / v.begin()[i]; return result; }
    constexpr iterator		begin (void)				{ return _v; }
    constexpr iterator		end (void)				{ return begin() + N; }
    constexpr reference		at (size_type i)			{ return _v[i]; }
    constexpr reference		operator[] (size_type i)		{ return _v[i]; }
    constexpr const_iterator	begin (void) const		{ return _v; }
    constexpr const_iterator	end (void) const		{ return begin() + N; }
    constexpr size_type		size (void) const		{ return N; }
    constexpr size_type		max_size (void) const		{ return N; }
    constexpr bool		empty (void) const		{ return N == 0; }
    constexpr const_reference	at (size_type i) const		{ return _v[i]; }
    constexpr const_reference	operator[] (size_type i) const	{ return _v[i]; }
    inline void			read (istream& is)			{ nr_container_read (is, *this); }
    inline void			write (ostream& os) const		{ nr_container_write (os, *this); }
    inline void			text_write (ostringstream& os) const	{ container_text_write (os, *this); }
    constexpr size_t		stream_size (void) const		{ return nr_container_stream_size (*this); }
    constexpr array&		operator+= (const_reference v)		{ for (size_type i = 0; i < N; ++i) _v[i] += v; return *this; }
    constexpr array&		operator-= (const_reference v)		{ for (size_type i = 0; i < N; ++i) _v[i] -= v; return *this; }
    constexpr array&		operator*= (const_reference v)		{ for (size_type i = 0; i < N; ++i) _v[i] *= v; return *this; }
    constexpr array&		operator/= (const_reference v)		{ for (size_type i = 0; i < N; ++i) _v[i] /= v; return *this; }
    constexpr array		operator+ (const_reference v) const	{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] + v; return result; }
    constexpr array		operator- (const_reference v) const	{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] - v; return result; }
    constexpr array		operator* (const_reference v) const	{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] * v; return result; }
    constexpr array		operator/ (const_reference v) const	{ array result; for (size_type i = 0; i < N; ++i) result[i] = _v[i] / v; return result; }
    inline void			fill (const_reference v)		{ ::ustl::fill (begin(), end(), v); }
    constexpr void		swap (array& v)				{ swap_ranges (begin(), end(), v.begin()); }
public:
    T				_v [N];
};

//----------------------------------------------------------------------

template <typename T1, size_t N, typename T2>
inline constexpr bool operator== (const array<T1,N>& t1, const array<T2,N>& t2)
{
    for (unsigned i = 0; i < N; ++ i)
	if (t1[i] != t2[i])
	    return false;
    return true;
}

template <typename T1, size_t N, typename T2>
inline constexpr bool operator< (const array<T1,N>& t1, const array<T2,N>& t2)
{
    for (unsigned i = 0; i < N && t1[i] <= t2[i]; ++ i)
	if (t1[i] < t2[i])
	    return true;
    return false;
}

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N>& operator+= (array<T1,N>& t1, const array<T2,N>& t2)
    { for (unsigned i = 0; i < N; ++ i) t1[i] = T1(t1[i] + t2[i]); return t1; }

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N>& operator-= (array<T1,N>& t1, const array<T2,N>& t2)
    { for (unsigned i = 0; i < N; ++ i) t1[i] = T1(t1[i] - t2[i]); return t1; }

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N>& operator*= (array<T1,N>& t1, const array<T2,N>& t2)
    { for (unsigned i = 0; i < N; ++ i) t1[i] = T1(t1[i] * t2[i]); return t1; }

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N>& operator/= (array<T1,N>& t1, const array<T2,N>& t2)
    { for (unsigned i = 0; i < N; ++ i) t1[i] = T1(t1[i] / t2[i]); return t1; }

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N> operator+ (const array<T1,N>& t1, const array<T2,N>& t2)
{
    array<T1,N> result;
    for (unsigned i = 0; i < N; ++ i) result[i] = T1(t1[i] + t2[i]);
    return result;
}

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N> operator- (const array<T1,N>& t1, const array<T2,N>& t2)
{
    array<T1,N> result;
    for (unsigned i = 0; i < N; ++ i) result[i] = T1(t1[i] - t2[i]);
    return result;
}

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N> operator* (const array<T1,N>& t1, const array<T2,N>& t2)
{
    array<T1,N> result;
    for (unsigned i = 0; i < N; ++ i) result[i] = T1(t1[i] * t2[i]);
    return result;
}

template <typename T1, size_t N, typename T2>
inline constexpr const array<T1,N> operator/ (const array<T1,N>& t1, const array<T2,N>& t2)
{
    array<T1,N> result;
    for (unsigned i = 0; i < N; ++ i) result[i] = T1(t1[i] / t2[i]);
    return result;
}

} // namespace ustl
