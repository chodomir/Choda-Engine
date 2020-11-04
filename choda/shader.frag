#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emmision;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight {
	float cutOff;

};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform float time;

vec3 calculateDirectionalLight() {
	// ambient
	vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, texCoord));

	// diffuse
	vec3 lightDir = -normalize(dirLight.direction);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// specular
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, texCoord));
	
	return ambient + diffuse + specular;
}

vec3 calculatePointLight() {
	// ambient ligthing
	vec3 ambient = pointLight.ambient *  vec3(texture(material.diffuse, texCoord));

	// diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = pointLight.diffuse * diff *  vec3(texture(material.diffuse, texCoord)); 
	
	// specular lighting
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, texCoord)); 

	// calculate how lit the object should be
	float dist = length(pointLight.position - fragPos);
	float luminosity = 1.0f / (pointLight.constant + pointLight.linear*dist + pointLight.quadratic*dist*dist);

	return luminosity * (ambient + diffuse + specular);
}


vec3 calculateSpotlight() {
	
	return (1.0f);
}

void main() {
	//vec3 result = calculateDirectionalLight();
	vec3 result = calculatePointLight();

	FragColor = vec4(result, 1.0f);
}