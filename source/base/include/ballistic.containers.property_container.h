#ifndef	_ballistic_property_container_h_
#define _ballistic_property_container_h_

#include "ballistic.property.h"
#include "ballistic.containers.icontainer.h"

#include <map>

namespace ballistic {
	namespace containers {

		template < class base_property_t = iproperty >
		class iproperty_container : public icontainer < std::map < id_t, base_property_t * > > {
		public:
			
			typedef icontainer < std::map < id_t, base_property_t * > > base_t;

			inline virtual ~iproperty_container () {
				for (auto & pair : base_t::data) {
					if (pair.second)
						delete pair.second;
				}
			}

			inline void remove (const id_t & id) {
				auto it = base_t::data.find (id);

				if (it != base_t::data.end ()) {
					delete it->second;
					base_t::data.erase (it);
				}
			}

			inline bool contains (const id_t & id) const {
				return base_t::data.find (id) != base_t::data.end ();
			}

			inline base_property_t * find (const id_t & id) const {
				auto it = base_t::data.find (id);

				if (it != base_t::data.end ())
					return it->second;
				else
					return nullptr;
			}

			inline void insert (base_property_t * p) {
				// TODO: can cause a memory leak
				base_t::data [p->id ()] = p;
			}

			inline virtual void copy_to (iproperty_container < base_property_t > & dest) const {
				for (auto it : base_t::data) {

					auto new_item = it.second->clone ();
					auto typed_item = dynamic_cast <base_property_t *> (new_item);

					if (typed_item)
						dest.insert (typed_item);
					else {
						debug_print ("cloned property_container " << new_item->id () << " cannot be converted to base property type");
						delete new_item;
					}
				}
			}

		};

		template <
			class base_property_t,
			template < class > class default_property_t
		>
		class base_property_container : public iproperty_container < base_property_t > {
		public:

			inline base_property_container () {}
		
			// delete copy and assignment
			base_property_container (const base_property_container < iproperty, default_property_t > & o) = delete;
			base_property_container & operator = (const base_property_container < iproperty, default_property_t > & o) = delete;

			template < class value_t >
			inline default_property_t < value_t > * require (const id_t & id, const value_t & default_value ) {
				auto p = this->find (id);

				if (!p) {
					auto new_p = new default_property_t < value_t > (id, default_value );
					this->insert (new_p);
					return new_p;
				} else {
					auto typed_p = dynamic_cast <default_property_t < value_t > *> (p);

					if (!typed_p) {
						debug_print ("property " << id << " already exists with different type. property overriden.");
						delete p;

						typed_p = new default_property_t < value_t > (id, default_value );
						this->insert (typed_p);
					}

					return typed_p;
				}
			}

			template < class value_t >
			inline default_property_t < value_t > * require (const id_t & id) {
				return require < value_t > (id, value_t ());
			}

			template < class value_t >
			inline default_property_t < value_t > * aquire (const id_t & id) {
				auto p = this->find (id);

				if (!p) {
					debug_print ("no property with id \"" << id << "\" defined");
					return new default_property_t < value_t > (id, value_t ()); // null property
				} else {
					auto typed_p = dynamic_cast <property < value_t > *> (p);

					if (!typed_p) {
						debug_print ("property " << id << " required with different type.");
						return new default_property_t < value_t > (id, value_t ()); // null property
					}

					return typed_p;
				}
			}

			inline details::property_accessor < base_property_t, default_property_t > operator [](id_t id) {
				auto p = this->find (id);

				if (!p) {
					debug_print ("property " << id << " not found!");
				}

				return details::property_accessor < base_property_t, default_property_t > (p);
			}

		};

		using property_container = base_property_container < iproperty, property >;

	}
}

#endif