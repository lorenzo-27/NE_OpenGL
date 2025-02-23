#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;

uniform mat4 mvp;
uniform mat4 mv;

out vec3 ex_normal;

void main() {
	gl_Position = vec4(in_pos.xy, 0.0, 1.0);
	vec3 eye_normal = normalize((mvp * gl_Position).xyz);
	ex_normal = normalize((mv * vec4(eye_normal, 0.0)).xyz);
}
