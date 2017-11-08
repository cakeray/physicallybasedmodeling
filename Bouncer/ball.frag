#version 430 core
in vec2 out_tex_coords;

out vec4 FragColor;

uniform sampler2D texture1;

void main(){
	FragColor = texture(texture1,out_tex_coords);
}