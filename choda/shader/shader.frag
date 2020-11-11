#version 330 core

#define NR_POINT_LIGHTS 4


struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
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
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform Spotlight spotlight;
uniform float time;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));

	// diffuse
	vec3 lightDir = -normalize(light.direction);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));
	
	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	// ambient ligthing
	vec3 ambient = light.ambient *  vec3(texture(material.texture_diffuse1, texCoord));

	// diffuse lighting
    vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff *  vec3(texture(material.texture_diffuse1, texCoord)); 
	
	// specular lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord)); 

	// calculate how lit the object should be
	float dist = length(light.position - fragPos);
	float luminosity = 1.0f / (light.constant + light.linear*dist + light.quadratic*dist*dist);

	return luminosity * (ambient + diffuse + specular);
}


vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 viewDir) {
	vec3 spotDir = vec3(0.0f, 0.0f, 1.0f);
	
	float theta = dot(spotDir, normalize(-fragPos));
	if (theta > light.outerCutOff) {
		float diff = max(dot(spotDir, normal), 0.0f);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));

		vec3 reflectDir = reflect(-spotDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));

		float dist = length(-fragPos);
		float luminosity = 1.0f / (light.constant + light.linear*dist + light.quadratic*dist*dist);

		// create soft edges around spotlight
		float intensity = 1.0f;	
		intensity = clamp((theta - light.outerCutOff) / (light.innerCutOff - light.outerCutOff), 0.0f, 1.0f);

		return intensity * luminosity * (diffuse + specular);

	}  
	else {
		return spotlight.ambient * vec3(texture(material.texture_diffuse1, texCoord));
	}
}

void main() {
	vec3 viewDir = normalize(-fragPos);
	vec3 norm = normalize(normal);
	// directional light
	vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);
	// point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += calculatePointLight(pointLight[i], norm, viewDir);
	}
	// spotlight
	result += calculateSpotlight(spotlight, norm, viewDir);

	FragColor = vec4(result, 1.0f);
}