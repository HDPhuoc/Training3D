#version 300 es
precision highp float;

out vec4 FragColor;

uniform sampler2D materialDiffuse;
uniform sampler2D materialSpecular;
uniform float materialShininess;

struct Light {
	vec3 position;
	vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};
uniform Light light;


uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(materialDiffuse, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(materialDiffuse, TexCoords).rgb;
		
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specular = light.specular * spec * texture(materialSpecular, TexCoords).rgb;

	// spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse = diffuse * intensity;
	specular = specular * intensity;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		
	ambient = ambient * attenuation; 
	diffuse = diffuse * attenuation;
	specular = specular * attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}