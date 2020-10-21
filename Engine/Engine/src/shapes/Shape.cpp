#include "Shape.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"

#define VERTICES 0
#define COLORS 1


Shape::Shape() {}

Shape::Shape(std::vector<Vec4>& vertices, std::vector<Vec4>& colors) : vertices(vertices), colors(colors)
{
	
}

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
	glDeleteVertexArrays(1, &vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// Initializes the vao and vbos, required so that we can change the vertices after creating the shape
void Shape::init() {

	if (hasBeenInitialized) {
		std::cout << "Shape has already been initialized" << std::endl;
		return;
	}

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	{
		GLuint vbos[2];
		glGenBuffers(2, vbos);

		vboVerticesId = vbos[0];
		vboColorsId = vbos[1];

		// Binding the vertices to the first vbo
		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
		{
			// The spec ensures that vectors store their elements contiguously
			// https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec4), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0);
		}
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		}*/

		// Binding the colors to the second vbo
		glBindBuffer(GL_ARRAY_BUFFER, vboColorsId);
		{
			glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec4), &colors[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkForOpenGLErrors("failed to initialize shape");
	hasBeenInitialized = true;
}

// Binds the vertex array object with glBindArray
void Shape::bind() {
	if (!hasBeenInitialized) {
		std::cout << "Cannot bind shape if it has not been initialized" << std::endl;
		return;
	}
	glBindVertexArray(vaoId);
	hasBeenBound = true;
}
// Unbinds the vertex array object with glBindArray(0)
void Shape::unBind() {
	glBindVertexArray(0);
}

// Draws the shape using glDrawArrays
void Shape::draw() {
	if (!hasBeenInitialized || !hasBeenBound) {
		std::cout << "Cannot draw shape if it has not been initialized and bound" << std::endl;
	}

	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
}

// Creates a white square centered in clip space (0,0,0)
Shape Shape::square(const float width) {
	std::vector<Vec4> vertices = {
		// first triangle
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -width / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, width / 2, 0.0f, 1.0f}, // top right vertex
		// second triangle
		{-width / 2, width / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -width / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, width / 2, 0.0f, 1.0f} // top right vertex
	};
	std::vector<Vec4> colors = {
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE
	};
	return Shape(vertices, colors);
}

// Creates a white rectangle centered in clip space (0,0,0)
Shape Shape::rectangle(const float width, const float height) {

	std::vector<Vec4> vertices = {
		// first triangle
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{width / 2, height / 2, 0.0f, 1.0f}, // top right vertex
		// second triangle
		{-width / 2, height / 2, 0.0f, 1.0f}, // top left vertex
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, height / 2, 0.0f, 1.0f} // top right vertex
	};
	std::vector<Vec4> colors = {
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE
	};
	return Shape(vertices, colors);

}

// Creates a white triangle centered in clip space (0,0,0)
Shape Shape::triangle(const float width, const float height) {
	std::vector<Vec4> vertices = {	
		{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
		{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
		{0, height / 2, 0.0f, 1.0f} // top center vertex		
	};
	std::vector<Vec4> colors = {
		ColorRGBA::WHITE,
		ColorRGBA::WHITE,
		ColorRGBA::WHITE
	};
	return Shape(vertices, colors);
}
