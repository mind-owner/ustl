// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "stdtest.h"

template <typename T, unsigned N>
struct make_array {};
template <typename T>
struct make_array<T,2> { static array<T,2> make (void) { array<T,2> v {{ 1, 2 }}; return v; } };
template <typename T>
struct make_array<T,4> { static array<T,4> make (void) { array<T,4> v = {{ 1, 2, 3, 4 }}; return v; } };
template <typename T>
struct make_array<T,8> { static array<T,8> make (void) { array<T,8> v {{ 1, 2, 3, 4, 5, 6, 7, 8 }}; return v; } };

template <typename T, size_t N>
static void TestArray (const char* ctrType)
{
    cout << "================================================" << endl;
    cout << "Testing " << ctrType << endl;
    cout << "================================================" << endl;
    assert (N <= 8);
    array<T,N> pt1 (make_array<T,N>::make());
    array<T,N> pt2;
    pt2 = {{4,4}};
    pt2.fill (pt2[0]);
    pt2 += {1,2,3,4,1,2,3,4};

    cout << "pt1:\t\t\tsize = " << pt1.size() << ", value = " << pt1 << endl;
    cout << "pt2:\t\t\t" << pt2 << endl;
    iota (pt2.begin(), pt2.end(), 10);
    cout << "pt2:\t\t\t" << pt2 << endl;

    pt1 *= 3;
    cout << "pt1 *= 3:\t\t" << pt1 << endl;
    pt1 /= 3;
    cout << "pt1 /= 3:\t\t" << pt1 << endl;
    pt1 += 3;
    cout << "pt1 += 3:\t\t" << pt1 << endl;
    pt1 -= 3;
    cout << "pt1 -= 3:\t\t" << pt1 << endl;

    pt1 *= pt2;
    cout << "pt1 *= pt2:\t\t" << pt1 << endl;
    pt1 /= pt2;
    cout << "pt1 /= pt2:\t\t" << pt1 << endl;
    pt1 += pt2;
    cout << "pt1 += pt2:\t\t" << pt1 << endl;
    pt1 -= pt2;
    cout << "pt1 -= pt2:\t\t" << pt1 << endl;

    pt1 = pt1 * pt2;
    cout << "pt1 = pt1 * pt2:\t" << pt1 << endl;
    pt1 = pt1 / pt2;
    cout << "pt1 = pt1 / pt2:\t" << pt1 << endl;
    pt1 = pt1 + pt2;
    cout << "pt1 = pt1 + pt2:\t" << pt1 << endl;
    pt1 = pt1 - pt2;
    cout << "pt1 = pt1 - pt2:\t" << pt1 << endl;
}

static void TestIntegralArrays (void)
{
    TestArray<float,4> ("array<float,4>");
    TestArray<float,2> ("array<float,2>");
    TestArray<int32_t,4> ("array<int32_t,4>");
    TestArray<uint32_t,4> ("array<uint32_t,4>");
    TestArray<int32_t,2> ("array<int32_t,2>");
    TestArray<uint32_t,2> ("array<uint32_t,2>");
    TestArray<int16_t,4> ("array<int16_t,4>");
    TestArray<uint16_t,4> ("array<uint16_t,4>");
    TestArray<int8_t,8> ("array<int8_t,8>");
    TestArray<uint8_t,8> ("array<uint8_t,8>");

    cout << "================================================" << endl;
    cout << "Testing array<string,3>" << endl;
    cout << "================================================" << endl;
    array<string,3> strv;
    strv[0] = "str0";
    strv[1] = "str1";
    strv[2] = "str2";
    cout << "str: " << strv << endl;
}

StdTestMain (TestIntegralArrays)
