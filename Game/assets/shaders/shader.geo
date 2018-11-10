#version 400 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vData {
	vec4 position;
    vec4 vertexColor;

    vec3 toLight;
    vec3 outNormal;

    mat4 modelMatrix;
} VertexIn[3];

out fData {
	vec4 position;
    vec4 vertexColor; 

    vec3 toLight;
    vec3 outNormal;
    
    mat4 modelMatrix;
} frag;

uniform mat4 projection;
uniform mat4 view;

void main(void)
{
    vec4 vecs[3], c;
    vec3 calcNormal;
    for (int i = 0; i < gl_in.length(); i++) {
        vecs[i] = gl_in[i].gl_Position;
    }

    vecs[1] = vecs[1] - vecs[0];
    vecs[2] = vecs[2] - vecs[0];

    if (vecs[0].y / 12.0 > -.75) {
        c = vec4(0.0, 1.0, 0.0, 1.0);
    } else {
        c = vec4(0.9294, 0.7882, 0.6863, 1.0);
    }

    calcNormal.x = vecs[1].y * vecs[2].z - vecs[1].z * vecs[2].y;
    calcNormal.y = vecs[1].z * vecs[2].x - vecs[1].x * vecs[2].z;
    calcNormal.z = vecs[1].x * vecs[2].y - vecs[1].y * vecs[2].x;

    for (int i = 0; i < gl_in.length(); i++)
    {
        frag.vertexColor = c;
        frag.toLight = VertexIn[i].toLight;
        frag.outNormal = calcNormal;
        frag.modelMatrix = VertexIn[i].modelMatrix;

        gl_Position = gl_in[i].gl_Position * VertexIn[i].modelMatrix * view * projection;

        EmitVertex();
    }
    EndPrimitive();
}