#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec2 _texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.0f);
	texCoords = _texCoords;
}