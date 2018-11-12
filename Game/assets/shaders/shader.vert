#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

layout (location = 3) in vec4 location;
layout (location = 4) in vec4 rotation;
layout (location = 5) in vec4 scale;

uniform float frame;
uniform mat4  projection;
uniform mat4  view;
uniform float  use3D;

uniform vec4 light_position[8];
uniform int  light_state[8];
//uniform float intensities[16];

out vData {
	vec4 position;
    vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

    vec3 toLight;
    vec3 outNormal;
    
    mat4 modelMatrix;
} VertexIn;

/*
out vec4 vertexColor;

// For lighting normal calculations in the fragment shader
out vec3 toLight;
out vec3 outNormal;

out mat4 modelMatrix;
out mat4 viewMatrix;
out mat4 projectionMatrix; */

mat4 makeTransformMatrix(vec4 loc) {
	return mat4(	
			1, 0, 0, loc.x,
			0, 1, 0, loc.y,
			0, 0, 1, loc.z,
			0, 0, 0, 1 
		);
}

mat4 makeRotationMatrix(vec4 rot) {
	mat4 rot_x = mat4(
		1, 0, 		   0,		    0,
		0, cos(rot.x), -sin(rot.x), 0,
		0, sin(rot.x),  cos(rot.x), 0,
		0, 0,          0,           1
	);
	
	mat4 rot_y = mat4(
		 cos(rot.y), 0, sin(rot.y), 0,
		 0,			 1, 0, 		    0,
		-sin(rot.y), 0, cos(rot.y), 0,
		 0, 	     0, 0,          1
	);
	
	mat4 rot_z = mat4(
		cos(rot.z), -sin(rot.z), 0, 0,
		sin(rot.z),  cos(rot.z), 0, 0,
		0, 			 0, 		 1, 0,
		0, 			 0, 		 0, 1
	);
	
	return rot_x * rot_y * rot_z;
}

mat4 makeScaleMatrix(vec4 sc) {
	return mat4(
		sc.x, 0,    0,    0,
		0,    sc.y, 0,    0,
		0,    0,    sc.z, 0,
		0,    0,    0,    1
	);
}


mat4 makeModel(vec4 loc, vec4 rot, vec4 sc) {	
	return makeScaleMatrix(sc) * makeRotationMatrix(rot) * makeTransformMatrix(loc);
}

float getDotProduct(vec4 v1, vec4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec4 normalizeVector(vec4 vector) {
	vec4 v = vector;
	
	float length = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	v = vec4(
		v.x / length,
		v.y / length,
		v.z / length,
		1
	);
	return v;
}

void main(void)
{
	mat4 model = makeModel(location, rotation, scale);
	vec4 norm  = normalizeVector(vec4(normal, 1.0) * makeRotationMatrix(rotation));
	
	//VertexIn.outNormal = vec3(norm.xyz);
	VertexIn.toLight   = vec3(light_position[0]);
	
	//float dp = getDotProduct(norm, lights[0]);
	
	//vec4 c = color;
	//vec4 c = vec4(1.0, 1.0, 1.0, 1.0);
	
	//if (dp <= 0) {
	//	c.x = 0;
	//	c.y = 0;
	//	c.z = 0;
	//} else {
	//	c.x = c.x * dp;
	//	c.y = c.y * dp;
	//	c.z = c.z * dp;
	//} 
	
	VertexIn.position = vec4(
		vertex.x,
		vertex.y,
		vertex.z,
		1.0
	);

	gl_Position = vec4(
			vertex.x,
			vertex.y,
			vertex.z,
			1.0
		);

	VertexIn.modelMatrix = model;
	
	vec4 normVec = normalizeVector(vec4(vertex, 1.0));
	float val = abs(normVec.y) * 1.5;
	val = -val + .75;
	val = pow(val, 2);
	//VertexIn.vertexColor = vec4(1.0 * val, 1.0 * val, 1.0 * val, 1.0);
	VertexIn.vertexColor = color;
}