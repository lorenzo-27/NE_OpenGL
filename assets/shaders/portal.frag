#version 330 core
precision highp float;

uniform sampler2D tex;
in vec4 ex_uv;

out vec4 FragColor;

void main() {
	vec2 uv = (ex_uv.xy / ex_uv.w);
	uv = uv*0.5 + 0.5;
	FragColor = vec4(texture(tex, uv).rgb, 1.0);
}