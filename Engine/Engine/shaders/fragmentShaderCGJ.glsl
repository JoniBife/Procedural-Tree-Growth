#version 330 core

in vec4 exPosition;
in vec3 exNormal;
in vec4 exColor;
in vec2 exTextCoord;

out vec4 fragmentColor;

uniform vec4 color;

uniform sampler2D woodTexture;

void main(void)
{
	fragmentColor = texture(woodTexture, exTextCoord);
}