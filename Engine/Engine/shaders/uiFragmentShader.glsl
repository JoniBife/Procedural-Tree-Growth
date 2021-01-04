#version 330 core
in vec2 exTextCoord;
out vec4 fragmentColor;

uniform sampler2D texture;

void main()
{    
    fragmentColor = vec4(texture(texture,exTextCoord).rgb, 1.0f);
} 