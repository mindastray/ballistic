#include "ballistic.graphics.opengl_mesh.h"

#include <GL/glew.h>

#include "ballistic.graphics.opengl_debug.h"

namespace ballistic {
	namespace graphics {

		GLint opengl_mesh::calc_stride (mesh_attribute attributes) {
			GLint stride = 0;

			if (attributes & mesh_attribute_position)
				stride += sizeof (vec3);
			if (attributes & mesh_attribute_uv)
				stride += sizeof (vec2);
			if (attributes & mesh_attribute_normal)
				stride += sizeof (vec3);
			if (attributes & mesh_attribute_bone_index)
				stride += sizeof (uvec2);
			if (attributes & mesh_attribute_bone_weight)
				stride += sizeof (real);

			return stride;
		}

		aabox opengl_mesh::calc_aabox (uint8_t * data_buffer, uint32_t data_buffer_size, uint32_t offset) {
			aabox box = {0};

			for (uint32_t i = 0; i < data_buffer_size; i += offset) {
				vec3 & position = *(vec3 *)(data_buffer + i);
				box.wrap (position);
			}

			return box;
		}

		uint32_t opengl_mesh::set_attributes (mesh_attribute attributes) {
			//GLint attribute_id = 0;
			GLint attribute_offset = 0;
			GLint attribute_stride = calc_stride (attributes);

			if (attributes & mesh_attribute_position) {
				gl_eval_scope ([ballistic::graphics::opengl_mesh::set_attributes] mesh_attribute_position);
				attribute_offset = add_attribute (
					attribute_location_position,
					3,
					GL_FLOAT,
					attribute_stride,
					attribute_offset
					);
			}

			if (attributes & mesh_attribute_uv) {
				gl_eval_scope ([ballistic::graphics::opengl_mesh::set_attributes] mesh_attribute_uv);
				attribute_offset = add_attribute (
					attribute_location_uv,
					2,
					GL_FLOAT,
					attribute_stride,
					attribute_offset
					);
			}

			if (attributes & mesh_attribute_normal) {
				gl_eval_scope ([ballistic::graphics::opengl_mesh::set_attributes] mesh_attribute_normal);
				attribute_offset = add_attribute (
					attribute_location_normal,
					3,
					GL_FLOAT,
					attribute_stride,
					attribute_offset
					);
			}

			if (attributes & mesh_attribute_bone_index) {
				gl_eval_scope ([ballistic::graphics::opengl_mesh::set_attributes] mesh_attribute_bone_index);
				attribute_offset = add_attribute (
					attribute_location_bone_index,
					2,
					GL_UNSIGNED_INT,
					attribute_stride,
					attribute_offset
					);
			}

			if (attributes & mesh_attribute_bone_weight) {
				gl_eval_scope ([ballistic::graphics::opengl_mesh::set_attributes] mesh_attribute_bone_weight);
				attribute_offset = add_attribute (
					attribute_location_bone_weight,
					1,
					GL_FLOAT,
					attribute_stride,
					attribute_offset
					);
			}

			return attribute_offset;
		}

		GLint opengl_mesh::add_attribute (GLint id, GLint size, GLenum gl_type, GLint stride, GLint offset) {
			glVertexAttribPointer (
				id,
				size,         // number of elements
				gl_type,         // type
				GL_FALSE,     // normalized?
				stride,
				(GLvoid*)(offset) // array buffer offset in bytes
				);

			glEnableVertexAttribArray (id);
	
			return offset + size * 4; // 4 = size of float or int
		}
		
		opengl_mesh::opengl_mesh ( const id_t & id, uint8_t run_id ) :
			imesh (id),
			_run_id (run_id), 
			_vertex_array_id (0),
			_vertex_buffer_id(0),
			_index_buffer_id (0),
			_attribute_offset (0)
		{}
		
		opengl_mesh::~opengl_mesh() {
			if (_vertex_buffer_id > 0) {
				glDeleteBuffers (1, &_vertex_buffer_id);
				glDeleteBuffers (1, &_index_buffer_id);
			}
		}
		
		uint8_t opengl_mesh::run_id () { return _run_id; }

		void opengl_mesh::set_data (
			uint8_t *			data_buffer,
			int32_t			data_buffer_size,
			uint16_t *		index_buffer,
			uint32_t			index_buffer_size,
			mesh_attribute	attributes,
			bool			dynamic
		){
			_index_buffer_size = index_buffer_size;

			gl_eval_scope (opengl_mesh::set_data);

			// Create vertex buffer
			glGenBuffers (1, &_vertex_buffer_id);
			glBindBuffer (GL_ARRAY_BUFFER, _vertex_buffer_id);

			// Set vertex buffer data
			glBufferData (GL_ARRAY_BUFFER, data_buffer_size, (GLvoid *)data_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

			// Create index buffer
			glGenBuffers (1, &_index_buffer_id);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _index_buffer_id);
			
			// Set index buffer data
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, _index_buffer_size, (GLvoid *)index_buffer, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

			// Unbind buffers
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

			// Create Vertex Array Object
			glGenVertexArrays (1, &_vertex_array_id);
			glBindVertexArray (_vertex_array_id);

			// Setup vertex array object
			glBindBuffer (GL_ARRAY_BUFFER, _vertex_buffer_id);

			_attribute_offset = set_attributes (attributes);

			if (data_buffer)
				_bounding_box = calc_aabox (data_buffer, data_buffer_size, _attribute_offset);
			else
				_bounding_box = {};

			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _index_buffer_id);

			// Reset state machine
			glBindVertexArray (0);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void opengl_mesh::update_data (
			uint8_t *			data_buffer,
			int32_t			data_buffer_size
		){
			if (_vertex_array_id == 0) {
				debug_error ("gL cannot update data from undefined mesh");
				return;
			}

			gl_eval_scope (opengl_mesh::update_data);

			glBindVertexArray (_vertex_array_id);
			glBindBuffer (GL_ARRAY_BUFFER, _vertex_buffer_id);

			if (data_buffer_size > 0)
				glBufferSubData (
					GL_ARRAY_BUFFER,
					0,
					data_buffer_size,
					(GLvoid *)data_buffer
				);

			if (data_buffer)
				_bounding_box = calc_aabox (data_buffer, data_buffer_size, _attribute_offset);
			else
				_bounding_box = {};

			// Reset state machine
			glBindVertexArray (0);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void opengl_mesh::update_index (
			uint16_t *		index_buffer,
			uint32_t			index_buffer_size
		){
			if (_vertex_array_id == 0) {
				debug_error ("gl cannot update data from undefined mesh");
				return;
			}

			gl_eval_scope (opengl_mesh::update_index);

			_index_buffer_size = index_buffer_size;
			glBindVertexArray (_vertex_array_id);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, _index_buffer_id);

			if (_index_buffer_size > 0)
				glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, 0, _index_buffer_size, (GLvoid *)index_buffer);

			// Reset state machine
			glBindVertexArray (0);
			glBindBuffer (GL_ARRAY_BUFFER, 0);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		
		void opengl_mesh::apply (idevice * device) {
			gl_eval_scope (ballistic::graphics::opengl_mesh::apply);
			glBindVertexArray (_vertex_array_id);
		}
		
		void opengl_mesh::render () {
			gl_eval_scope (ballistic::graphics::opengl_mesh::render);
			glDrawElements (GL_TRIANGLES, _index_buffer_size, GL_UNSIGNED_SHORT, 0);
		}

		aabox opengl_mesh::bounding_box () const {
			return _bounding_box;
		}

	}
}