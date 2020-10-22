#ifndef SHAPE_GROUP_H
#define SHAPE_GROUP_H

#include <vector>
#include "Drawable.h"
#include "Shape.h"
#include "../math/Mat4.h"

class ShapeGroup : Drawable {

public:
	std::vector<Shape> shapes;

	ShapeGroup(const ShapeGroup& shapeGroup);
	ShapeGroup(const std::vector<ShapeGroup>& shapeGroups);
	ShapeGroup(const std::vector<Shape>& shapes);
	ShapeGroup(const std::vector<Shape>& shapes, const std::vector<ShapeGroup>& shapeGroups);

	ShapeGroup& operator=(const ShapeGroup& shapeGroup);

	void init() override;

	void bind() override;
	void unBind() override;

	void draw() override;

	void transform(const Mat4& transformation);

	void paint(const Vec4& color);
};

#endif

