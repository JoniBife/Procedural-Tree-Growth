#version 400 core

in vec4 InPosition;
in vec3 InNormal;
in vec4 InColor;
in vec2 InTextCoord;

out vec4 ExPosition;
out vec3 ExNormal;
out vec4 ExColor;
out vec2 ExTextCoord;

uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	ExPosition = InPosition;
	ExNormal = InNormal;
	ExColor = InColor;
	ExTextCoord = InTextCoord;

	gl_Position =  projection * view * model * InPosition;
}