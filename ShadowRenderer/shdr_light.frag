#version 330 core

struct Shadow {
	sampler2D buffer;
	vec3 color;

	float bias;
	float alpha;
};

uniform vec3 u_lightdirection;
struct Light {
	vec3 ambient_color;

	vec3 diffuse_color;
	vec3 diffuse_direction;
};

uniform Light u_light;
uniform Shadow u_shadow;

in VS_OUT {
	vec3 normal;
	vec3 shadowCoord;
} fs_in;

out vec4 o_color;

void main() {
	// Diffuse
	vec3 normal = normalize(fs_in.normal);
	vec3 light = normalize(-u_light.diffuse_direction);
	float diffuse_value = max(dot(normal, light), 0.0);

	// Shadow depth checking
	float depth = fs_in.shadowCoord.z;
	float bufferDepth = texture(u_shadow.buffer, fs_in.shadowCoord.xy).z;

	if ((depth + u_shadow.bias) > bufferDepth) {
		diffuse_value *= u_shadow.alpha;
	}

	// Ambient
	vec3 diffuse = diffuse_value * u_light.diffuse_color;
	vec3 ambient = u_light.ambient_color * (1.0 - diffuse_value);

	o_color = vec4(diffuse + ambient, 1.0);
}