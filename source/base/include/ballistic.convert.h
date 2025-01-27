#pragma once
#ifndef _ballistic_convert_h_
#define _ballistic_convert_h_

#include "ballistic.debug.h"

#include <string>
#include <sstream>
#include <type_traits>

#pragma warning (disable : 4800 )

namespace ballistic {

#	define MSG_NOT_FUNDAMENTAL_SRC	"non fundamental source type for conversion"
#	define MSG_NOT_FUNDAMENTAL_DST	"non fundamental destination type for conversion"
#	define MSG_STRING_CONV_FAILED	"conversion from string failed"

	inline istream & convert_split (istream & stream, string & trim, char delimiter) {
		char c;

		trim.clear ();

		while (stream.get (c).good ()) {
			if (c == '\n' || c == '\r')
				continue;

			if (c == delimiter)
				return stream;

			trim.append (1, c);

			if (stream.peek () == std::char_traits < char>::eof ())
				return stream;
		}

		return stream;
	}

	//template < class src_t, class dst_t > 
	//inline void convert ( const src_t & src, dst_t & dst ){
	//	// Compile time type conversion errors
	//	static_assert (std::is_fundamental < src_t >::value, MSG_NOT_FUNDAMENTAL_SRC);
	//	static_assert (std::is_fundamental < dst_t >::value, MSG_NOT_FUNDAMENTAL_DST);
	//
	//	dst = (dst_t)src;
	//}

	// Same type
	template < class t >
	inline void convert ( const t & src, t & dst ) {
		dst = src;
	}

	// Support for strings
	template < class string_t, class dst_t > 
	inline void __convert_string ( const string_t & src, dst_t & dst ) {
		// Compile time type conversion errors
		static_assert (std::is_fundamental < dst_t >::value, MSG_NOT_FUNDAMENTAL_DST);

		std::stringstream stream (src);

		//extract whitespaces
		stream >> std::ws >> dst;
		if (stream.fail ()) {
			debug_error (MSG_STRING_CONV_FAILED);
			dst = dst_t ();
		}
	}

	inline void convert (string src, bool & dst) {
		if (src == "true")
			dst = true;
		else if (src == "false")
			dst = false;
		else
			debug_error ("cannot convert " << src << " to boolean.");
	}

	inline void convert (const char * src, bool & dst) {
		convert (string (src), dst);
	}

	template < class dst_t > 
	inline void convert ( const char * src, dst_t & dst ) {
		__convert_string < const char *, dst_t > ( src, dst );
	}

	template  < class dst_t >
	inline void convert ( std::string & src, dst_t & dst ) {
		__convert_string < std::string, dst_t > ( src, dst );
	}

	template < class dst_t >
	inline void convert ( const std::string & src, dst_t & dst ) {
		__convert_string < std::string, dst_t > ( src, dst );
	}

	template < class src_t >
	inline void convert ( const src_t & src, std::string & dst ) {
		// Compile time type conversion errors
		static_assert ( std::is_fundamental < src_t >::value, MSG_NOT_FUNDAMENTAL_SRC );
		dst = std::to_string (src);
	}

	inline void convert ( std::string & src, std::string & dst ) {
		dst = src;
	}

	template < class data_t, size_t length>
	inline bool convert_vectors (const std::string & src, data_t dst [length]) {
		stringstream src_stream (src);
		string trim;

		for (uint32_t i = 0; i < length; ++i) {
			if (!convert_split (src_stream, trim, ','))
				return false;

			convert (trim, dst [i]);
		}

		return true;
	}

	template < class t >
	inline void convert_vectors (const t & src, std::string & dst) {
		stringstream stream;

		stream << src.data [0];
		for (uint32_t i = 1; i < t::size; ++i) {
			stream << ',' << src.data [i];
		}
	}

	template < class dst_t, class src_t >
	inline dst_t convert_to ( const src_t & src ) {
		dst_t tmp_value;
		convert (src, tmp_value);
		return tmp_value;
	}

}

#endif