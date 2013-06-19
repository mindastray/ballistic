#ifndef _ballistic_graphics_system_component_h_
#define	_ballistic_graphics_system_component_h_

#include <ballistic.base.h>

#include "ballistic.graphics.idevice.h"

namespace ballistic {
	namespace graphics {

		class system_component : public ballistic::component {
		private:

			idevice * _device;

		public:

			system_component ( idevice * device );

			virtual void notify ( ballistic::message & message );
		};

	}
}

#endif