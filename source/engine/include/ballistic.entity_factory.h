#ifndef _ballistic_ballistic_entity_factory_h_
#define _ballistic_ballistic_entity_factory_h_

#include <atomic>
#include <functional>
#include <string>

#include "ballistic.entity.h"
#include "ballistic.entity_definition.h"

using namespace std;

namespace ballistic {
		
	typedef size_t entity_type_t;

	class entity_factory {
	public:
			
		typedef map < entity_type_t, entity_definition * > entity_definition_map;
			
	private:
			
		static atomic < uint32 > _current_id;
		static entity_definition_map _definitions;
			
	public:
			
		static entity_definition & define (const string & name);
			
		static entity * create (const string & type);
		static entity * create (entity_type_t type);
			
	};
		
}

#endif
