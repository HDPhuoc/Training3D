#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

in vec2 texCoords;
in vec3 normalVector;
in vec3 fragPos;

uniform vec3 viewPosition;
void main()
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords)).rgb;

	vec3 normal = normalize(normalVector);
	vec3 lightDirection = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords)).rgb;

	vec3 viewDir = normalize(viewPosition - fragPos);
	vec3 reflectDir = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords)).rgb;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}