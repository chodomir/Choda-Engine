#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture1;

void main() {
	fragColor = vec4(vec3(texture(texture1, texCoord)), 1.0f);
}