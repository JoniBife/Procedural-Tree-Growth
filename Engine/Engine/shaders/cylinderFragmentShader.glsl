#version 330 core
out vec4 FragColor;

in vec2 exTextCoord;
in vec3 exNormal;

uniform sampler2D wood;

void main()
{
    vec3 textColor = texture(wood, exTextCoord * 2).rgb;
    vec3 normal = exNormal;
    FragColor = vec4(abs(exNormal), 1.0);   
}  