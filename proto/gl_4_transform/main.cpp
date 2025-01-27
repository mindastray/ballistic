
#include <GL/glew.h>
#include <glm.hpp>
#include <ext.hpp>

#include <iostream>

#include "debug.h"
#include "ifrontend.h"
#include "win_frontend.h"
#include "mac_frontend.h"

#include "math.h"
#include "matrix.h"

static const GLfloat vertex_data [] =
{
	-1.0F, -1.0F, .0F,
	1.0F, -1.0F, .0F,
	0.0F, 1.0F, .0F
};

GLuint	vertex_buffer_obj_id,
		vertex_buffer_id,
		vshader_id,
		pshader_id,
		program_id;

const char vshader_src [] = {
	gl_shader_source (	// ----------------------------

	layout (location = 0) in vec3 vertexPosition_modelspace;
	
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;

	void main () {
	
		mat4 mvp = proj * view * model;
		
		gl_Position = mvp * vec4 (vertexPosition_modelspace, 1.);
	}

	)}; //----------------------------------------------

const char pshader_src [] = {
	gl_shader_source (// ------------------------------

	out vec3 color;

	void main () {
		color = vec3 (1, 0, 0);
	}

	)}; // ---------------------------------------------

void load () {

	glDisable (GL_CULL_FACE);

	// shaders
	{
		gl_eval_scope ("shader id creation");

		vshader_id = glCreateShader (GL_VERTEX_SHADER);
		pshader_id = glCreateShader (GL_FRAGMENT_SHADER);
	}

	{
		gl_eval_scope ("vertex shader creation");

		const char * p_vshader_src = +vshader_src;

		glShaderSource (vshader_id, 1, &p_vshader_src, NULL);
		glCompileShader (vshader_id);

		gl_eval_shader_compile (vshader_id);
}

	{
		gl_eval_scope ("fragment shader creation");

		const char * p_pshader_src = +pshader_src;

		glShaderSource (pshader_id, 1, &p_pshader_src, NULL);
		glCompileShader (pshader_id);

		gl_eval_shader_compile (pshader_id);
	}

	{
		gl_eval_scope ("program linking");

		program_id = glCreateProgram ();
		glAttachShader (program_id, vshader_id);
		glAttachShader (program_id, pshader_id);

		glLinkProgram (program_id);

		gl_eval_program_link (program_id);
	}

	{
		gl_eval_scope ("shader clean up");

		glDeleteShader (vshader_id);
		glDeleteShader (pshader_id);
	}

	// geometry 
	{
		gl_eval_scope ("load geometry");
		
		glGenVertexArrays (1, &vertex_buffer_obj_id);
		glBindVertexArray (vertex_buffer_obj_id);
		
		glGenBuffers (1, &vertex_buffer_id);
		glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer_id);

		glBufferData (GL_ARRAY_BUFFER, sizeof (vertex_data), vertex_data, GL_STATIC_DRAW);
	}
}

float _x = .0F;
float _inc = .0001F;

float angle = 0.0;
float radius = 5.0;

vec3 camPos;

void frame () {

	// animations

	_x += _inc;

	if (_x < -1. || _x > 1)
		_inc *= -1;

	matrix model = matrix::make_translation (_x, .0F, .0F);

	camPos.x = cos (angle) * radius;
	camPos.y = 0.;
	camPos.z = sin (angle) * radius;
	
	angle += 0.001F;
	radius += 0.001;
	
	if (angle > (3.1415 * 2.0)) {
		angle = 0.0;
		radius = 5.0;
	}

	//camPos = vec3 (4, 4, 3);

	// camera projections and stuffs

	float const pi = float (3.1415926535897932384626433832795);

	float
		fov = 45. * (pi / 180.0),
		a = 1.,
		n = 0.1,
		f = 100.0;

	float
		range = tan (fov / 2.) * n,
		l = -range * a,
		r = -l,
		b = -range,
		t = range;

	matrix proj (
		(2. * n) / (r - l), .0, .0, .0,
		.0, (2. * n) / (t - b), .0, .0,
		.0, .0, -(f + n) / (f - n), -1.,
		.0, .0, -(2. * f * n) / (f - n), .0
		);

	// calculate view

	vec3 zaxis = normalize (vec3() - camPos);
	vec3 yaxis = normalize (vec3 (0, 1., 0));
	vec3 xaxis = normalize (cross (zaxis, yaxis));
	yaxis = cross (xaxis, zaxis);

	matrix view (
					xaxis.x, yaxis.x, -zaxis.x, .0,
					xaxis.y, yaxis.y, -zaxis.y, .0,
					xaxis.z, yaxis.z, -zaxis.z, .0,
					-dot (xaxis, camPos), -dot (yaxis, camPos), dot (zaxis, camPos), 1.
				 );

	// clear frame
	{
		gl_eval_scope ("clear buffers");

		glClearColor (.0F, .6F, 1.F, 1.F);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	{
		gl_eval_scope ("enable shader");

		glUseProgram (program_id);
		
		GLuint u_model_m_id = glGetUniformLocation (program_id, "model");
		glUniformMatrix4fv (u_model_m_id, 1, GL_FALSE, (const float *)&model);
		
		GLuint u_proj_m_id = glGetUniformLocation (program_id, "proj");
		glUniformMatrix4fv (u_proj_m_id, 1, GL_FALSE, (const float *)&proj);
		
		GLuint u_view_m_id = glGetUniformLocation (program_id, "view");
		glUniformMatrix4fv (u_view_m_id, 1, GL_FALSE, (const float *)&view);
	}

	{
		gl_eval_scope ("bind geometry");

		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, vertex_buffer_id);

		glVertexAttribPointer (
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);
	}

	{
		gl_eval_scope ("draw geometry");

		glDrawArrays (GL_TRIANGLES, 0, 3);
	}

	glDisableVertexAttribArray (0);

}

int main () {

	ifrontend * window = new frontend (400, 400);
	window->create ();
	window->show ();

	glewExperimental = true;
	if (glewInit () != GLEW_OK) {
		std::cerr << "failed to initialize glew";
		return 0;
	}

	glGetError (); //reset errors
	auto version = (const char *)glGetString (GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl << std::endl;

	load ();

	window->do_event_loop (frame);
	
	return 0;
}