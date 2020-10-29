#version 330 core

in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position =  projection * view * model * in_Position;
	ex_Color = in_Color;
}