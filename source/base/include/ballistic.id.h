#ifndef _ballistic_ballistic_id_h_
#define _ballistic_ballistic_id_h_

#include "ballistic.config.h"

#include <functional>
#include <string>

using namespace std;

namespace ballistic {
	
#ifndef BALLISTIC_DEBUG
	typedef size_t id_t;
	
	inline id_t text_to_id ( const char * id_str ) {
		return hash < string > () ( id_str );
	}
#else

	struct id_t {
		size_t key;
		string text;

		inline static void swap (id_t & v1, id_t & v2) {
			size_t t_key = v1.key;

			v1.key = v2.key;
			v2.key = t_key;

			v1.text.swap (v2.text);
		}

		inline id_t () : key (0) {}

		inline id_t (const size_t & key_v) : key (key_v) {}

		inline id_t (const size_t & key_v, const string & text_v) : key (key_v), text (text_v) {}

		inline id_t (const id_t & v) : key (v.key), text (v.text) {}

		inline id_t (id_t && v) {
			swap (*this, v);
		}

		inline id_t & operator = (id_t v) {
			swap (*this, v);
			return *this;
		}

		inline bool operator == (const id_t & v ) const {
			return key == v.key;
		}

		inline bool operator == (const uint32_t & v) const {
			return key == v;
		}

		inline bool operator < (const id_t & v) const {
			return key < v.key;
		}

		inline operator size_t () const {
			return key;
		}

	};

	inline id_t text_to_id (const char * id_str) {
		return id_t (hash < string > ()(id_str), id_str);
	}

#endif
}

#endif
