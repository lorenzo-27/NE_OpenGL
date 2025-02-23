#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 0) in vec2 in_uv;

uniform mat4 mvp;

out vec4 ex_uv;

void main(void) {
	gl_Position = mvp * vec4(in_pos, 1.0);
	ex_uv = gl_Position;
}
