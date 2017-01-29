#version 330 core

struct Material {
	vec4 color;
	float emissive;
};

uniform Material u_material;

in VS_OUT {
	vec3 world;
	vec3 normal;
} fs_in;

layout(location = 0) out vec4 o_color;
layout(location = 1) out float o_emissive;
layout(location = 2) out vec3 o_world;
layout(location = 3) out vec3 o_normal;

void main() {
	o_color = u_material.color;
	o_emissive = u_material.emissive;
	o_world = fs_in.world;
	o_normal = fs_in.normal;
}