#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

in vec3 toLight;
in vec3 outNormal;

void main()
{
	float product = dot(outNormal, toLight);
	float brightness = max(product, 0.0);
	vec3 diffuse = brightness * vec3(1, 1, 1); //LIGHT COLOR
	
	vec4 color = vec4(1, 1, 1, 1);
    //FragColor = vertexColor;
    FragColor = vec4(diffuse, 1.0) * color;
} 