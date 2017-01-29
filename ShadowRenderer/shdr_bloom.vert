#version 330 core

layout(location = 0) in vec2 a_vertex;
layout(location = 2) in vec2 a_uv;

out VS_OUT {
	vec2 uv;
} vs_out;

void main() {
	gl_Position = vec4(a_vertex, 0.0, 1.0);
	vs_out.uv = a_uv;
}