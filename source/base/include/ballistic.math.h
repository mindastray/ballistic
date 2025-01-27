#ifndef	_ballistic_math_h_
#define _ballistic_math_h_

#include "ballistic.config.h"
#include "ballistic.math.vectors.h"

#include <cmath>

namespace ballistic {
	namespace math {

		real const pi = float (3.1415926535897932384626433832795);

		template < class T >
		inline T radians (const T & degrees) {
			return degrees * (pi / real (180));
		}

		template < class value_t, template < class > class struct_t >
		inline value_t dot ( const struct_t < value_t > & v1, const struct_t < value_t > & v2 ) {
			// avoid branching by avoiding zero ( floating point precision exploit )
			// returns minimum positive number for min
			value_t r = numeric_limits < value_t >::min ();

			for ( uint32_t i = 0; i < struct_t < value_t >::size; ++i)
				r += v1.data [i] * v2.data [i];
			
			return r;
		}

		template < class value_t, template < class > class struct_t >
		inline value_t sqr_length ( const struct_t < value_t > & v ) {
			return dot < value_t, struct_t > (v, v);
		}

		template < class value_t, template < class > class struct_t >
		inline value_t length (const struct_t < value_t > & v) {
			return std::sqrt (sqr_length < value_t, struct_t > (v));
		}

		template < class value_t, template < class > class struct_t >
		inline value_t length (const struct_t < value_t > & v1, const struct_t < value_t > & v2) {
			return length < value_t, struct_t >  ( v2 - v1 );
		}

		template < class value_t >
		inline details::vec3_t < value_t > cross (const details::vec3_t < value_t > & v1, const details::vec3_t < value_t > & v2) {
			return {
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
			};
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > normalize ( const struct_t < value_t > & v ) {
			value_t norm = value_t (1) / length < value_t, struct_t > ( v );
			return v * norm;
		}

		inline real lerp ( real v1, real v2, real alpha ) {
			return v1 + ((v2 - v1) * alpha);
		}

		template < class value_t, template < class > class struct_t >
		inline struct_t < value_t > lerp (const struct_t < value_t > & v1, const struct_t < value_t > & v2, real alpha) {
			struct_t < value_t > v;

			for (int i = 0; i < struct_t < value_t >::size; ++i)
				v.data [i] = lerp (v1.data [i], v2.data [i], alpha);

			return v;
		}

		template < class value_t, template < class > class struct_t >
		inline void lerp (const struct_t < value_t > & v1, const struct_t < value_t > & v2, real alpha, struct_t < value_t > & r) {
			for (int i = 0; i < struct_t < value_t >::size; ++i)
				r.data [i] = lerp (v1.data [i], v2.data [i], alpha);
		}

	};
}

#endif