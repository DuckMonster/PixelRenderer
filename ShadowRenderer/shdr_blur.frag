#version 330 core

uniform sampler2D u_sampler;
uniform vec2 u_blurdirection;

in VS_OUT {
	vec2 uv;
} fs_in;

const int SAMPLE_SIZE = 3;
out vec4 o_color;

float w(float x) {
	return 1.0 / pow(1.2, x);
}

void main() {
	vec3 color = vec3(0.0);

	vec2 tSize = textureSize(u_sampler, 0);
	vec2 pUv = vec2(1.0, 1.0) / tSize;

	float weights[SAMPLE_SIZE + 1];
	weights[0] = 1.0;
	float weightTotal = 1.0;

	for(int i=1; i<SAMPLE_SIZE + 1; i++) {
		weights[i] = w(float(i));
		weightTotal += weights[i] * 2.0;
	}

	float wTotal = 0.0;

	for(int i=-SAMPLE_SIZE; i<=SAMPLE_SIZE; i++) {
		vec4 pix = texture2D(u_sampler, fs_in.uv + pUv * u_blurdirection * float(i));

		float w = weights[int(abs(float(i)))];

		color += pix.rgb * w;
		wTotal += w;
	}

	o_color = vec4(color / wTotal, 1.0);
	//o_color = vec4(1.0, 1.0, 1.0, 1.0);
}