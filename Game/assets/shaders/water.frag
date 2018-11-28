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

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform mat4 projection;
uniform mat4 view;

float LinearizeDepth(vec2 uv)
{
  float n = 0.1; // camera z near
  float f = 10.0; // camera z far
  float z = texture(texture1, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));	
}

float LinearizeDepth(float z)
{
  float n = 0.1; // camera z near
  float f = 50.0; // camera z far
  return (2.0 * n) / (f + n - z * (f - n));	
}

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
  
	vec4 pos = frag.position * frag.modelMatrix * view * projection;
	vec4 world = frag.position * frag.modelMatrix * view;
  
	vec2 screen = (pos.xy / pos.w) / 2.0 + 0.5;
  
	float depth = LinearizeDepth(screen);
	depth = depth - LinearizeDepth(world.z);
	
	vec4 ground_color = texture(texture0, screen) * depth;
	vec4 blue_color = vec4(color.x, color.y, color.z, color.a) * vec4(diffuse, 1.0) * (-depth + 1);

	FragColor = ground_color + blue_color;
}
