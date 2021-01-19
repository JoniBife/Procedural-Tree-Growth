#ifndef LEAVES_H
#define LEAVES_H

#include "../meshes/Mesh.h"
#include "../textures/Texture2D.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../textures/DepthMap.h"

struct Leaves {

	Mesh* quad;
	Texture2D* texture;
	DepthMap* depthMap;
	ShaderProgram* shader;
	ShaderProgram* shaderSM; // Used for shadow mapping

	std::vector<SceneNode*> sceneNodes;

	Leaves(DepthMap* depthMap, ShaderProgram* shader, ShaderProgram* shaderSM);
	~Leaves();
	void removeLeaves();
};

#endif
