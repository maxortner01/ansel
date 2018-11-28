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
	
	flat int type;
} frag;

uniform int lighting;
uniform int use_textures;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// Indentify the color
	vec4 color = frag.vertexColor;

	vec4 texColor;
	texColor = texture(texture0, frag.tex) * (-frag.type + 1);
	texColor = texColor + texture(texture1, frag.tex) * (frag.type);

	//texColor.z = frag.tex.x;
	//texColor.w = 1;

	color = (texColor * use_textures) + (color * (-use_textures + 1));
	
	
	// Calculate the dot-product between the normal and the 
	// toLight vector
	float product = dot(frag.outNormal, frag.toLight);
	// Calculate the brightness value
	float brightness = max(product, 00);
	// Calculate the color effect of the light
	vec3 diffuse = brightness * vec3(1, 1, 1);
		
	// Multiply the color light by the original color
	color = vec4(diffuse, 1.0) * color;
	
	FragColor = color;
} 