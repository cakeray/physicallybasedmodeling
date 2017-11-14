#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 in_tex_coords;

out vec2 out_tex_coords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
	vec3 world_pos = vec3(model * vec4(pos,1.0));
	gl_Position = projection * view * vec4(world_pos,1.0);
	out_tex_coords = in_tex_coords;
}