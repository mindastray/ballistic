#ifndef	_ballistic_text_h_
#define _ballistic_text_h_

#include "ballistic.config.h"
#include "ballistic.debug.h"

#include <memory>
#include <string>

namespace ballistic {

	struct text {
		static const uint32_t max_length = sizeof (real)* 16 - (sizeof (uint32_t) + 1); // matrix4 size - length - zero termination

		union {
			char data [sizeof (real) * 16];
			struct {
				char data [max_length + 1];
				uint32_t length;
			} str;
		};

		inline text () { data [0] = '\0'; str.length = 0; }

		inline text (text && v) : str (v.str) {}

		inline text & operator = (text && v) {
			str = v.str;
			return *this;
		}

		inline bool operator == (const text & v) {
			return std::strcmp (+data, +v.data) != 0;
		}

		inline bool operator == (const char * v) {
			return std::strcmp (+data, v) != 0;
		}

		inline bool operator != (const text & v) {
			return !this->operator== (v);
		}

		inline bool operator != (const char * v) {
			return !this->operator==(v);
		}

		// convert from const char *
		inline text (const char * v) {
			str.length = std::strlen (v);

			if (str.length > max_length) {
				str.length = max_length;
				debug_print ("[ballistic::text::char_ptr_ctor] text length is unsupported. Text will be trimmed.");
			}

			std::copy (v, v + str.length, +str.data);
			str.data [str.length]= '\0';
		}

		// convert from string
		inline text (const std::string & v) {
			str.length = v.length ();

			if (str.length > max_length) {
				str.length = max_length;
				debug_print ("[ballistic::text::string_ctor] text length is unsupported. Text will be trimmed.");
			}

			std::copy (v.begin (), v.end (), +str.data);
			str.data [str.length] = '\0';
		}

		inline operator const char * () const {
			return +data;
		}

		inline operator std::string () const {
			return std::string (+str.data, str.length);
		}

	};

}

#endif