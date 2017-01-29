#version 330 core

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_camera;
uniform mat4 u_light;

out VS_OUT {
	vec3 normal;
	vec3 shadowCoord;
} vs_out;

void main() {
	gl_Position = u_camera * u_model * vec4(a_vertex, 1.0);

	vs_out.normal = (u_model * vec4(a_normal, 0.0)).xyz;

	// Shadow
	mat4 biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	vs_out.shadowCoord = (biasMatrix * u_light * u_model * vec4(a_vertex, 1.0)).xyz;
}