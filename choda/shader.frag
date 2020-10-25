#version 330 core
in vec3 position;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lampColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	// ambient ligthing
	const float ambientStrength = 0.05f;
	vec3 ambient = ambientStrength * lampColor;

	// diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = diff * lampColor; 
	
	// specular lighting
	const float specularStrength = 0.8f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);
	vec3 specular = specularStrength * spec * lampColor; 

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0f);
}