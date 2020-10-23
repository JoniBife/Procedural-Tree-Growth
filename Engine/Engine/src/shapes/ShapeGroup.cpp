#include "../utils/OpenGLUtils.h"
#include "ShapeGroup.h"

ShapeGroup::ShapeGroup(const ShapeGroup& shapeGroup) {
	shapes = shapeGroup.shapes;
}

ShapeGroup::ShapeGroup(const std::vector<ShapeGroup>& shapeGroups) {
	for (ShapeGroup sg : shapeGroups) {
		for (Shape sp : sg.shapes) {
			this->shapes.push_back(sp);
		}
	}
}

ShapeGroup::ShapeGroup(const std::vector<Shape>& shapes) : shapes(shapes) {}

ShapeGroup::ShapeGroup(const std::vector<Shape>& shapes, const std::vector<ShapeGroup>& shapeGroups) : shapes(shapes) {
	for (ShapeGroup sg : shapeGroups) {
		for (Shape sp : sg.shapes) {
			this->shapes.push_back(sp);
		}
	}
}

ShapeGroup& ShapeGroup::operator=(const ShapeGroup& shapeGroup) {
	shapes = shapeGroup.shapes;
	return *this;
}

void ShapeGroup::init() {
	for (Shape& shape : shapes) {
		shape.init();
	}
}

void ShapeGroup::bind() {
	// Does nothing
}

void ShapeGroup::unBind() {
	GL_CALL(glBindVertexArray(0));
}

void ShapeGroup::draw() {
	for (Shape& shape : shapes) {
		shape.bind();
		shape.draw();
	}
}

void ShapeGroup::transform(const Mat4& transformation) {
	for (Shape& shape : shapes) {
		shape.transform(transformation);
	}
}

void ShapeGroup::paint(const Vec4& color) {
	for (Shape& shape : shapes) {
		shape.paint(color);
	}
}