#version 330 core

uniform float u_bias;
uniform float u_anglebias;

uniform sampler2D u_shadowbuffer;
uniform vec3 u_lightdirection;

in VS_OUT {
	vec3 normal;
	vec3 shadowCoord;
} fs_in;

out float o_value;

void main() {
	// Bias tangent
	float bias = clamp( u_bias, 0.0, 0.01 );

	// Shadow depth checking
	// Facing away from light
	if ( dot( u_lightdirection, fs_in.normal ) >= -u_anglebias ) {
		o_value = 1.0;
	} else {
		float depth = fs_in.shadowCoord.z;
		float bufferDepth = texture(u_shadowbuffer, fs_in.shadowCoord.xy).z;

		o_value = (depth > bufferDepth + bias) ? 1.0 : 0.0;
	}
}