#include "ballistic.graphics.opengl_debug.h"
#include "ballistic.graphics.opengl_effect.h"

namespace ballistic {
	namespace graphics {

		string opengl_debug::error_to_string (GLenum error) {
#			define auto_case(x) \
				case x: \
					return #x;

			switch (error) {
				auto_case (GL_INVALID_ENUM)
				auto_case (GL_INVALID_VALUE)
				auto_case (GL_INVALID_OPERATION)
				auto_case (GL_INVALID_FRAMEBUFFER_OPERATION)
				auto_case (GL_OUT_OF_MEMORY)
				auto_case (GL_STACK_UNDERFLOW)
				auto_case (GL_STACK_OVERFLOW)
				auto_case (GL_NO_ERROR)
			default:
				return "Unknown error code!";
			};

#			undef auto_case
		}

		void opengl_debug::eval_shader_compile (GLint shader_id) {

			GLint compiler_state;
			glGetShaderiv (shader_id, GL_COMPILE_STATUS, &compiler_state);
			
			if (compiler_state == GL_FALSE) {
				int32_t
					info_length,
					writen_chars;
					
				char * info_log;
			
				glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_length);
				info_log = new char [info_length];
				
				glGetShaderInfoLog (shader_id, info_length, &writen_chars, info_log);
				
				string error (info_log, writen_chars);
				debug_error("GL Shader " << shader_id << " compile failed with error: " << error);
				
				delete [] info_log;
			}
		}

		void opengl_debug::eval_program_link (GLint program_id) {
			GLint link_state;
			glGetProgramiv (program_id, GL_LINK_STATUS, &link_state);
			if (link_state == GL_FALSE) {
				int32_t
					info_length,
					writen_chars;
					
				char * info_log;
			
				glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_length);
				info_log = new char [info_length];
				
				glGetProgramInfoLog (program_id, info_length, &writen_chars, info_log);
				
				string error (info_log, writen_chars);
				debug_error("GL Program " << program_id << " link failed with error: " << error);
				
				delete [] info_log;
			}
		}

		opengl_debug::eval_scope::~eval_scope () {
			GLenum error = glGetError ();
			if (error != GL_NO_ERROR)
				debug_error ("GL Scope [" << method_name << "] Failed in file " << __FILE__ << " with " << ballistic::graphics::opengl_debug::error_to_string (error) );
		}

		// ------------------------------------------------

		const char * opengl_debug::_shader_source = 
		gl_vshader_source (
			layout (location = 0) in vec3	in_position;

			void main () {
				gl_Position = vec4 (
					in_position,
					1.
				);
			}
		)
		gl_fshader_source (
			uniform vec4					in_color;
			out vec4 out_color;

			void main () {
				out_color = in_color;
			}
		);

		GLuint opengl_debug::_vao_id_line = 0;
		GLuint opengl_debug::_vbo_id_line = 0;
		GLuint opengl_debug::_vao_id_quad = 0;
		GLuint opengl_debug::_vbo_id_quad = 0;
		
		graphics::opengl_effect		opengl_debug::_effect (0, 0);
		graphics::iconstant *	opengl_debug::_color_uniform;

		void opengl_debug::create_vertex_array (GLuint & vao_id, GLuint & vbo_id, uint32_t size_in_bytes) {
			glGenBuffers (1, &vbo_id);
			glBindBuffer (GL_ARRAY_BUFFER, vbo_id);

			// set buffer data
			glBufferData (GL_ARRAY_BUFFER, size_in_bytes, NULL, GL_DYNAMIC_DRAW);

			glGenVertexArrays (1, &vao_id);
			glBindVertexArray (vao_id);

			glBindBuffer (GL_ARRAY_BUFFER, vbo_id);

			glVertexAttribPointer (
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(GLvoid *)0
				);

			glEnableVertexAttribArray (0);
		}

		void opengl_debug::initialize () {
			gl_eval_scope(opengl_debug::initialize);
			
			_effect.load (_shader_source, strlen (_shader_source));
			_color_uniform = _effect.constant(text_to_id("in_color"));

			// --- create vector array
			create_vertex_array (_vao_id_line, _vbo_id_line, 4 * 3 * 2);
			create_vertex_array (_vao_id_quad, _vbo_id_quad, 4 * 3 * 8);

			// clear binding
			glBindVertexArray (0);

		}

		void opengl_debug::draw_line (const vec3 & p1, const vec3 & p2, const color & col ) {
			gl_eval_scope (opengl_debug::draw_line);

			_effect.apply (nullptr);
			_color_uniform->set_value (col);

			vec3 buffer [2] = { p1, p2 };

			glBindVertexArray (_vao_id_line);
			glBindBuffer (GL_ARRAY_BUFFER, _vbo_id_line);
			
			glBufferSubData (
				GL_ARRAY_BUFFER,
				0,
				4 * 3 * 2,
				(GLvoid *)+buffer
				);

			glDrawArrays (GL_LINES, 0, 2);

			glBindVertexArray (0);
		}

		void opengl_debug::draw_rect (const vec3 & p1, const vec3 & p2, const color & col ) {
			gl_eval_scope (opengl_debug::draw_rect);

			_effect.apply (nullptr);
			_color_uniform->set_value(col);

			vec3 uc = {p2.x, p1.y, p1.z};
			vec3 bc = {p1.x, p2.y, p2.z};

			vec3 buffer [8] = { 
				p1, uc,
				uc, p2,
				bc, p2,
				p1, bc
			};

			glBindVertexArray (_vao_id_quad);
			glBindBuffer (GL_ARRAY_BUFFER, _vbo_id_quad);
			glBufferSubData (
				GL_ARRAY_BUFFER,
				0,
				4 * 3 * 8,
				(GLvoid *)+buffer
			);

			glDrawArrays (GL_LINES, 0, 8);

			glBindVertexArray (0);
		}

		void opengl_debug::draw_text (const vec3 & pos, const color & col, const char message []) {

		}

	}
}