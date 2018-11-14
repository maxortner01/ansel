#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;

layout (location = 3) in vec4 location;
layout (location = 4) in vec4 rotation;
layout (location = 5) in vec4 scale;

uniform vec4 light_position[8];
uniform int  light_state[8];

out vData {
	vec4 position;
    vec4 vertexColor; 

    vec3 toLight;
    vec3 outNormal;
    
    mat4 modelMatrix;
	mat4 rotationMatrix;
} VertexIn;


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
	// Make the toLight vector
	VertexIn.toLight   = vec3(light_position[0]);
	// Create the rotation matrix for rotating the normals
	VertexIn.rotationMatrix = makeRotationMatrix(rotation);

	// Create the model matrix
	mat4 model = makeModel(location, rotation, scale);	

	// Set the position for the geometry shader
	VertexIn.position = vec4(
		vertex.x,
		vertex.y,
		vertex.z,
		1.0
	);

	// Pass the position to the vertex shader
	gl_Position = VertexIn.position;
	// Pass the model matrix
	VertexIn.modelMatrix = model;
	// Pass the color
	VertexIn.vertexColor = color;
	// Pass the normal, even if it isn't used
	VertexIn.outNormal = normal;
}