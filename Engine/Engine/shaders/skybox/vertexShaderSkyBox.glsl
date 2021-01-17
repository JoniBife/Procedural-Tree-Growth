#version 330 core

in vec4 inPosition;

out vec3 exTextCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    exTextCoord = inPosition.xyz;
    gl_Position = projection * view * inPosition;
}  