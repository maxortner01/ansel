#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in vec4 loc;

uniform float frame;
uniform mat4  projection;
uniform mat4  view;
uniform float  use3D;

//uniform vec4 location[1000];

out vec4 vertexColor; 

void main(void)
{
	/*
	vec3 fVertex = vec3(
		vertex.x * scale[gl_InstanceID].x,
		vertex.y * scale[gl_InstanceID].y,
		vertex.z * scale[gl_InstanceID].z,
	);
	*/
	
	if (use3D == 0.0) {
		gl_Position = vec4(
			vertex.x + loc.x,
			vertex.y + loc.y,
			vertex.z + loc.z,
			1.0
		) * view * projection;
	}
	else {
		gl_Position = vec4(
			vertex.x + loc.x,
			vertex.y + loc.y,
			vertex.z + loc.z,
			1.0
		);
	}
	
	vertexColor = color;
}