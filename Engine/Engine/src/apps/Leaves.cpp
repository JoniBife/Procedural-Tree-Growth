#include "Leaves.h"

Leaves::Leaves()
{
	quad = Mesh::loadFromFile("../Engine/objs/quad.obj");

	texture = new Texture2D("../Engine/textures/tree/leaves.png");

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderBlending.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderBlending.glsl");

	shader = new ShaderProgram(vs, fs);
}

void Leaves::removeLeaves()
{
	for (SceneNode* sn : sceneNodes) {
		sn->deleteSceneNode();
	}
	sceneNodes.clear();
}
