#version 330 core
in vec4 in_Position;
out vec4 ex_Color;

uniform mat4 Matrix;

uniform vec4 Color_front;
uniform vec4 Color_back;

int vertCount = 1;


void main(void)
{
	gl_Position = Matrix * in_Position;

	int remain = vertCount % 6;

	if (remain == 4 || remain == 5 || remain == 0){
		ex_Color = Color_back;	
	}
	else ex_Color = Color_front;

	++vertCount;
}