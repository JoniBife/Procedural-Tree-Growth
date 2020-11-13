#version 330 core

in vec4 inPosition;
in vec3 inNormal;
in vec4 inColor;
in vec2 inTextCoord;

out vec4 exPosition;
out vec3 exNormal;
out vec4 exColor;
out vec2 exTextCoord;

uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
	exPosition = inPosition;
	//exNormal = inNormal;
	exColor = inColor;
	exTextCoord = inTextCoord;

	vec4 transNormal = model * vec4(inNormal,1.0f);
	exNormal = transNormal.xyz;
	gl_Position =  projection * view * model * inPosition;
}