#version 330 core
in vec3 position;
in vec3 normal;
in vec3 worldPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lampColor;
uniform vec3 lightPos;

void main() {
	// ambient ligthing
	const float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lampColor;
	// diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - worldPos);
	float diff = max(dot(lightDirection, norm), 0.0f);
	vec3 diffuse = diff * lampColor; 

	FragColor = vec4((ambient + diffuse) * objectColor, 1.0f);
}