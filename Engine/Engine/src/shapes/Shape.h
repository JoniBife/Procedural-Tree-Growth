#ifndef SHAPE_2D_H
#define SHAPE_2D_H

#include <vector>
#include <GL/glew.h>
#include "Drawable.h"
#include "../math/Mat4.h"
#include "../math/Vec4.h"

/*
* Usage example:
* 
* std::vector<Vec4> vertices = ...;
* std::vector<Vec4> colors = ...;
* 
* Shape square(vertices,colors);
* 
* square.bind();
* 
* // Do stuff with the shader program (ex: set the uniforms);
* 
* square.draw();
* square.unbind();
* 
* delete square;
* 
*/
class Shape : public Drawable {

private:
	GLuint vaoId;
	GLuint vboId[2]; // First element is the GL_ARRAY_BUFFER and the second is the GL_ELEMENT_ARRAY_BUFFER
	std::vector<Vec4> vertices;
	std::vector<Vec4> colors;

public:

	// Initiates the Shape2D by creating the vao and vbo
	Shape(std::vector<Vec4>& vertices, std::vector<Vec4>& colors);

	// Deletes all the vbos, vaos and disables the vertex array atributes
	~Shape() override;

	// Binds the vertex array object with glBindArray
	void bind() override;
	// Unbinds the vertex array object with glBindArray(0)
	void unBind() override;

	// Draws the shape using glDrawArrays
	void draw() override;
};

#endif
