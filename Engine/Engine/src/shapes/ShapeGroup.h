#ifndef SHAPE_GROUP_H
#define SHAPE_GROUP_H

#include <vector>
#include "Drawable.h"
#include "Shape.h"
#include "../math/Mat4.h"

/*
* Usage example:
*
* Shape a = ...;
* Shape b = ...;
*
* ShapeGroup sg({a,b});
*
* sg.bind()
* 
* // Do stuff with the shader program (ex: set the uniforms);
*
* sg.draw();
* sg.unbind();
*/
class ShapeGroup : Drawable {

private:
	bool hasBeenInitialized = false;

public:
	std::vector<Shape> shapes;

	ShapeGroup(const ShapeGroup& shapeGroup);
	ShapeGroup(const std::vector<ShapeGroup>& shapeGroups);
	ShapeGroup(const std::vector<Shape>& shapes);
	ShapeGroup(const std::vector<Shape>& shapes, const std::vector<ShapeGroup>& shapeGroups);

	ShapeGroup& operator=(const ShapeGroup& shapeGroup);

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

