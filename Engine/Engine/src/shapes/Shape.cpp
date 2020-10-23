#include "Shape.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"
#include <iostream>

#define VERTICES 0
#define COLORS 1

Shape::Shape() {}

Shape::Shape(const Shape& shape) {
	vertices = shape.vertices;
	colors = shape.colors;

	if (shape.hasIndices) {
		indices = shape.indices;
		hasIndices = true;
	}
}

Shape::Shape(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors) : vertices(vertices), colors(colors) {}

Shape::Shape(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors, const std::vector<GLubyte>& indices) :
	vertices(vertices), colors(colors), indices(indices), hasIndices(true) {}

// Deletes all the vbos, vaos and disables the vertex array atributes
Shape::~Shape() {

	if (!hasBeenInitialized) {
		return;
	}

	// Bind the the vao so that we can disable the vertex attrib array
	GL_CALL(glBindVertexArray(vaoId));
	GL_CALL(glDisableVertexAttribArray(0));
	GL_CALL(glDisableVertexAttribArray(1));
	GL_CALL(glDeleteBuffers(1, &vboVerticesId));
	if (colors.size() > 0)
		GL_CALL(glDeleteBuffers(1, &vboColorsId));
	if (hasIndices) {
		GL_CALL(glDeleteBuffers(1, &eboIndicesId));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	GL_CALL(glDeleteVertexArrays(1, &vaoId));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindVertexArray(0));
}

Shape& Shape::operator=(const Shape& shape) {
	vertices = shape.vertices;
	colors = shape.colors;
	if (shape.hasIndices) {
		indices = shape.indices;
		hasIndices = true;
	}
	return *this;
}

// Initializes the vao and vbos, required so that we can change the vertices after creating the shape
void Shape::init() {
	if (hasBeenInitialized) {
		std::cout << "Shape has already been initialized" << std::endl;
		return;
	}

	if (!vertices.empty()) {
		GL_CALL(glGenVertexArrays(1, &vaoId));
		GL_CALL(glBindVertexArray(vaoId));
		{
			// Obtaining the number of buffers that need to be created
			GLsizei numberOfBuffers = 1;
			if (!colors.empty())
				++numberOfBuffers;
			if (!indices.empty())
				++numberOfBuffers;

			// Allocated on the heap because the numberOfBuffers is only known on run-time
			GLuint* bufferIds = new GLuint[numberOfBuffers];
			
			// Generating all buffers at once, its better than generating each of them separately 
			GL_CALL(glGenBuffers(numberOfBuffers, bufferIds));

			vboVerticesId = bufferIds[0];
			// Binding the vertices to the first vbo
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId));
			{
				// The spec ensures that vectors store their elements contiguously
				// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(VERTICES));
				GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
			}

			// If this shape was created with indices then they will be placed in an element array buffer
			if (hasIndices) {
				eboIndicesId = bufferIds[1];
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesId));
				{
					GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vec4), &indices[0], GL_STATIC_DRAW));
				}
			}

			if (!colors.empty()) {
				vboColorsId = bufferIds[numberOfBuffers - 1];
				// Binding the colors to the second vbo
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboColorsId));
				{
					GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec4), &colors[0], GL_STATIC_DRAW));
					GL_CALL(glEnableVertexAttribArray(COLORS));
					GL_CALL(glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
				}
			}

			// BufferIds was allocated on the heap therefore we delete it because we no longer need it
			delete[] bufferIds;
		}
		GL_CALL(glBindVertexArray(0));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		if (hasIndices) {
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		hasBeenInitialized = true;
	}
}

// Binds the vertex array object with glBindArray
void Shape::bind() {
	if (!hasBeenInitialized) {
		std::cout << "Cannot bind shape if it has not been initialized" << std::endl;
		return;
	}
	GL_CALL(glBindVertexArray(vaoId));
	hasBeenBound = true;
}
// Unbinds the vertex array object with glBindArray(0)
void Shape::unBind() {
	GL_CALL(glBindVertexArray(0));
}

// Draws the shape using glDrawArrays
void Shape::draw() {
	if (!hasBeenInitialized || !hasBeenBound) {
		std::cout << "Cannot draw shape if it has not been initialized and bound" << std::endl;
		return;
	}

	if (hasIndices) {
		GL_CALL(glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_BYTE, (GLvoid*)0));
	}
	else
	{
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size())));
	}
}

// Creates a black square centered in clip space (0,0,0)
Shape Shape::square(const float width) {
	Shape square;
	square.vertices = {
		// first triangle
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -width / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, width / 2, 0.0f, 1.0f}, // top right vertex
		// second triangle
		{-width / 2, width / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, width / 2, 0.0f, 1.0f} // top right vertex
	};
	return square;
}

// Creates a black rectangle centered in clip space (0,0,0)
Shape Shape::rectangle(const float width, const float height) {
	Shape rectangle;
	rectangle.vertices = {
		// first triangle
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, height / 2, 0.0f, 1.0f}, // top right vertex
		// second triangle
		{-width / 2, height / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, height / 2, 0.0f, 1.0f} // top right vertex
	};
	return rectangle;

}

// Creates a black triangle centered in clip space (0,0,0)
Shape Shape::triangle(const float width, const float height) {
	Shape triangle;
	triangle.vertices = {	
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{0, height / 2, 0.0f, 1.0f} // top center vertex		
	};
	return triangle;
}

void Shape::transform(const Mat4& transformation) {
	for (Vec4& vert : vertices) {
		vert = transformation * vert;
	}
}

void Shape::paint(const Vec4& color) {
	if (colors.empty()) {
		int colorSize = int(vertices.size());

		for (int i = 0; i < colorSize; i++) {
			colors.push_back(color);
		}
	}
	else {
		for (Vec4& vec : colors) {
			vec = color;
		}
	}
}