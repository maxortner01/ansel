#version 330 core
out vec4 FragColor;
  
in fData {
	vec4 position;
    vec4 vertexColor;

    vec3 toLight;
    vec3 outNormal;

    mat4 modelMatrix;
	mat4 rotationMatrix;
} frag;

void main()
{
	// Indentify the color
	vec4 color = frag.vertexColor;

	// Calculate the dot-product between the normal and the 
	// toLight vector
	float product = dot(frag.outNormal, frag.toLight);
	// Calculate the brightness value
	float brightness = max(product, 00);
	// Calculate the color effect of the light
	vec3 diffuse = brightness * vec3(1, 1, 1);
	
	// Multiply the color light by the original color
    FragColor = vec4(diffuse, 1.0) * color;
} 