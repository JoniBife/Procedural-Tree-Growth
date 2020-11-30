#version 330 core

in vec4 exPosition;
in vec3 exNormal;
in vec4 exColor;
in vec2 exTextCoord;
in vec3 exTangentLightPos;
in vec3 exTangentViewPos;
in vec3 exTangentFragPos;

out vec4 fragmentColor;

uniform vec3 lightPos;
uniform vec4 color;
uniform vec3 viewPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main(void)
{
    vec3 normal = texture(normalMap, exTextCoord).rgb;
    
    normal = normalize(normal * 2.0 - 1.0);
	
  	
    // diffuse 
    vec3 color = texture(diffuseMap, exTextCoord).rgb;
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * color;

    vec3 lightDir = normalize(lightPos - vec3(exTangentFragPos));
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 viewDir = normalize(exTangentViewPos - exTangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);

    vec3 specular = vec3(0.2) * spec;  

    fragmentColor = vec4(ambient + diffuse, 1.0);
}


/*#version 330 core

in vec4 exPosition;
in vec3 exNormal;
in vec4 exColor;
in vec2 exTextCoord;
in vec3 exTangentLightPos;
in vec3 exTangentViewPos;
in vec3 exTangentFragPos;
in mat3 exTBN;

out vec4 fragmentColor;

//uniform vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
	vec3 normal = texture(normalMap, exTextCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(diffuseMap, exTextCoord).rgb;

	vec3 lightDir = normalize(exTangentLightPos - exTangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

	fragmentColor = vec4(diffuse + normal, 1.0);


}*/