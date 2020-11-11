#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 eyePosition;

out vec3 color;

void main() {
	gl_Position = projection * view * model * vec4(_position, 1.0f);

	vec3 vertWorldPos = vec3(model * vec4(_position, 1.0f));
	
	// ambient lighting
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lighting
	float diffuseStrength = 1.0f;
	vec3 lightDir = normalize(lightPosition - vertWorldPos);
	mat3 normalMatrix = mat3(transpose(inverse(model))); // not efficient, only for learning purposes
	vec3 normal = normalize(normalMatrix * _normal); // just in case
	float diffuseEffect = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diffuseStrength * diffuseEffect * lightColor;

	// specular lighting
	float specularStrength = 0.5f;
	float specularSpread = 32;
	vec3 reflection = reflect(-lightDir, normal);
	vec3 eyeDir = normalize(eyePosition - vertWorldPos);
	float specularEffect = pow(max(dot(reflection, eyeDir), 0.0f), specularSpread);
	vec3 specular = specularStrength * specularEffect * lightColor;

	color = (ambient + diffuse + specular) * objectColor;
}