#ifndef _ballistic_graphics__ballistic_graphics_opengl_mesh_h_
#define _ballistic_graphics__ballistic_graphics_opengl_mesh_h_

#include <ballistic.base.h>
#include <ballistic.graphics.h>

namespace ballistic {
	namespace graphics {
		
		class opengl_mesh : public ballistic::graphics::imesh
		{
		private:
			
			uint32
				_vertex_buffer_id,
				_index_buffer_id,
				_index_buffer_size;
			
			
		public:
			
			opengl_mesh ();
			~opengl_mesh ();
			
			virtual void set_data (
				vertex vert_buffer [],
				uint32 vert_size,
				uint16 index_buffer [],
				uint32 index_size
			);
			
			virtual void attach ();
			virtual void render ();
			virtual void detach ();
			
		};
		
	}
}


#endif