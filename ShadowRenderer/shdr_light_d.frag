#version 330 core

struct Shadow {
	float alpha;
};

struct Light {
	vec3 ambient_color;
	vec3 diffuse_direction;
};

struct Deferred {
	sampler2D world;
	sampler2D normal;
	sampler2D color;
	sampler2D emissive;
	sampler2D shadow;
	sampler2D occlusion;
};

struct Effects {
	float ssao_alpha;
	float bloom_alpha;
};

uniform Light u_light;
uniform Shadow u_shadow;
uniform Deferred u_deferred;
uniform Effects u_effects;

in VS_OUT {
	vec2 uv;
} fs_in;

layout(location = 0) out vec4 o_color;
layout(location = 1) out vec3 o_bloom;

void main() {
	vec3 world = texture(u_deferred.world, fs_in.uv).xyz;
	vec3 normal = normalize(texture(u_deferred.normal, fs_in.uv).xyz);
	vec3 color = texture(u_deferred.color, fs_in.uv).xyz;
	float emissive = texture(u_deferred.emissive, fs_in.uv).x;
	float shadow = texture(u_deferred.shadow, fs_in.uv).x;
	float occlusion = texture(u_deferred.occlusion, fs_in.uv).x;

	if (world == vec3(0.0, 0.0, 0.0)) {
		discard;
	}

	// Diffuse
	vec3 light = normalize(-u_light.diffuse_direction);
	float diffuse_value = max(dot(normal, light), 0.0);

	// Shadow depth checking
	if (shadow >= 0.5) {
		diffuse_value *= u_shadow.alpha;
	}

	// Ambient
	vec3 diffuse = max(diffuse_value, emissive) * color;
	vec3 ambient = u_light.ambient_color * (1.0 - diffuse_value);

	o_color = vec4((diffuse + ambient) * (1.0 - occlusion * u_effects.ssao_alpha), 1.0);

	o_bloom = o_color.xyz * 0.4 * length(diffuse) * u_effects.bloom_alpha;
}