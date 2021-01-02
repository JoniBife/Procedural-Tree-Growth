#version 330 core
in vec4 inPosition; // <vec2 pos, vec2 tex>
out vec2 exTextCoords;

uniform mat4 projection;

void main()
{
    // No model matrix, it would be the same as having a model matrix which is the product identity
    gl_Position = projection * vec4(inPosition.xy, 0.0, 1.0);
    exTextCoords = inPosition.zw;
}  