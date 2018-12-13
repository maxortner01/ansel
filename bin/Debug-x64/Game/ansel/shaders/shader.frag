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

uniform vec4 mat_ambient;
uniform float mat_spec;

uniform vec4 mat_emissionColor;
uniform float mat_emissionStrength;

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_occlusion;
uniform sampler2D tex_emission;

uniform struct {
	int on;
	int type;

	vec4 location;
	vec3 color;
} lights[LIGHT_COUNT];

vec3 getLightDir(int index) {
	if (lights[index].type == POINT) {
		return normalize(lights[index].location - frag.position).xyz;
	}
	else if (lights[index].type == DIRECTIONAL) {
		return normalize(lights[index].location).xyz;
	}
}

vec3 getLightDiffuse(int index) {
	vec3 lightDir = getLightDir(index);
	float diff = max(dot(frag.outNormal, lightDir), 0.0);
	return diff * lights[index].color;
}

float getLightSpecular(int index) {
	float spec = 0.8;

	vec3 lightDir = getLightDir(index);
	vec4 viewDir = frag.rotationMatrix * vec4(normalize(camera_position - frag.position.xyz), 1.0);
	vec3 reflectDir = reflect(-lightDir, frag.outNormal);

	float specVal = pow(max(dot(viewDir.xyz, reflectDir), 0.0), mat_spec);
	return spec * specVal;
}

vec3 getLightMatAmbient(int index) {
	return lights[index].color * mat_ambient.xyz;
}

vec3 getMatEmission() {
	return texture(tex_emission, frag.tex).r * mat_emissionColor.xyz * mat_emissionStrength;
}

void main()
{
	vec4 color = frag.vertexColor;
	if (use_textures == 1) {
		color = color * texture(tex_albedo, frag.tex);
	}

	// ambient
	vec3 ambient = getLightMatAmbient(0);

	// diffuse
	vec3 diffuse = color.xyz * getLightDiffuse(0);

	// specular
	float specular = getLightSpecular(0);

	//color = vec3(1, 1, 1);
	FragColor = vec4(ambient + diffuse + specular + getMatEmission(), color.w);
}

void crap() {
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
	//vec3 normal_tex = texture(tex_normal, frag.tex).rgb;
	//normal_tex = normalize(normal_tex * 2.0 - 1.0) / 1.5;

	//normal = normalize(normal_tex + normal);

	// Calculate the dot-product between the normal and the
	// toLight vector
	float diff = max(dot(normal, toLight), 0.0);
	vec3 diffuse = diff * lights[0].color;

	color = (vec4(diffuse, 1.0)) * color;

	if (lighting != 1)
		color = frag.vertexColor;

	FragColor = color;
}