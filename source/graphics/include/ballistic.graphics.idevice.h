#ifndef	_ballistic_graphics_idevice_h_
#define _ballistic_graphics_idevice_h_

#include <ballistic.base.h>
#include <string>

#include "ballistic.graphics.imaterial.h"
#include "ballistic.graphics.imesh.h"
#include "ballistic.graphics.itexture.h"

using namespace std;

namespace ballistic {
	namespace graphics {

		class idevice {
		public:

			virtual imaterial * create_material () = 0;
			virtual imesh *		create_mesh () = 0;
			virtual itexture *	create_texture (const point & size) = 0;

			virtual imaterial * load_material ( const string & filename ) = 0;
			virtual imesh *		load_mesh ( const string & filename ) = 0;
			virtual itexture *	load_texture ( const string & filename ) = 0;
			
			virtual void		set_transform ( const mat4 & matrix ) = 0;

			virtual void		start_frame () = 0;
			virtual void		end_frame () = 0;

			virtual void		set_current_mesh ( imesh * mesh ) = 0;
			virtual void		set_current_texture ( itexture * texture ) = 0;
				
			virtual void		present () = 0;

			virtual void		destroy () = 0;
		};

	}
}

#endif