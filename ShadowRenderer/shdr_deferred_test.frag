#version 330 core

in VS_OUT {
	vec3 world;
	vec3 normal;
} fs_in;

layout(location = 0) out float o_color;

void main() {
	//o_color = vec4(1.0, 0.0, 0.0, 1.0);
	o_color = 1.0;
}