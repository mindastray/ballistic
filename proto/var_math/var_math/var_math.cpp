// var_math.cpp : Defines the entry point for the console application.
//


#pragma warning (disable : 4996)

#include "stdafx.h"
#include <cinttypes>
#include <string>
#include <iostream>

#include <vector>

#include <smmintrin.h>

#include "calc_time.h"


namespace math {

	template < int index, int length >
	struct for_op {

		template < class oper_t >
		inline static void run (oper_t & op) {
			op (index);
			for_op < index + 1, length >::run (op);
		}

	};

	template < int length >
	struct for_op < length, length > {

		template < class oper_t >
		inline static void run (oper_t & op) {}

	};

	template < class value_t, class data_t >
	struct vecn_t : public data_t {

		typedef vecn_t < value_t, data_t > this_type;

		inline void swap (this_type & right) {
			std::swap (data, right.data);
		}

		inline void clear () {
			for (value_t & v : this->data)
				v = value_t (); // zero
		}

		inline vecn_t () {
			clear ();
		}
		
		inline this_type & operator = (this_type v) {
			swap (v);
			return *this;
		}

	};

	template < class value_t, class data_t >
	inline vecn_t < value_t, data_t > operator * (
		const vecn_t < value_t, data_t > & va,
		float vb) {
		vecn_t r;

		for_op < 0, data_t::size >::run (
			[&](int i) {r.data [i] = va.data [i] * vb; }
		);

		return r;
	}

	template < class value_t, class data_t, class this_type = vecn_t < value_t, data_t >>
	inline this_type operator * (
		const this_type & v1, 
		const this_type & v2) 
	{
		vecn_t < value_t, data_t > r;

		for_op < 0, data_t::size >::run (
			[&](int i) { r.data [i] = v1.data [i] * v2.data [i]; }
		);

		return r;
	}

	template < class value_t, class data_t >
	inline vecn_t < value_t, data_t > operator - (
		const vecn_t < value_t, data_t > & v1,
		const vecn_t < value_t, data_t > & v2) {
		vecn_t < value_t, data_t > r;

		for_op < 0, data_t::size >::run (
			[&](int i) { r.data [i] = v1.data [i] - v2.data [i]; }
		);

		return r;
	}

	template < class value_t >
	struct vec2_data {
		static const uint32_t size = 2;

		union {
			value_t data [size];
			struct {
				value_t x, y;
			};
		};
	};

	template < class value_t >
	using vec2_t = vecn_t < value_t, vec2_data < value_t > >;

	template< class value_t >
	struct vec4_data {

		static const uint32_t size = 4;

		union {
			value_t data [size];
			struct { value_t x, y, z, w; };
			struct { value_t r, g, b, a; };
		};
	};


	template < class value_t, class data_t, class this_type = vecn_t < value_t, data_t > >
	inline value_t sqr_length (const this_type & v) {
		return dot (v, v);
	}

	template < class value_t, class data_t, class this_type = vecn_t < value_t, data_t > >
	inline value_t sqr_length (const this_type & v1, const this_type & v2) {
		return sqr_lenght (v2 - v1);
	}

	template < class value_t, class data_t, class this_type = vecn_t < value_t, data_t > >
	inline value_t length (const this_type & v) {
		return sqrtf (sqr_length (v));
	}

	template < class value_t, class data_t, class this_type = vecn_t < value_t, data_t > >
	inline value_t length (const this_type & v1, const this_type & v2) {
		return sqrtf (sqr_length (v1, v2));
	}

}

struct A {

	union {
		struct {
			double data [3];
		};
		struct {
			double X;
			double Y;
			double Z;
		};
	};

	inline A () {}
	inline A (const std::initializer_list < double > & l) {
		std::copy (l.begin (), l.end (), +data);
	}

	inline A (A && v) { 
		std::copy (std::begin (v.data), std::end (v.data), +data);
	}

	inline A operator * (A v) {
		A tmp;

		tmp.X = X * v.X;
		tmp.Y = Y * v.Y;
		tmp.Z = Z * v.Z;

		return tmp;
	}

	inline A & operator = (A && v) {
		X = v.X;
		Y = v.Y;
		Z = v.Z;

		return *this;
	}

};

struct B {

	double X;
	double Y;
	double Z;

	inline B () {}
	inline B (std::initializer_list < double > args) {
		const double * V = args.begin ();

		X = V [0];
		Y = V [1];
		Z = V [2];
	}

	inline B operator * (B v) const {
		B tmp;

		tmp.X = X * v.X;
		tmp.Y = Y * v.Y;
		tmp.Z = Z * v.Z;

		return tmp;
	}

	inline B & operator *= (const B & v) {
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;

		return *this;
	}

};

#define MAX_TURNS 1000000

A calculate_A () {

	A x = {13.0, 12.0, 11.0}, y = {13.0, 12.0, 11.0}, z = {13.0, 12.0, 11.0};

	z = x;

	for (int i = 0; i < MAX_TURNS; ++i) {
		z = x * y;
	}

	return z;

}

void test_a () { 
	A v = calculate_A ();
	std::cout << v.X;
}

B calculate_B () {

	B x = {13.0, 12.0, 11.0}, y = {13.0, 12.0, 11.0}, z = {13.0, 12.0, 11.0};

	for (int i = 0; i < MAX_TURNS; ++i) {
		z = x * y;
	}

	return z;

}

void test_b () {
	B v = calculate_B ();
	std::cout << v.X;
}

int _tmain(int argc, _TCHAR* argv[])
{

	double seconds = diag::timed_avg_call (100, test_a) ();
	std::cout << std::endl << "Move multiply and copy average: " << seconds << std::endl;

	seconds = diag::timed_avg_call (100, test_b) ();
	std::cout << std::endl << "Normal multiply and copy average: " << seconds;

	std::cin >> seconds;

	return 0;
}
