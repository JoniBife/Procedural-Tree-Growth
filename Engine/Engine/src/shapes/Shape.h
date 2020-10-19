#ifndef SHAPE_2D_H
#define SHAPE_2D_H

#include <vector>
#include <functional>
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

public:
	std::vector<Vec4> vertices;
	std::vector<Vec4> colors;

private:
	GLuint vaoId;
	GLuint vboVerticesId;
	GLuint eboVerticesId; // Is the Id of the GL_ELEMENT_ARRAY_BUFFER, that contains the order in which the vertices should be drawn
	GLuint vboColorsId;

public:

	Shape();

	Shape(std::vector<Vec4>& vertices, std::vector<Vec4>& colors);

	// Deletes all the vbos, vaos and disables the vertex array atributes
	~Shape() override;

	// Initializes the vao and vbo, required so that we can change the vertices after creating the shape
	void init();

	// Binds the vertex array object with glBindArray
	void bind() override;
	// Unbinds the vertex array object with glBindArray(0)
	void unBind() override;

	// Draws the shape using glDrawArrays
	void draw() override;

	// Creates a square centered in clip space (0,0,0)
	static Shape square(const float width);

	// Creates a rectangle centered in clip space (0,0,0)
	static Shape rectangle(const float width, const float height);

	// Creates a triangle centered in clip space (0,0,0)
	static Shape triangle(const float width, const float height);
};

#endif
