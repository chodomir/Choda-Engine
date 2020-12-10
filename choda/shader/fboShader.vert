#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 2) in vec2 _texCoord;

out vec2 texCoord;

void main() {
	gl_Position = vec4(_position.xy/3 + 0.67f, 0.0f, 1.0f);
	texCoord = _texCoord;
}