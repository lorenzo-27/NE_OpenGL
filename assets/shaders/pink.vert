#version 330 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(in_pos, 1.0);
}
