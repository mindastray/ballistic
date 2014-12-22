uniform vec3        effect_diffuse;
uniform vec3		effect_color_mask;
uniform float		effect_opacity;
uniform mat4        effect_t_model;

#ifdef VERTEX_SHADER

layout (location = 1) in vec3 in_position;
layout (location = 2) in vec2 in_uv;

out vec2            var_uv;

void main () {
	var_uv = in_uv;
	// opengl invert texture
	var_uv.y = 1.0 - var_uv.y;
	vec4 pos = effect_t_model * vec4 (in_position.xyz, 1.0);
	gl_Position = pos;
}

#endif

#ifdef FRAGMENT_SHADER

uniform sampler2D   effect_texture;

in vec3 var_normal;
in vec2 var_uv;
out vec4 out_color;

void main () {
	vec4 texture_color = texture (effect_texture, var_uv);
	out_color.rgb = effect_color_mask * (effect_diffuse + texture_color.rgb);
	out_color.a = texture_color.a * effect_opacity;
}

#endif