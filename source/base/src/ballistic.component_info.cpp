#include "ballistic.component_info.h"
#include "ballistic.component_constructor.h"
#include "ballistic.debug.h"
#include "ballistic.icomponent.h"

namespace ballistic {

	component_info::component_info () : _ctor (nullptr) {}
	
	icomponent_constructor * component_info::ctor () {
		return _ctor;
	}
		
	void component_info::ctor( icomponent_constructor * ctor_v ) {
		_ctor = ctor_v;
	}
		
	icomponent * component_info::create (entity * parent) {
		if (!_ctor) {
			debug_error ("component constructor not set. cannot create component instance.");
			return nullptr;
		}
			
		icomponent * new_component = nullptr;
		
		if (properties.count ())
			new_component = _ctor->create (parent, properties);
		else
			new_component = _ctor->create (parent);
			
		return new_component;
	}
		
}