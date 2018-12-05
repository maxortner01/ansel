#version 330 core

#define LIGHT_COUNT 8

#define AMBIENT     0
#define POINT       1
#define DIRECTIONAL 2

out vec4 FragColor;

in fData {
	vec4 position;
    vec4 vertexColor;

    vec3 outNormal;

    mat4 modelMatrix;
	mat4 rotationMatrix;

	vec2 tex;
} frag;

uniform int lighting;
uniform int use_textures;
uniform int use_colors;

uniform mat4 view;
uniform vec3 camera_position;

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_occlusion;

uniform struct {
	int on;
	int type;

	vec4 location;
	vec3 color;
} lights[LIGHT_COUNT];

void main()
{
	vec3 toLight;
	vec3 normal = normalize(frag.outNormal);
	vec3 vertexWorldSpace = (frag.position * frag.modelMatrix).xyz;
	if (lights[0].type == POINT) {
		toLight = normalize(lights[0].location.xyz - vertexWorldSpace);
	}
	else if (lights[0].type == DIRECTIONAL) {
		toLight = normalize(lights[0].location.xyz);
	}

	if (lighting == 0) {
		normal = toLight;
	}

	float spec_strength = 0.0;

	vec3 viewDir = normalize(camera_position - vertexWorldSpace);
	vec3 reflectDir = reflect(-toLight, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32 * 2);
	vec3 specular = spec_strength * spec * lights[0].color;

	// Indentify the color
	vec4 color = (use_colors == 0)?texture(tex_albedo, frag.tex):frag.vertexColor;

	// Obtain the normal from texture
	vec3 normal_tex = texture(tex_normal, frag.tex).rgb;
	normal_tex = normalize(normal_tex * 2.0 - 1.0) / 1.5;

	normal = normalize(normal_tex + normal);

	// Calculate the dot-product between the normal and the
	// toLight vector
	float diff = max(dot(normal, toLight), 0.0);
	vec3 diffuse = diff * lights[0].color;

	color = (vec4(diffuse, 1.0) + vec4(specular, 1.0)) * color;

	if (lighting != 1)
		color = frag.vertexColor;

	FragColor = color;
}
