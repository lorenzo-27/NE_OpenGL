#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

uniform mat4 mvp;
uniform mat4 mv;

out vec2 ex_uv;
out vec3 ex_normal;

void main() {
    gl_Position = mvp * vec4(in_pos, 1.0);
    ex_uv = in_uv;
    ex_normal = normalize((mv * vec4(in_normal, 0.0)).xyz);
}
