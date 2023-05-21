#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;

uniform sampler2D Texture;
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;


float CalculateDirectionalIllumination()
{
	float ambient = 0.1f;

	vec3 Nnor = normalize(nor);
	vec3 Nto_light = normalize(-lightDirection);
	float diffuse = max(dot(Nto_light, Nnor), 0.f);

	vec3 Nfrom_light = normalize(lightDirection);
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

	float specular = pow(max(dot(NcamDirection, NrefLight), 0), 64);

	return (ambient + diffuse + specular);
}

float CalculatePositionalIllumination()
{
	float ambient = 0.1f;

	vec3 Nnor = normalize(nor);
	vec3 Nto_light = normalize(lightPos - FragPosWorldSpace);
	float diffuse = max(dot(Nto_light, Nnor), 0.f);

	vec3 Nfrom_light = -Nto_light;
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

	float specular = pow(max(dot(NcamDirection, NrefLight), 0), 64);

	float distance = length(lightPos - FragPosWorldSpace);
	float attenuation = 1 / (1.5 + (0.05 * distance) + (0.02 * pow(distance, 2)));

	return (ambient + diffuse + specular) * attenuation;
}


float CalculateSpotIllumination()
{
	float ambient = 0.4f;

	vec3 Nnor = normalize(nor);
	vec3 Nto_light = normalize(lightPos - FragPosWorldSpace);
	float diffuse = max(dot(Nto_light, Nnor), 0.f);

	vec3 Nfrom_light = -Nto_light;
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);

	float specular = pow(max(dot(NcamDirection, NrefLight), 0), 64);

	float distance = length(lightPos - FragPosWorldSpace);
	float attenuation = 1 / (1.5 + (0.05 * distance) + (0.02 * pow(distance, 2)));

	float phi = cos(radians(15));
	vec3 NSpotDir = normalize(lightDirection);
	float theta = dot(Nfrom_light, NSpotDir);

	float phong;

	if (theta > phi)
	{
		phong = (ambient + diffuse + specular) * attenuation;
	}
	else 
	{
		phong = ambient * attenuation;
	}

	return phong;
}


void main()
{
	float phong = CalculateSpotIllumination();

	fColour = texture(Texture, tex) * phong;
}