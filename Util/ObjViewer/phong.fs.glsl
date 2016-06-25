R"(
#version 430 core
struct LightSource
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec3 spotDirection;
	float spotCutoff, spotExponent;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
};

struct material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

in vec4 position;   // position of the vertex (and fragment) in world space
in vec3 normal;		// surface normal vector in world space
uniform mat4 m, v, p;
uniform mat4 v_inv;
uniform LightSource light0;

material frontMaterial = material(
	vec4(0.8, 0.8, 0.8, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	100.0
);

void main()
{
	vec3 normalDirection = normalize(normal);
	vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));
	vec3 lightDirection;
	float attenuation;

	if (0.0 == light0.position.w) // directional light
	{
		attenuation = 1.0; // no attenuation
		lightDirection = normalize(vec3(light0.position));
	}
	else // point light, spotlight, etc.
	{
		vec3 positionToLightSource = vec3(light0.position - position);
		float distance = length(positionToLightSource);
		lightDirection = normalize(positionToLightSource);
		attenuation = 1.0 / (light0.constantAttenuation
							 + light0.linearAttenuation * distance
							 + light0.quadraticAttenuation * distance * distance);

		if (light0.spotCutoff <= 90.0) // spotlight
		{
			float clampedCosine = max(0.0, dot(-lightDirection, light0.spotDirection));
			if (clampedCosine < cos(radians(light0.spotCutoff))) // outside of spotlight cone
			{
				attenuation = 0.0;
			}
			else
			{
				attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
			}
		}
	}

	vec3 ambientLighting = vec3(light0.ambient) * vec3(frontMaterial.ambient);

	vec3 diffuseReflection = attenuation
		* vec3(light0.diffuse) * vec3(frontMaterial.diffuse)
		* max(0.0, dot(normalDirection, lightDirection));

	vec3 specularReflection;
	if (dot(normalDirection, lightDirection) < 0.0) // lighting from the back face
	{
		specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
	else // lighting from the front face
	{
	specularReflection = attenuation * vec3(light0.specular) * vec3(frontMaterial.specular)
		* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), frontMaterial.shininess);
	}

	gl_FragColor = vec4(ambientLighting + diffuseReflection + specularReflection, 1.0);
}
)"
