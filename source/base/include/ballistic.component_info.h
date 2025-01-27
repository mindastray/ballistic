
#ifndef _ballistic_component_info_h_
#define _ballistic_component_info_h_

#include "ballistic.config.h"
#include "ballistic.debug.h"

#include "ballistic.containers.property_container.h"


namespace ballistic {

	class entity;
	class icomponent;
	class icomponent_constructor;

	class component_info {
	private:
		icomponent_constructor * _ctor;
	public:

		component_info ();
		component_info (const component_info & o);

		icomponent_constructor * ctor ();
		void ctor (icomponent_constructor * ctor_v);

		containers::property_container properties;

		icomponent * create (entity * parent);
	};

}

#endif
