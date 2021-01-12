#version 330 core

layout (lines) in; // [x,y,z,diameter]
layout (triangle_strip, max_vertices = 132) out;

out vec3 exNormal;
out vec2 exTextCoord;
out vec4 fragPos;

uniform mat3 normal;
uniform mat4 model;

uniform SharedMatrices {
	mat4 view;
	mat4 projection;
};

const int sectors = 16;
const float EPSILON = 0.000001f;

void generateCylinder(vec4 bottomPosition, vec4 topPosition, float bottomRadius, float topRadius)
{    
    const float PI = acos(-1);
    float sectorStep = (2 * PI / float(sectors));
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan((bottomRadius - topRadius) / length((bottomPosition - topPosition)));
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    vec4 unitCircleVertices[sectors + 1]; 
    vec3 sideNormals[sectors + 1];
    
    // Calculating the vertices of a circle centered in (0,0,0)
    for(int i = 0; i <= sectors; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices[i] = vec4(cos(sectorAngle), 0.0 ,sin(sectorAngle), 0.0);
        // Calcuting the side normals for smooth shading, this way we can reduce the number of vertices
        sideNormals[i] = vec3(cos(sectorAngle)*x0 - sin(sectorAngle)*z0, y0, sin(sectorAngle)*x0 + cos(sectorAngle)*z0); 
    }

    // Generating the top circle
    for(int i = 0; i <= sectors; ++i)
    {
        vec4 v = (topPosition + vec4(unitCircleVertices[i].x * topRadius, 0 ,unitCircleVertices[i].z * topRadius, 0.0));
        gl_Position = projection *  view * model * v; 
        exTextCoord = vec2(unitCircleVertices[i].x * 0.5 + 0.5, -unitCircleVertices[i].z * 0.5 + 0.5);
        exNormal = normal * vec3(0,1,0);
        fragPos = model * v;
        EmitVertex();
        gl_Position = projection *  view * model * (topPosition);   
        exTextCoord = vec2(0.5, 0.5);
        exNormal = normal * vec3(0,1,0);
        fragPos = model * topPosition;
        EmitVertex(); 
    }
    EndPrimitive();

    float u;
    vec4 v1, v2, v3, v4;

    // Generating the side of the cylinder, one surface at a time
    for(int i = 0; i < sectors; ++i)
    {
        u = float(i) / sectors; // u start at 0 increments to 1 as it reaches the last sector
        v1 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, 0 ,unitCircleVertices[i].z * bottomRadius, 0.0));
        gl_Position = projection *  view * model * v1;  
        exTextCoord = vec2(u, 0.0);
        exNormal = normal * sideNormals[i];
        fragPos = model * v1;
        EmitVertex(); 
        v1 = (topPosition + vec4(unitCircleVertices[i].x* topRadius, 0 ,unitCircleVertices[i].z* topRadius, 0.0));
        gl_Position = projection *  view * model * v1;
        exTextCoord = vec2(u, 1.0); 
        exNormal = normal * sideNormals[i];
        fragPos = model * v1;
        EmitVertex(); 

        u = float((i+1)) / sectors;
        v1 = (bottomPosition + vec4(unitCircleVertices[i + 1].x * bottomRadius, 0 ,unitCircleVertices[i + 1].z * bottomRadius, 0.0));
        gl_Position = projection *  view * model * v1;
        exTextCoord = vec2(u, 0.0);
        exNormal = normal * sideNormals[i + 1];
        fragPos = model * v1;
        EmitVertex(); 
        v1 = (topPosition + vec4(unitCircleVertices[i + 1].x* topRadius, 0 ,unitCircleVertices[i + 1].z* topRadius, 0.0));
        gl_Position = projection *  view * model * v1;
        exTextCoord = vec2(u, 1.0);
        exNormal = normal * sideNormals[i + 1];
        fragPos = model * v1;
        EmitVertex(); 
    }
    EndPrimitive();

    // Generating the bottom circle
    for(int i = 0; i <= sectors; ++i)
    {
        gl_Position = projection *  view * model * (bottomPosition);   
        exTextCoord = vec2(0.5, 0.5);
        exNormal = normal * vec3(0,-1,0);
        fragPos = model * (bottomPosition);
        EmitVertex();
        v1 = (bottomPosition + vec4(unitCircleVertices[i].x * bottomRadius, 0 ,unitCircleVertices[i].z * bottomRadius, 0.0));
        gl_Position = projection *  view * model * v1;  
        exTextCoord = vec2(-unitCircleVertices[i].x * 0.5 + 0.5, -unitCircleVertices[i].z * 0.5 + 0.5);
        exNormal = normal * vec3(0,-1,0);
        fragPos = model * v1;
        EmitVertex(); 
        
    }
    EndPrimitive();
}

void main(void)
{
    vec4 bottomPosition = vec4(gl_in[0].gl_Position.xyz, 1.0);
    vec4 topPosition = vec4(gl_in[1].gl_Position.xyz, 1.0);
    
    float bottomRadius = gl_in[0].gl_Position.w * 0.5;
    float topRadius = gl_in[1].gl_Position.w * 0.5;
    
	generateCylinder(bottomPosition, topPosition, bottomRadius, topRadius);
}