#version 330 core

in vec4 exPosition;
in vec3 exNormal;
in vec4 exColor;
in vec2 exTextCoord;

out vec4 fragmentColor;

uniform vec4 color;

void main(void)
{
	//vec3 color = (exNormal + vec3(1.0)) * 0.5;
	//fragmentColor = vec4(color,1);
	fragmentColor = color;
	//fragmentColor = exColor;
	//fragmentColor = vec4(vec3(gl_FragCoord.z), 1.0);
	/*vec3 N = normalize(exNormal);
	vec3 direction = vec3(0.0, 0.0, 1.0);
	float intensity = max(dot(direction, N), 0.0);
	fragmentColor = vec4((vec3(intensity) + vec3(0,0,1))/2, 1.0);*/
}