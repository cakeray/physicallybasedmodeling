#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 in_tex_coords;

out vec2 out_tex_coords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
	gl_Position = projection * model * view * vec4(pos,1.0);
	out_tex_coords = in_tex_coords;
}