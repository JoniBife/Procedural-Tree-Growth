#include "../utils/OpenGLUtils.h"
#include "MeshGroup.h"

MeshGroup::MeshGroup(const MeshGroup& shapeGroup) {
	shapes = shapeGroup.shapes;
}

MeshGroup::MeshGroup(const std::vector<MeshGroup>& shapeGroups) {
	for (MeshGroup sg : shapeGroups) {
		for (Mesh sp : sg.shapes) {
			this->shapes.push_back(sp);
		}
	}
}

MeshGroup::MeshGroup(const std::vector<Mesh>& shapes) : shapes(shapes) {}

MeshGroup::MeshGroup(const std::vector<Mesh>& shapes, const std::vector<MeshGroup>& shapeGroups) : shapes(shapes) {
	for (MeshGroup sg : shapeGroups) {
		for (Mesh sp : sg.shapes) {
			this->shapes.push_back(sp);
		}
	}
}

MeshGroup& MeshGroup::operator=(const MeshGroup& shapeGroup) {
	shapes = shapeGroup.shapes;
	return *this;
}

void MeshGroup::init() {
	for (Mesh& shape : shapes) {
		shape.init();
	}
}

void MeshGroup::bind() {
	// Does nothing
}

void MeshGroup::unBind() {
	GL_CALL(glBindVertexArray(0));
}

void MeshGroup::draw() {
	for (Mesh& shape : shapes) {
		shape.bind();
		shape.draw();
	}
}

void MeshGroup::transform(const Mat4& transformation) {
	for (Mesh& shape : shapes) {
		shape.transform(transformation);
	}
}

void MeshGroup::paint(const Vec4& color) {
	for (Mesh& shape : shapes) {
		shape.paint(color);
	}
}