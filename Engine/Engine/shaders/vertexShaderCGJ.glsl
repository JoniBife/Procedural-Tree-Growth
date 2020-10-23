#version 330 core
in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 Matrix;

void main(void)
{
	gl_Position = Matrix * in_Position;
}