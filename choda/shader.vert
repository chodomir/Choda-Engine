#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position;
out vec3 normal;
out vec3 fragPos;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.0f);
	position = _position;
	normal = mat3(transpose(inverse(model))) * _normal;
	// normal = _normal;
	fragPos = vec3(model * vec4(position, 0.0f));
}