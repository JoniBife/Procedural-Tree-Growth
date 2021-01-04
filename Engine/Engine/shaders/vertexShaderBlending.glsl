#version 330 core
// INPUT
in vec4 inPosition;
in vec3 inNormal;
in vec2 inTextCoord;
in vec3 inTangent;

uniform mat3 normal;
uniform mat4 model;

out vec2 TexCoords;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main()
{
	mat3 normalmat = normal;
    TexCoords = inTextCoord;
    gl_Position = projection * view * model * inPosition;
}