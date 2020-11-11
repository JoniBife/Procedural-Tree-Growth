#ifndef MESH_GROUP_H
#define MESH_GROUP_H

#include <vector>
#include "IDrawable.h"
#include "Mesh.h"
#include "../math/Mat4.h"

/*
* Usage example:
*
* Mesh a = ...;
* Mesh b = ...;
*
* MeshGroup sg({a,b});
*
* sg.bind()
* 
* // Do stuff with the shader program (ex: set the uniforms);
*
* sg.draw();
* sg.unbind();
*/
class MeshGroup : IDrawable {

private:
	bool hasBeenInitialized = false;

public:
	std::vector<Mesh> shapes;

	MeshGroup(const MeshGroup& shapeGroup);
	MeshGroup(const std::vector<MeshGroup>& shapeGroups);
	MeshGroup(const std::vector<Mesh>& shapes);
	MeshGroup(const std::vector<Mesh>& shapes, const std::vector<MeshGroup>& shapeGroups);

	MeshGroup& operator=(const MeshGroup& shapeGroup);

	// Initialized all shapes
	void init() override;

	// Does nothing, but still has to be implemented due to base class
	void bind() override;

	// Unbinds the shape group with glBindVertexArray(0)
	void unBind() override;

	// Goes through each shape within shapes and binds and draw them
	void draw() override;

	// Applies the transformation to all shapes that compose this shape group
	void transform(const Mat4& transformation);

	// Applies the color to all shapes that compose this shape group
	void paint(const Vec4& color);
};

#endif

