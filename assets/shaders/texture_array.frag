#version 330 core
precision highp float;

#define LIGHT vec3(0.36, 0.80, 0.48)

uniform sampler2DArray tex;
in vec3 ex_uv;
in vec3 ex_normal;

out vec4 FragColor;

void main() {
	float s = dot(ex_normal, LIGHT)*0.25 + 0.75;
	FragColor = vec4(texture(tex, ex_uv).rgb * s, 1.0);
}