#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	vec3 n = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(n, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}