#version 330 core

uniform sampler2D u_first;
uniform sampler2D u_second;

in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 o_color;

void main() {
	o_color = texture(u_first, fs_in.uv) + texture(u_second, fs_in.uv);
}