#version 330 core

layout(location = 0) in vec3 a_vertex;

uniform mat4 u_model;
uniform mat4 u_camera;

void main() {
	gl_Position = u_camera * u_model * vec4(a_vertex, 1.0);
}