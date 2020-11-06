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
	// position and direction isn't needed because we are in view space
	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;
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
	vec3 spotDir = vec3(0.0f, 0.0f, 1.0f);
	
	float theta = dot(spotDir, normalize(-fragPos));
	if (theta > spotlight.innerCutOff) {
		vec3 norm = normalize(normal);
		float diff = max(dot(spotDir, norm), 0.0f);
		vec3 diffuse = spotlight.diffuse * diff * vec3(texture(material.diffuse, texCoord));

		vec3 viewDir = normalize(-fragPos);
		vec3 reflectDir = reflect(-spotDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = spotlight.specular * spec * vec3(texture(material.specular, texCoord));

		float dist = length(-fragPos);
		float luminosity = 1.0f / (spotlight.constant + spotlight.linear*dist + spotlight.quadratic*dist*dist);

		return luminosity * (diffuse + specular);

	} else if (theta > spotlight.outerCutOff) {
		vec3 norm = normalize(normal);
		float diff = max(dot(spotDir, norm), 0.0f);
		vec3 diffuse = spotlight.diffuse * diff * vec3(texture(material.diffuse, texCoord));

		vec3 viewDir = normalize(-fragPos);
		vec3 reflectDir = reflect(-spotDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = spotlight.specular * spec * vec3(texture(material.specular, texCoord));

		float dist = length(-fragPos);
		float luminosity = 1.0f / (spotlight.constant + spotlight.linear*dist + spotlight.quadratic*dist*dist);

		// create soft edges around spotlight
		float intensity = clamp((theta - spotlight.outerCutOff) / (spotlight.innerCutOff - spotlight.outerCutOff), 0.0f, 1.0f);

		return intensity * luminosity * (diffuse + specular);
		
	} else {
		return spotlight.ambient * vec3(texture(material.diffuse, texCoord));
	}
}

void main() {
	//vec3 result = calculateDirectionalLight();
	//vec3 result = calculatePointLight();
	vec3 result = calculateSpotlight();

	FragColor = vec4(result, 1.0f);
}