// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once
#include "uutility.h"
#include "uiterator.h"

namespace ustl {

template <typename T>
inline constexpr typename tm::RemoveReference<T>::Result&& move (T&& v) noexcept
    { return static_cast<typename tm::RemoveReference<T>::Result&&>(v); }

template <typename T>
inline constexpr T&& forward (typename tm::RemoveReference<T>::Result& v) noexcept
    { return static_cast<T&&>(v); }

template <typename T>
inline constexpr T&& forward (typename tm::RemoveReference<T>::Result&& v) noexcept
    { return static_cast<T&&>(v); }

template <typename T, typename U = T>
constexpr auto exchange (T& a, U&& b)
{
    auto t = move(a);
    a = forward<U>(b);
    return t;
}

/// Assigns the contents of a to b and the contents of b to a.
/// This is used as a primitive operation by many other algorithms. 
/// \ingroup SwapAlgorithms
///
template <typename T>
inline constexpr void swap (T& a, T& b)
{
    typename tm::RemoveReference<T>::Result t = move(a);
    a = move(b);
    b = move(t);
}

/// Equivalent to swap (*a, *b)
/// \ingroup SwapAlgorithms
///
template <typename Iterator> 
inline constexpr void iter_swap (Iterator a, Iterator b)
{
    swap (*a, *b);
}

/// Copy copies elements from the range [first, last) to the range
/// [result, result + (last - first)). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. [1] Generally,
/// for every integer n from 0 to last - first, copy performs the assignment
/// *(result + n) = *(first + n). Assignments are performed in forward order,
/// i.e. in order of increasing n. 
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
    for (; first != last; ++result, ++first)
	*result = *first;
    return result;
}

/// Copy_n copies elements from the range [first, first + n) to the range
/// [result, result + n). That is, it performs the assignments
/// *result = *first, *(result + 1) = *(first + 1), and so on. Generally,
/// for every integer i from 0 up to (but not including) n, copy_n performs
/// the assignment *(result + i) = *(first + i). Assignments are performed
/// in forward order, i.e. in order of increasing n.
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy_n (InputIterator first, size_t count, OutputIterator result)
{
    for (; count; --count, ++result, ++first)
	*result = *first;
    return result;
}

/// \brief Copy copies elements from the range (last, first] to result.
/// \ingroup MutatingAlgorithms
/// Copies elements starting at last, decrementing both last and result.
///
template <typename InputIterator, typename OutputIterator>
inline OutputIterator copy_backward (InputIterator first, InputIterator last, OutputIterator result)
{
    while (first != last)
	*--result = *--last;
    return result;
}

/// For_each applies the function object f to each element in the range
/// [first, last); f's return value, if any, is ignored. Applications are
/// performed in forward order, i.e. from first to last. For_each returns
/// the function object after it has been applied to each element.
/// \ingroup MutatingAlgorithms
///
template <typename InputIterator, typename UnaryFunction>
inline UnaryFunction for_each (InputIterator first, InputIterator last, UnaryFunction f)
{
    for (; first != last; ++first)
	f (*first);
    return f;
}

/// Fill assigns the value value to every element in the range [first, last).
/// That is, for every iterator i in [first, last),
/// it performs the assignment *i = value.
/// \ingroup GeneratorAlgorithms
///
template <typename ForwardIterator, typename T>
inline void fill (ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; ++first)
	*first = value;
}

/// Fill_n assigns the value value to every element in the range
/// [first, first+count). That is, for every iterator i in [first, first+count),
/// it performs the assignment *i = value. The return value is first + count.
/// \ingroup GeneratorAlgorithms
///
template <typename OutputIterator, typename T>
inline OutputIterator fill_n (OutputIterator first, size_t count, const T& value)
{
    for (; count; --count, ++first)
	*first = value;
    return first;
}

inline constexpr auto copy_n_fast (const void* src, size_t count, void* dest) noexcept
    { return __builtin_memmove (dest, src, count); }
inline constexpr auto copy_backward_fast (const void* first, const void* last, void* result) noexcept
    { return __builtin_memmove (advance_ptr(result,-distance(first,last)), first, distance(first,last)); }
inline auto fill_n8_fast (uint8_t* dest, size_t count, uint8_t v) noexcept
    { return __builtin_memset (dest, v, count); }
template <typename T> static inline void stosv (T*& p, size_t n, T v)
    { while (n--) *p++ = v; }
#if __x86__
template <> inline void stosv (uint16_t*& p, size_t n, uint16_t v)
    { asm volatile ("rep;\n\tstosw" : "=&D"(p), "=c"(n) : "0"(p), "1"(n), "a"(v) : "memory"); }
template <> inline void stosv (uint32_t*& p, size_t n, uint32_t v)
    { asm volatile ("rep;\n\tstosl" : "=&D"(p), "=c"(n) : "0"(p), "1"(n), "a"(v) : "memory"); }
#endif
inline void fill_n16_fast (uint16_t* dest, size_t count, uint16_t v) noexcept
    { stosv (dest, count, v); }
inline void fill_n32_fast (uint32_t* dest, size_t count, uint32_t v) noexcept
    { stosv (dest, count, v); }

extern "C" void rotate_fast (void* first, void* middle, void* last) noexcept;

/// \brief Computes the number of 1 bits in a number.
/// \ingroup ConditionAlgorithms
inline size_t popcount (uint32_t v)	{ return __builtin_popcount (v); }
inline size_t popcount (uint64_t v)	{ return __builtin_popcountll (v); }

//----------------------------------------------------------------------
// Optimized versions for standard types
//----------------------------------------------------------------------

template <typename T>
inline T* unrolled_copy (const T* first, size_t count, T* result)
{
    copy_n_fast (first, count * sizeof(T), result);
    return result + count;
}

template <>
inline uint8_t* copy_backward (const uint8_t* first, const uint8_t* last, uint8_t* result)
{
    copy_backward_fast (first, last, result);
    return result;
}

template <typename T>
inline T* unrolled_fill (T* result, size_t count, T value)
{
    for (; count; --count, ++result)
	*result = value;
    return result;
}
template <> inline uint8_t* unrolled_fill (uint8_t* result, size_t count, uint8_t value)
    { fill_n8_fast (result, count, value); return result + count; }
template <> inline uint16_t* unrolled_fill (uint16_t* result, size_t count, uint16_t value)
    { fill_n16_fast (result, count, value); return result + count; }
template <> inline uint32_t* unrolled_fill (uint32_t* result, size_t count, uint32_t value)
    { fill_n32_fast (result, count, value); return result + count; }
template <> inline float* unrolled_fill (float* result, size_t count, float value)
    { fill_n32_fast (reinterpret_cast<uint32_t*>(result), count, *noalias_cast<uint32_t*>(&value)); return result + count; }

#define UNROLLED_COPY_SPECIALIZATION(type)						\
template <> inline type* copy (const type* first, const type* last, type* result)	\
{ return unrolled_copy (first, distance (first, last), result); }			\
template <> inline type* copy_n (const type* first, size_t count, type* result)		\
{ return unrolled_copy (first, count, result); }
#define UNROLLED_FILL_SPECIALIZATION(type)						\
template <> inline void fill (type* first, type* last, const type& value)		\
{ unrolled_fill (first, distance (first, last), value); }				\
template <> inline type* fill_n (type* first, size_t count, const type& value)		\
{ return unrolled_fill (first, count, value); }
UNROLLED_COPY_SPECIALIZATION(uint8_t)
UNROLLED_FILL_SPECIALIZATION(uint8_t)
UNROLLED_COPY_SPECIALIZATION(uint16_t)
UNROLLED_FILL_SPECIALIZATION(uint16_t)
UNROLLED_COPY_SPECIALIZATION(uint32_t)
UNROLLED_FILL_SPECIALIZATION(uint32_t)
UNROLLED_COPY_SPECIALIZATION(float)
UNROLLED_FILL_SPECIALIZATION(float)
#undef UNROLLED_FILL_SPECIALIZATION
#undef UNROLLED_COPY_SPECIALIZATION

// Specializations for void* and char*, aliasing the above optimized versions.
//
// All these need duplication with const and non-const arguments, since
// otherwise the compiler will default to the unoptimized version for
// pointers not const in the caller's context, such as local variables.
// These are all inline, but they sure slow down compilation... :(
//
#define COPY_ALIAS_FUNC(ctype, type, alias_type)			\
template <> inline type* copy (ctype* first, ctype* last, type* result)	\
{ return reinterpret_cast<type*> (copy (reinterpret_cast<const alias_type*>(first), reinterpret_cast<const alias_type*>(last), reinterpret_cast<alias_type*>(result))); }
#if HAVE_THREE_CHAR_TYPES
COPY_ALIAS_FUNC(const char, char, uint8_t)
COPY_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_ALIAS_FUNC(const int8_t, int8_t, uint8_t)
COPY_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_ALIAS_FUNC(const int16_t, int16_t, uint16_t)
COPY_ALIAS_FUNC(int16_t, int16_t, uint16_t)
COPY_ALIAS_FUNC(uint16_t, uint16_t, uint16_t)
COPY_ALIAS_FUNC(const int32_t, int32_t, uint32_t)
COPY_ALIAS_FUNC(int32_t, int32_t, uint32_t)
COPY_ALIAS_FUNC(uint32_t, uint32_t, uint32_t)
COPY_ALIAS_FUNC(const void, void, uint8_t)
COPY_ALIAS_FUNC(void, void, uint8_t)
#undef COPY_ALIAS_FUNC
#define COPY_BACKWARD_ALIAS_FUNC(ctype, type, alias_type)				\
template <> inline type* copy_backward (ctype* first, ctype* last, type* result)	\
{ return reinterpret_cast<type*> (copy_backward (reinterpret_cast<const alias_type*>(first), reinterpret_cast<const alias_type*>(last), reinterpret_cast<alias_type*>(result))); }
#if HAVE_THREE_CHAR_TYPES
COPY_BACKWARD_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_BACKWARD_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const uint16_t, uint16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(int16_t, int16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const int16_t, int16_t, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(void, void, uint8_t)
COPY_BACKWARD_ALIAS_FUNC(const void, void, uint8_t)
#undef COPY_BACKWARD_ALIAS_FUNC
#define FILL_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline void fill (type* first, type* last, const v_type& value)	\
{ fill (reinterpret_cast<alias_type*>(first), reinterpret_cast<alias_type*>(last), alias_type(value)); }
FILL_ALIAS_FUNC(void, uint8_t, char)
FILL_ALIAS_FUNC(void, uint8_t, uint8_t)
#if HAVE_THREE_CHAR_TYPES
FILL_ALIAS_FUNC(char, uint8_t, char)
FILL_ALIAS_FUNC(char, uint8_t, uint8_t)
#endif
FILL_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_ALIAS_FUNC(int16_t, uint16_t, int16_t)
FILL_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#undef FILL_ALIAS_FUNC
#define COPY_N_ALIAS_FUNC(ctype, type, alias_type)					\
template <> inline type* copy_n (ctype* first, size_t count, type* result)	\
{ return reinterpret_cast<type*> (copy_n (reinterpret_cast<const alias_type*>(first), count, reinterpret_cast<alias_type*>(result))); }
COPY_N_ALIAS_FUNC(const void, void, uint8_t)
COPY_N_ALIAS_FUNC(void, void, uint8_t)
#if HAVE_THREE_CHAR_TYPES
COPY_N_ALIAS_FUNC(const char, char, uint8_t)
COPY_N_ALIAS_FUNC(char, char, uint8_t)
#endif
COPY_N_ALIAS_FUNC(int8_t, int8_t, uint8_t)
COPY_N_ALIAS_FUNC(uint8_t, uint8_t, uint8_t)
COPY_N_ALIAS_FUNC(const int8_t, int8_t, uint8_t)
COPY_N_ALIAS_FUNC(int16_t, int16_t, uint16_t)
COPY_N_ALIAS_FUNC(uint16_t, uint16_t, uint16_t)
COPY_N_ALIAS_FUNC(const int16_t, int16_t, uint16_t)
COPY_N_ALIAS_FUNC(int32_t, int32_t, uint32_t)
COPY_N_ALIAS_FUNC(uint32_t, uint32_t, uint32_t)
COPY_N_ALIAS_FUNC(const int32_t, int32_t, uint32_t)
#undef COPY_N_ALIAS_FUNC
#define FILL_N_ALIAS_FUNC(type, alias_type, v_type)				\
template <> inline type* fill_n (type* first, size_t n, const v_type& value)	\
{ return reinterpret_cast<type*> (fill_n (reinterpret_cast<alias_type*>(first), n, alias_type(value))); }
FILL_N_ALIAS_FUNC(void, uint8_t, char)
FILL_N_ALIAS_FUNC(void, uint8_t, uint8_t)
#if HAVE_THREE_CHAR_TYPES
FILL_N_ALIAS_FUNC(char, uint8_t, char)
FILL_N_ALIAS_FUNC(char, uint8_t, uint8_t)
#endif
FILL_N_ALIAS_FUNC(int8_t, uint8_t, int8_t)
FILL_N_ALIAS_FUNC(int16_t, uint16_t, int16_t)
FILL_N_ALIAS_FUNC(int32_t, uint32_t, int32_t)
#undef FILL_N_ALIAS_FUNC

extern const char _FmtPrtChr[2][8];

} // namespace ustl
