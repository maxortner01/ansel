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

// Pull in the projection and view matrix for transformations
uniform mat4 projection;
uniform mat4 view;

// Whether or not to generate colors or use the buffered color values
uniform int use_colors;
uniform int use_normals;

void main(void)
{
    // Array of vectors for storing three points
    vec4 vecs[3];
    // The final normal vector
    vec3 calcNormal = vec3(0, 0, 0);
    // Loop through the points and push them to vecs
    for (int i = 0; i < gl_in.length(); i++) {
        vecs[i] = gl_in[i].gl_Position;
    }

    // Relativize the other two points to the first
    vecs[1] = vecs[1] - vecs[0];
    vecs[2] = vecs[2] - vecs[0];

    // Calculate the normal
    calcNormal.x = vecs[1].y * vecs[2].z - vecs[1].z * vecs[2].y;
    calcNormal.y = vecs[1].z * vecs[2].x - vecs[1].x * vecs[2].z;
    calcNormal.z = vecs[1].x * vecs[2].y - vecs[1].y * vecs[2].x;

    for (int i = 0; i < gl_in.length(); i++)
    {
        if (use_normals == 1) {
            calcNormal = vec3(vec4(normalize(VertexIn[i].outNormal), 1.0) * VertexIn[i].rotationMatrix);
        } else {
            calcNormal = vec3(vec4(normalize(calcNormal), 1.0) * VertexIn[i].rotationMatrix);
        }

        // Pass all useful information to the fragment shader
        frag.vertexColor = VertexIn[i].vertexColor;
        frag.toLight = VertexIn[i].toLight;
        frag.outNormal = calcNormal;
        frag.modelMatrix = VertexIn[i].modelMatrix;
        frag.tex = VertexIn[i].tex;

        // Transform the vertex once and for all in 3D space
        gl_Position = gl_in[i].gl_Position * VertexIn[i].modelMatrix * view * projection;

        EmitVertex();
    }
    EndPrimitive();
}
