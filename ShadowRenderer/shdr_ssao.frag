#version 330 core

struct Deferred {
	sampler2D world;
	sampler2D normal;
	sampler2D depth;
};

uniform sampler2D u_noise;
uniform Deferred u_deferred;
uniform vec3 u_kernels[64];
uniform mat4 u_projection;
uniform float u_bias;
uniform float u_radius;

uniform vec2 u_noiseScale;

in VS_OUT {
	vec2 uv;
} fs_in;

out float o_color;

void main() {
	vec3 world = texture(u_deferred.world, fs_in.uv).xyz;
	vec3 normal = normalize(texture(u_deferred.normal, fs_in.uv).xyz);
	float depth = texture(u_deferred.depth, fs_in.uv).x;

	vec3 noiseVector = texture(u_noise, fs_in.uv * u_noiseScale).xyz;

	if (depth >= 1.0)
		discard;

	float occlusion = 0.0;

	for(int i; i<64; i++) {
		vec3 kernel = u_kernels[i];

		// Calculate TBN
		vec3 N = normal;
		vec3 T = normalize( cross( normal, noiseVector ) );
		vec3 B = cross( normal, T );
		mat3 TBN = mat3( T, B, N );

		kernel = TBN * kernel;

		vec3 sample = world + kernel * u_radius;

		vec4 proj = u_projection * vec4(sample, 1.0);
		proj.xyz /= proj.w;
		proj.xyz = proj.xyz * 0.5 + 0.5;

		float gDepth = texture(u_deferred.depth, proj.xy).x;
		vec3 gWorld = texture(u_deferred.world, proj.xy).xyz;

		float rangeCheck = smoothstep(1.0, 0.0, (distance(world, gWorld) / u_radius) - u_radius);

		occlusion += (gDepth < proj.z - u_bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = occlusion / 64.0;
	o_color = occlusion;
}