#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <iostream>
#include "../shaders/ShaderProgram.h"
#include "../view/Camera.h"
#include "../meshes/Mesh.h"
#include "../math/Mat4.h"

class SceneNode {

public:
	SceneNode();

	SceneNode(Mesh* mesh, const Mat4& model, SceneNode* parent, ShaderProgram* shaderProgram);

	SceneNode* createChild();
	SceneNode* createChild(Mesh* mesh, const Mat4& model);
	SceneNode* createChild(Mesh* mesh, const Mat4& model, ShaderProgram* shaderProgram);

	void setModel(const Mat4& model);
	void setShaderProgram(ShaderProgram* shaderProgram);

	Mat4 getModel();
	Mesh* getMesh();
	ShaderProgram* getShaderProgram();

	void init();

	void draw();

private:
	Mesh* mesh = nullptr;
	Mat4 model = Mat4::IDENTITY;
	GLint modelUniformLocation = GLint(-1);
	ShaderProgram* shaderProgram = nullptr;
	SceneNode* parent = nullptr;
	std::vector<SceneNode*> children;

};

class SceneGraph {

private:
	Camera* camera;
	SceneNode* root;

public:

	SceneGraph(Camera* camera);

	~SceneGraph();

	void init();

	void draw();

	SceneNode* createChild();

	SceneNode* getRoot();

	Camera* getCamera();
};

#endif
