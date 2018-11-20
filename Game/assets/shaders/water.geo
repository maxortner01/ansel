#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vData {
	vec4 position;
    vec4 vertexColor; 

    vec3 toLight;
    vec3 outNormal;
    
    mat4 modelMatrix;
	mat4 rotationMatrix;

	vec2 tex;
} VertexIn[3];

out fData {
	vec4 position;
    vec4 vertexColor; 

    vec3 toLight;
    vec3 outNormal;
    
    mat4 modelMatrix;
	mat4 rotationMatrix;
    
	vec2 tex;
} frag;

uniform mat4 projection;
uniform mat4 view;
uniform float frame;

void main(void)
{
    // Retrieve the three vertices that make up the triangle
    vec4 vecs[3], fvecs[3], c;
    vec3 calcNormal;
    for (int i = 0; i < gl_in.length(); i++) {
        vecs[i] = gl_in[i].gl_Position;
        vecs[i].y = vecs[i].y + ( ( sin(frame / 50.0 + vecs[i].x) + cos(frame / 50.0 + vecs[i].z) ) / 5.0 );

        fvecs[i] = vecs[i];
    }

    // Relativize the two other points to the first
    vecs[1] = vecs[1] - vecs[0];
    vecs[2] = vecs[2] - vecs[0];

    // Set out water color
    c = vec4(0.0, 0.0, 1.0, .85);

    // Calculate the normal
    calcNormal.x = vecs[1].y * vecs[2].z - vecs[1].z * vecs[2].y;
    calcNormal.y = vecs[1].z * vecs[2].x - vecs[1].x * vecs[2].z;
    calcNormal.z = vecs[1].x * vecs[2].y - vecs[1].y * vecs[2].x;

    // Process each point, setting the normal and new wave location
    for (int i = 0; i < gl_in.length(); i++)
    {
        frag.vertexColor = c;
        frag.toLight = VertexIn[i].toLight;
        frag.outNormal = normalize(calcNormal);
        frag.modelMatrix = VertexIn[i].modelMatrix;

        vec4 position = fvecs[i];
		frag.position = position;

        gl_Position = position * VertexIn[i].modelMatrix * view * projection;

        EmitVertex();
    }
    EndPrimitive();
}