#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 fragPos;
in vec3 LightPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightColor;
uniform Material material;
uniform Light light;

void main() {
	// ambient ligthing
	vec3 ambient = light.ambient *  vec3(texture(material.diffuse, texCoord));

	// diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(LightPos - fragPos);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = light.diffuse * diff *  vec3(texture(material.diffuse, texCoord)); 
	
	// specular lighting
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord)); 

	// setting the srength of each lighting
	diffuse *= light.diffuse;
	specular *= light.specular;
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}