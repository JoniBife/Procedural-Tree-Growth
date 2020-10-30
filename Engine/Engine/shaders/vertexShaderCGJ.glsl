#version 330 core

in vec4 in_Position;

uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	gl_Position =  projection * view * model * in_Position;
}