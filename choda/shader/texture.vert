#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 2) in vec2 _texCoord;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.0f);
	texCoord = _texCoord;
}