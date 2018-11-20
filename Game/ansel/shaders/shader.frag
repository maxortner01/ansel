#version 330 core
out vec4 FragColor;

in fData {
	vec4 position;
    vec4 vertexColor;

    vec3 toLight;
    vec3 outNormal;

    mat4 modelMatrix;
	mat4 rotationMatrix;

	vec2 tex;
} frag;

uniform int lighting;
uniform int use_textures;

void main()
{
	// Indentify the color
	vec4 color = frag.vertexColor;

	// Calculate the dot-product between the normal and the
	// toLight vector
	float product = dot(normalize(frag.outNormal), normalize(frag.toLight));
	// Calculate the brightness value
	float brightness = max(product, 0);
	// Calculate the color effect of the light
	vec3 diffuse = brightness * vec3(1, 1, 1);

	// Multiply the color light by the original color
	//color = vec4(diffuse, 1.0) * color;

	FragColor = color;
}
