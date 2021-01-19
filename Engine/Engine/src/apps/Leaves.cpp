#include "Leaves.h"

Leaves::Leaves(DepthMap* depthMap, ShaderProgram* shader, ShaderProgram* shaderSM) : depthMap(depthMap), shader(shader), shaderSM(shaderSM)
{
	quad = Mesh::loadFromFile("../Engine/objs/quad.obj");
	texture = new Texture2D("../Engine/textures/tree/leaves.png");
}

Leaves::~Leaves() {
	delete quad;
	delete texture;
	delete shader;
	delete shaderSM;
}

void Leaves::removeLeaves()
{
	for (SceneNode* sn : sceneNodes) {
		sn->deleteSceneNode();
	}
	sceneNodes.clear();
}
