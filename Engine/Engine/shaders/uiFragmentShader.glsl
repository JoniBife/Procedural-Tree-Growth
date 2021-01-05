#version 330 core
in vec2 exTextCoord;
out vec4 fragmentColor;

uniform sampler2D texture;

void main()
{    
    fragmentColor = texture(texture,exTextCoord);
} 