#include "ballistic.graphics.mesh.h"

#include "ballistic.graphics.defines.h"

namespace ballistic {
	namespace graphics {

		mesh::mesh ()
			: _v_buffer_id (0), _i_buffer_id (0), _face_count(0)
		{}

		mesh::mesh ( vertex v_buffer [], uint16 index_buffer [] ) 
			: _v_buffer_id (0), _i_buffer_id (0), _face_count (0)
		{
			set_data (v_buffer, index_buffer);
		}

		mesh::~mesh() {
			clear_data ();
		}

		void mesh::set_data ( vertex v_buffer [], uint16 index_buffer [] ) {
			clear_data ();
			//Copy vertex buffers
			glGenBuffers (1, (GLuint *)&_v_buffer_id);

			glBindBuffer (GL_ARRAY_BUFFER, _v_buffer_id );
			glBufferData (GL_ARRAY_BUFFER, sizeof (v_buffer), &v_buffer [0].position[0], GL_STATIC_DRAW);

			//Copy index buffers
			glGenBuffers (1, (GLuint *)&_i_buffer_id);

			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _i_buffer_id );
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (index_buffer), &index_buffer [0], GL_STATIC_DRAW);

			_face_count = (sizeof ( index_buffer ) / sizeof ( uint16 ) / 3);
		}

		void mesh::clear_data () {
			if (!_v_buffer_id)
				return;

			glDeleteBuffers (1, &_v_buffer_id);
			glDeleteBuffers (1, &_i_buffer_id);

			_v_buffer_id = _i_buffer_id = 0;
		}

		void mesh::render () {
			// Set client state --------------------------
			glBindBuffer (GL_ARRAY_BUFFER, _v_buffer_id);

			glEnableClientState (GL_VERTEX_ARRAY);
			glVertexPointer (3, GL_FLOAT, sizeof (vertex), (GLvoid *)0);
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer (2, GL_FLOAT, sizeof (vertex), (GLvoid *)12);
			
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _i_buffer_id);
			// -------------------------------------------
			glDrawElements (GL_TRIANGLES, _face_count, GL_UNSIGNED_SHORT, 0);

			// Delete client state -----------------------
			glDisableClientState (GL_VERTEX_ARRAY);
			glDisableClientState (GL_TEXTURE_COORD_ARRAY);
			// -------------------------------------------

			// if we ever group states to optimize state management
			// remove these two from the render method
		}

	}
}