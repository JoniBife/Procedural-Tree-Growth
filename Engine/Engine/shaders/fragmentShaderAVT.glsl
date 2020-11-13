#version 400 core

in vec4 ExPosition;
in vec3 ExNormal;
in vec4 ExColor;
in vec2 ExTextCoord;

out vec4 FragmentColor;

void main(void)
{
	vec3 color = (ExNormal + vec3(1.0)) * 0.5;
	FragmentColor = vec4(color,1);
	//FragmentColor = ExColor;
	//FragmentColor = vec4(vec3(gl_FragCoord.z), 1.0);
	/*vec3 N = normalize(ExNormal);
	vec3 direction = vec3(0.0, 0.0, 1.0);
	float intensity = max(dot(direction, N), 0.0);
	FragmentColor = vec4((vec3(intensity) + vec3(0,0,1))/2, 1.0);*/
}