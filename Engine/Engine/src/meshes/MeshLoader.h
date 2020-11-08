#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <iostream>
#include "Mesh.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../math/Vec2.h"

class MeshLoader {

public:
	static Mesh loadFromFile(const std::string& filePath);

	static Mesh createMeshFromStream(meshStream);

private:

	static std::vector<Vec4> parseVertex(std::stringstream& sin);

	static std::vector<Vec3> parseNormal(std::stringstream& sin);

	static std::vector<Vec2> parseTextCoord(std::stringstream& sin);
};
#endif
