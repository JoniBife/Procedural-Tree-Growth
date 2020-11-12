#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h>
#include "IDrawable.h"
#include "../math/Mat4.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../math/Vec2.h"

/*
* Usage example:
* 
* std::vector<Vec4> vertices = ...;
* std::vector<Vec4> colors = ...;
* 
* Mesh square(vertices,colors);
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
class Mesh : public IDrawable {

public:
	std::vector<Vec4> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec4> colors;
	std::vector<Vec2> textCoords;
	std::vector<GLubyte> indices;

private:
	GLuint vaoId = GLuint(0);
	GLuint vboVerticesId = GLuint(0);
	GLuint vboNormalsId = GLuint(0);
	GLuint vboColorsId = GLuint(0);
	GLuint vboTextCoordsId = GLuint(0);
	GLuint eboIndicesId = GLuint(0);
	bool hasBeenInitialized = false;
	bool hasBeenBound = false;

public:

	Mesh();

	// Copy constructor
	Mesh(const Mesh& mesh);

	Mesh(const std::vector<Vec4>& vertices);

	Mesh(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors);

	Mesh(const std::vector<Vec4>& vertices,const std::vector<Vec4>& colors,const std::vector<GLubyte>& indices);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals, const std::vector<Vec2>& textCoords);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec3>& normals);

	Mesh(const std::vector<Vec4>& vertices, const std::vector<Vec2>& textCoords);

	// Deletes all the vbos, vaos and disables the vertex array atributes
	~Mesh() override;

	// Assignment
	Mesh& operator=(const Mesh& mesh);

	// Initializes the vao and vbo, required so that we can change the vertices after creating the mesh
	void init() override;

	// Binds the vertex array object with glBindArray
	void bind() override;
	// Unbinds the vertex array object with glBindArray(0)
	void unBind() override;

	// Draws the mesh using glDrawArrays
	void draw() override;

	// Creates a black centered in clip space (0,0,0)
	static Mesh square(const float width = 0.75f);

	// Creates a black centered in clip space (0,0,0)
	static Mesh rectangle(const float width = 1.0f, const float height = 0.75f);

	// Creates a black centered in clip space (0,0,0)
	static Mesh triangle(const float width = 1.0f, const float height = 1.0f);

	static Mesh* loadFromFile(const std::string& filePath);

	void transform(const Mat4& transformation);
	void paint(const Vec4& color);
};

#endif
