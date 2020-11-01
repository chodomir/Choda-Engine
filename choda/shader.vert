#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 normal;
out vec3 fragPos;
out vec3 LightPos;
out vec2 texCoord;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.0f);
	normal = mat3(transpose(inverse(view * model))) * _normal;
	fragPos = vec3(view * model * vec4(_position, 1.0f));
	LightPos = vec3(view * vec4(lightPos, 1.0f));
	texCoord = _texCoord;
}