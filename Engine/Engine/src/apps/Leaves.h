#ifndef LEAVES_H
#define LEAVES_H

#include "../meshes/Mesh.h"
#include "../textures/Texture2D.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"

struct Leaves {

	Mesh* quad;
	Texture2D* texture;
	ShaderProgram* shader;

	std::vector<SceneNode*> sceneNodes;

	Leaves();
	void removeLeaves();
};

#endif
