#include "Mesh.h"
#include "../utils/ColorRGBA.h"
#include "../utils/OpenGLUtils.h"
#include <iostream>
#include "../math/Vec2.h"
#include "MeshLoader.h"
#include <assert.h>

#define VERTICES 0
#define NORMALS 1
#define COLORS 2
#define TEXTCOORDS 3

Mesh::Mesh() {}

Mesh::Mesh(const Mesh& mesh) {
	vertices = mesh.vertices;
	colors = mesh.colors;
	normals = mesh.normals;
	textCoords = mesh.textCoords;
	indices = mesh.indices;
}

Mesh::Mesh(const std::vector<Vec4>& vertices) : vertices(vertices) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors) : vertices(vertices), colors(colors) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec4>& colors, const std::vector<GLubyte>& indices) :
	vertices(vertices), colors(colors), indices(indices) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals, const std::vector<Vec2>& textCoords) 
	: vertices(vertices), normals(normals), textCoords(textCoords) {}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals) 
	: vertices(vertices), normals(normals) {
}

Mesh::Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec2>& textCoords) 
	: vertices(vertices), textCoords(textCoords) {}

// Deletes all the vbos, vaos and disables the vertex array atributes
Mesh::~Mesh() {

	if (!hasBeenInitialized) {
		return;
	}

	// Bind the the vao so that we can disable the vertex attrib array
	GL_CALL(glBindVertexArray(vaoId));
	GL_CALL(glDisableVertexAttribArray(0));
	GL_CALL(glDisableVertexAttribArray(1));
	GL_CALL(glDeleteBuffers(1, &vboVerticesId));
	if (!normals.empty())
		GL_CALL(glDeleteBuffers(1, &vboNormalsId));
	if (!colors.empty())
		GL_CALL(glDeleteBuffers(1, &vboColorsId));
	if (!textCoords.empty())
		GL_CALL(glDeleteBuffers(1, &vboTextCoordsId));
	if (!indices.empty()) {
		GL_CALL(glDeleteBuffers(1, &eboIndicesId));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	GL_CALL(glDeleteVertexArrays(1, &vaoId));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindVertexArray(0));
}

Mesh& Mesh::operator=(const Mesh& mesh) {
	vertices = mesh.vertices;
	colors = mesh.colors;
	normals = mesh.normals;
	textCoords = mesh.textCoords;
	indices = mesh.indices;
	return *this;
}

// Initializes the vao and vbos, required so that we can change the vertices after creating the mesh
void Mesh::init() {
	if (hasBeenInitialized) {
		return;
	}

	if (!vertices.empty()) {
		GL_CALL(glGenVertexArrays(1, &vaoId));
		GL_CALL(glBindVertexArray(vaoId));
		{
			// Obtaining the number of buffers that need to be created
			GLsizei numberOfBuffers = 1;
			int idxVertices, idxNormals, idxColors, idxTextCoords, idxIndices;
			if (!normals.empty()) {
				idxNormals = numberOfBuffers;
				++numberOfBuffers;
			}
			if (!colors.empty()) {
				idxColors = numberOfBuffers;
				++numberOfBuffers;
			}
			if (!textCoords.empty()) {
				idxTextCoords = numberOfBuffers;
				++numberOfBuffers;
			}
			if (!indices.empty()) {
				idxIndices = numberOfBuffers;
				++numberOfBuffers;
			}

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
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec4), &vertices[0], GL_STATIC_DRAW));
				GL_CALL(glEnableVertexAttribArray(VERTICES));
				GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
			}

			if (!normals.empty()) {
				vboNormalsId = bufferIds[idxNormals];
				// Binding the colors to the second vbo
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId));
				{
					GL_CALL(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3), &normals[0], GL_STATIC_DRAW));
					GL_CALL(glEnableVertexAttribArray(NORMALS));
					GL_CALL(glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0));
				}
			}
			

			if (!colors.empty()) {
				vboColorsId = bufferIds[idxColors];
				// Binding the colors to the second vbo
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboColorsId));
				{
					GL_CALL(glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec4), &colors[0], GL_STATIC_DRAW));
					GL_CALL(glEnableVertexAttribArray(COLORS));
					GL_CALL(glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
				}
			}

			if (!textCoords.empty()) {
				vboTextCoordsId = bufferIds[idxTextCoords];
				// Binding the colors to the second vbo
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboTextCoordsId));
				{
					GL_CALL(glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(Vec2), &textCoords[0], GL_STATIC_DRAW));
					GL_CALL(glEnableVertexAttribArray(TEXTCOORDS));
					GL_CALL(glVertexAttribPointer(TEXTCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0));
				}
			}

			// If this mesh was created with indices then they will be placed in an element array buffer
			if (!indices.empty()) {
				eboIndicesId = bufferIds[idxIndices];
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndicesId));
				{
					GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLubyte), &indices[0], GL_STATIC_DRAW));
				}
			}

			// BufferIds was allocated on the heap therefore we delete it because we no longer need it
			delete[] bufferIds;
		}
		GL_CALL(glBindVertexArray(0));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		if (!indices.empty()) {
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		hasBeenInitialized = true;
	}
}

// Binds the vertex array object with glBindArray
void Mesh::bind() {
	assert(hasBeenInitialized);
	GL_CALL(glBindVertexArray(vaoId));
	hasBeenBound = true;
}
// Unbinds the vertex array object with glBindArray(0)
void Mesh::unBind() {
	GL_CALL(glBindVertexArray(0));
}

// Draws the mesh using glDrawArrays
void Mesh::draw() {
	assert(hasBeenInitialized && hasBeenBound);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (!indices.empty()) {
		GL_CALL(glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_BYTE, (GLvoid*)0));
	}
	else
	{
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size())));
	}
}

// Creates a black square centered in clip space (0,0,0)
Mesh Mesh::square(const float width) {
	Mesh square;

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
Mesh Mesh::rectangle(const float width, const float height) {
	Mesh rectangle;

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
Mesh Mesh::triangle(const float width, const float height) {
	Mesh triangle;

		triangle.vertices = {
			{-width / 2, -height / 2, 0.0f, 1.0f}, // bottom left vertex
			{width / 2, -height / 2, 0.0f, 1.0f}, // bottom right vertex
			{0, height / 2, 0.0f, 1.0f} // top center vertex		
		};

	return triangle;
}

Mesh* Mesh::loadFromFile(const std::string& filePath) {
	return MeshLoader::loadFromFile(filePath);
}

void Mesh::transform(const Mat4& transformation) {
	for (Vec4& vert : vertices) {
		vert = transformation * vert;
	}
}

void Mesh::paint(const Vec4& color) {
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