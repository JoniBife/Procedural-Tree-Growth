#include "Shape.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"

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
		std::cout << "Shape has not been initialized!" << std::endl;
		return;
	}

	// Bind the the vao so that we can disable the vertex attrib array
	glBindVertexArray(vaoId);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteBuffers(1, &vboVerticesId);
	glDeleteBuffers(1, &vboColorsId);
	if (hasIndices) {
		glDeleteBuffers(1, &eboIndicesId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glDeleteVertexArrays(1, &vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		{
			// Todo generate all buffers at once
			glGenBuffers(1, &vboVerticesId);
			// Binding the vertices to the first vbo
			glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
			{
				// The spec ensures that vectors store their elements contiguously
				// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(VERTICES);
				glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0);
			}

			// If this shape was created with indices then they will be placed in an element array buffer
			if (hasIndices) {
				glGenBuffers(1, &eboIndicesId);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesId);
				{
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vec4), &indices[0], GL_STATIC_DRAW);
				}
			}

			if (!colors.empty()) {
				glGenBuffers(1, &vboColorsId);
				// Binding the colors to the second vbo
				glBindBuffer(GL_ARRAY_BUFFER, vboColorsId);
				{
					glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec4), &colors[0], GL_STATIC_DRAW);
					glEnableVertexAttribArray(COLORS);
					glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0);
				}
			}
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (hasIndices)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		checkForOpenGLErrors("failed to initialize shape");
		hasBeenInitialized = true;
	}
}

// Binds the vertex array object with glBindArray
void Shape::bind() {
	if (!hasBeenInitialized) {
		std::cout << "Cannot bind shape if it has not been initialized" << std::endl;
		return;
	}
	glBindVertexArray(vaoId);
	checkForOpenGLErrors("failed to bind shape");
	hasBeenBound = true;
}
// Unbinds the vertex array object with glBindArray(0)
void Shape::unBind() {
	glBindVertexArray(0);
	checkForOpenGLErrors("failed to unBind shape");
}

// Draws the shape using glDrawArrays
void Shape::draw() {
	if (!hasBeenInitialized || !hasBeenBound) {
		std::cout << "Cannot draw shape if it has not been initialized and bound" << std::endl;
	}

	if(hasIndices)
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, (GLvoid*)0);
	else
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

	checkForOpenGLErrors("failed to draw shape");
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
		float colorSize = vertices.size();

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