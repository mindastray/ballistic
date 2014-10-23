// transforms
uniform vec3		effect_t_eye;
uniform mat4        effect_t_model;
uniform mat4        effect_t_view;
uniform mat4        effect_t_proj;
uniform mat4        effect_t_normal;
uniform mat4        effect_t_mvp;

// Lights
/*
	mat [0].xyz		= position
	mat [0].w		= fallout
	mat [1].xyz		= color
	mat [1].w		= intensity
*/
uniform mat2x4		effect_lights [8];
uniform int		effect_light_count;

// material
uniform vec4        effect_diffuse;
uniform vec4		effect_specular;
uniform float		effect_specular_intensity;

uniform vec4		effect_ambient;

#ifdef VERTEX_SHADER

layout (location = 1) in vec3 in_position;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in vec3 in_normal;

out vec3			var_position;
out vec3            var_normal;
out vec2            var_uv;

void main () {

	vec4 pos = effect_t_mvp * vec4 (in_position, 1.0);

	var_position = (effect_t_model * vec4 (in_position, 1.0)).xyz;
	
	var_normal = ((effect_t_normal * vec4 (in_normal, 1.0)).xyz);

	var_uv = in_uv;
	// opengl invert texture
	var_uv.y = 1.0 - var_uv.y;

	gl_Position = pos;
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2D   effect_texture;

in vec3	var_position;
in vec3 var_normal;
in vec2 var_uv;
out vec4 out_color;

void main () {

	vec3
		light_pos,
		light_vec,

		light_col,
		skycol = vec3 (0.76, 0.83, 0.89),
		gndcol = vec3 (0.07, 0.11, 0.13),
		diff_col = vec3 (0.23, 0.14, 0.07),//effect_diffuse,
		spec_col = vec3 (1.0, 1.0, 1.0), //effect_specular;
		temp_col = vec3 (.0, .0, .0);

	float
		ambient_int = 0.05,
		light_fal,
		light_dst,
		light_int,
		spec_hard = 100.0,// effect_specular_hardness,
		spec_int = 0.8, // effect_specular_int
		att = 0.0,
		bgt = 0.0,
		sky_bgt = 0.0;

	temp_col = diff_col;

	vec3 normal = normalize (var_normal);

	// --- sky color ---
	vec3 up = vec3 (.0, 1.0, .0);
	sky_bgt = 0.5 * (1.0 + dot (up, normal));
	
	temp_col *= (sky_bgt * skycol + (1.0 - sky_bgt) * gndcol);
	
	// --- lights ---
	for (int i = 0; i < effect_light_count; ++i) {
		light_pos = effect_lights [i] [0].xyz;
		light_fal = effect_lights [i] [0].w;
		light_col = effect_lights [i] [1].xyz;
		light_int = effect_lights [i] [1].w;
	
		// --- attenuation ---
		// distance and normalize in one sqrt
		light_vec = light_pos - var_position;
		light_dst = max (length (light_vec), 0.0);
		light_vec = light_vec / light_dst;
	
		vec3 normal = normalize (var_normal);
		bgt = max (dot (normal, light_vec), 0.0);
	
		att = 0;
	
		if (light_dst < light_fal) {
			att = light_dst / light_fal + 1;
			att = 1.0 / (att * att);
		}
	
		light_col =  bgt * att * light_col * diff_col;
	
		// --- specular ---
		if (spec_int > 0.0) {
		
			vec3 eye_v = normalize (effect_t_eye - var_position);
		
			float spec_bgt = dot (normal, light_vec);
		
			if (spec_bgt > 0.0) {
				spec_bgt = pow (max (0.0, dot (eye_v, reflect (-light_vec, normal))), spec_hard), 1.0;
				light_col += spec_bgt * att * effect_lights [i] [1].xyz * spec_int;
			}
		}
	
		temp_col += light_col;
	}

	out_color = vec4 (temp_col, 1.0);
}

#endif