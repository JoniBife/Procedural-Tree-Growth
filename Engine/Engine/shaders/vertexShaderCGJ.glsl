#version 330 core
in vec4 in_Position;

uniform mat4 Transformation;

void main(void)
{
	gl_Position = Transformation * in_Position;
}