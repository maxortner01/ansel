#version 330 core
out vec4 FragColor;
  
in fData {
	vec4 position;
    vec4 vertexColor;

    // For lighting normal calculations in the fragment shader
    vec3 toLight;
    vec3 outNormal;

    mat4 modelMatrix;
} frag;

/*
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

in vec3 toLight;
in vec3 outNormal; */

void main()
{

	vec4 color = frag.vertexColor;

	float product = dot(frag.outNormal, frag.toLight);
	float brightness = max(product, 0.0);
	vec3 diffuse = brightness * vec3(1, 1, 1); //LIGHT COLOR
	
	//vec4 color = vec4(1, 1, 1, 1);
    FragColor = vec4(diffuse, 1.0) * color;
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 