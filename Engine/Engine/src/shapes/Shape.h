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
	std::vector<GLubyte> indices;

private:
	GLuint vaoId;
	GLuint vboVerticesId;
	GLuint vboColorsId;
	GLuint eboIndicesId; // Is the Id of the GL_ELEMENT_ARRAY_BUFFER, that contains the order in which the vertices should be drawn
	bool hasBeenInitialized = false;
	bool hasBeenBound = false;
	bool withIndices = false;

public:

	Shape();

	Shape(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors);

	Shape(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors,const std::vector<GLubyte>& indices);

	// Deletes all the vbos, vaos and disables the vertex array atributes
	~Shape() override;


	// Initializes the vao and vbo, required so that we can change the vertices after creating the shape
	void init() override;

	// Binds the vertex array object with glBindArray
	void bind() override;
	// Unbinds the vertex array object with glBindArray(0)
	void unBind() override;

	// Draws the shape using glDrawArrays
	void draw() override;

	// Creates a black centered in clip space (0,0,0)
	static Shape square(const float width = 0.75);

	// Creates a black centered in clip space (0,0,0)
	static Shape rectangle(const float width=1, const float height=0.75);

	// Creates a black centered in clip space (0,0,0)
	static Shape triangle(const float width=1, const float height=1);

	void transform(const Mat4& transformation);
	void paint(const Vec4& color);
};

#endif
