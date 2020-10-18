#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <iostream>
#include <GL/glew.h>
#include "Shader.h"
#include "../math/Vec2.h"
#include "../math/Vec3.h"
#include "../math/Vec4.h"
#include "../math/Mat2.h"
#include "../math/Mat3.h"
#include "../math/Mat4.h"

/*
* Usage example:
*
* // Creating a vertex shader
* Shader vs(GL_VERTEX_SHADER, "C:/Dev/CGJ-AVT/Engine/Engine/shaders/vertexShader.glsl");
* 
* // Creating a fragment shader
* Shader fs(GL_FRAGMENT_SHADER, "C:/Dev/CGJ-AVT/Engine/Engine/shaders/fragmentShader.glsl");
* 
* // Creating a shader program
* ShaderProgram sp(vs, fs);
* 
* // Shaders are no longer necessary since they have been linked to the shader program
* delete vs;
* delete fs;
* 
* // Using the shader program
* sp.use();
* 
* // Setting the uniforms
* sp.setUniform("scale", 0.5f);
* sp.setUniform("color", Vec4(1.0f, 0.0f, 0.0f, 1.0f));
* 
* // DRAW STUFF HERE
* 
* // Stop using the shader program
* sp.stopUsing();
*
* // No longer need the shader program so delete it
* delete sp;
*/
class ShaderProgram {

private:
	GLuint shaderProgramId;
	Shader& vertexShader;
	Shader& fragmentShader;

public:
	// In the future we should add other constructors to support other types of shaders
	ShaderProgram(Shader& vertexShader, Shader& fragmentShader);
	~ShaderProgram();

	void use();
	void stopUsing();

	// All theses methods se the value of a specific uniform
	void setUniform(const GLint location, const GLboolean value);
	void setUniform(const GLint location, const GLint value);
	void setUniform(const GLint location, const GLfloat value);
	void setUniform(const GLint location, const Vec2& value);
	void setUniform(const GLint location, const Vec3& value);
	void setUniform(const GLint location, const Vec4& value);
	void setUniform(const GLint location, const Mat2& value);
	void setUniform(const GLint location, const Mat3& value);
	void setUniform(const GLint location, const Mat4& value);

	// Used to obtain the location of uniforms, this way we avoid doing this in every setUniform
	GLint getUniformLocation(const GLchar* name) const;

	// Output, prints the id
	friend std::ostream& operator<<(std::ostream& os, const ShaderProgram& sp);

private: 
	void attachShaders();
	void detachShaders();
};

#endif
