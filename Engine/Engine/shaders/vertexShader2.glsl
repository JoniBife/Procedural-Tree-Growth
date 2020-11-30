#version 330 core

in vec4 inPosition;
in vec3 inNormal;
in vec4 inColor;
in vec2 inTextCoord;
in vec3 inTangent;
in vec3 inBitangent;

out vec4 exPosition;
out vec3 exNormal;
out vec4 exColor;
out vec2 exTextCoord;
out vec3 exTangentLightPos;
out vec3 exTangentViewPos;
out vec3 exTangentFragPos;

uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

void main(void)
{
    exPosition = model * inPosition;   
    exTextCoord = inTextCoord;
    exColor = inColor;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    exTangentLightPos = TBN * lightPos;
    exTangentViewPos  = TBN * viewPos;
    exTangentFragPos  = TBN * vec3(exPosition);
        
    gl_Position = projection * view * model * inPosition;
}

/*#version 330 core
in vec4 inPosition;
in vec4 inColor;
in vec3 inNormal;
in vec2 inTextCoord;
in vec3 inTangent;
in vec3 inBitangent;  

out vec4 exPosition;
out vec2 exTextCoord;
out vec3 exTangentLightPos;
out vec3 exTangentViewPos;
out vec3 exTangentFragPos;


uniform vec3 lightPos;
uniform vec3 viewPos;


uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main(void)
{
	exPosition = inPosition;
	exColor = inColor;
	exTextCoord = inTextCoord;

	vec3 T = normalize(vec3(model * vec4(inTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(inBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(inNormal,    0.0)));
	exTBN = transpose(mat3(T, B, N));
	exTangentLightPos = exTBN * lightPos;
    exTangentViewPos  = exTBN * viewPos;
    exTangentFragPos  = exTBN * vec3(model * exPosition);

	gl_Position =  projection * view * model * inPosition;
}*/