
#ifndef _ballistic_property_h_
#define _ballistic_property_h_

#include "ballistic.config.h"
#include "ballistic.debug.h"
#include "ballistic.id.h"

namespace ballistic {

	class entity;

	class iproperty {
	protected:

		id_t							_id = 0;
		entity *						_container;

	public:

		id_t							id () const;
		entity *						container () const;

		iproperty ();

		virtual							~iproperty ();
		void							raise_event () const;
	};

	template < class value_t >
	class property : public iproperty {
	private:		
		value_t					_value;
	public:

		inline property (const id_t & id_v, const value_t & v, iproperty_container * ctner)
			: _id (id_v), _value (v), _container (ctner)
		{}

		inline void operator = (const value_t & v) {
			_value = v;
			iproperty::raise_event ();
		}

		inline operator value_t () const {
			return _value;
		}

	};

	namespace details {

		struct property_accessor {

			id_t			id;
			iproperty *		prop;

			inline property_accessor (const id_t & id_v, iproperty * p) : id (id_v), prop (p) {}

			template < class value_t >
			inline void operator = (const value_t & v) {
#ifdef BALLISTIC_DEBUG
				auto p = dynamic_cast <property < value_t > *> (prop);
#else
				auto p = static_cast <property < value_t > *> (prop);
#endif
				if (p) {
					*p = v;
				} else {
					debug_run (
						if (prop) {
							debug_print ("[base::property_accessor::assign] wrong type property assign for" << id);
						} else {
							debug_print ("[base::property_accessor::assign] property " << id << " not found");
						}
					);
				}
			}

			template < class value_t >
			inline operator value_t () const {
#ifdef BALLISTIC_DEBUG
				auto p = dynamic_cast <property < value_t > *> (prop);
#else
				auto p = static_cast <property < value_t, > *> (prop);
#endif
				if (p)
					return *p;
				else {
					debug_run (
						if (prop) {
							debug_print ("[base::property_accessor::access] wrong type property access for" << id);
						} else {
							debug_print ("[base::property_accessor::access] property " << id << " not found");
						}
					);
					return value_t ();
				}
			}
		};
	}
}

#endif
