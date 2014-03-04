#ifndef	_ballistic_entity_type_h_
#define _ballistic_entity_type_h_

#include <functional>
#include <vector>
#include <string>

#include "ballistic.component_info.h"
#include "ballistic.id.h"
#include "ballistic.iresource.h"

#include <string>
#include <vector>

using namespace std;

namespace ballistic {
	
	class entity;
	class igame;
		
	class entity_type : public iresource {	
	public:

		entity_type (id_t id_v);

		vector < component_info > components;

		property_container properties;

		entity * create (id_t id);

	};
}


#endif