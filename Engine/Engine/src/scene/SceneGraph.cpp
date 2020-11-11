#include "SceneGraph.h"

///////////////////////////////// SCENE NODE
SceneNode::SceneNode() {}

SceneNode::SceneNode(Mesh* mesh, const Mat4& model, SceneNode* parent, ShaderProgram* shaderProgram)
	: mesh(mesh), model(model), parent(parent), shaderProgram(shaderProgram) {}

SceneNode* SceneNode::createChild() {
	SceneNode* child = new SceneNode();
	child->parent = this;
	child->shaderProgram = this->shaderProgram;
	children.push_back(child);
	return child;
}

SceneNode* SceneNode::createChild(Mesh* mesh, const Mat4& model) {
	SceneNode* child = new SceneNode(mesh, this->model * model, this, this->shaderProgram);
	children.push_back(child);
	return child;
}

SceneNode* SceneNode::createChild(Mesh* mesh, const Mat4& model, ShaderProgram* shaderProgram) {
	// TODO What to do with the shaderProgram
	SceneNode* child = new SceneNode(mesh, this->model * model, this, shaderProgram);
	children.push_back(child);
	return child;
}

void SceneNode::setModel(const Mat4& model) {
	if (this->parent != nullptr)
		this->model = model * this->parent->model;
	this->model = model;
}

void SceneNode::setShaderProgram(ShaderProgram* shaderProgram) {
	this->shaderProgram = shaderProgram;
	// TODO WRONG
}

Mat4 SceneNode::getModel() {
	return model;
}
Mesh* SceneNode::getMesh() {
	return mesh;
}
ShaderProgram* SceneNode::getShaderProgram() {
	return shaderProgram;
}

void SceneNode::init() {
	if (mesh != nullptr) {
		mesh->init();
		modelUniformLocation = shaderProgram->getUniformLocation("model");
	}
	for (SceneNode* child : children) {
		child->init();
	}
}

void SceneNode::draw() {
	if (mesh != nullptr) {
		shaderProgram->use();
		mesh->bind();
		shaderProgram->setUniform(modelUniformLocation, model);
		mesh->draw();
		mesh->unBind();
		shaderProgram->stopUsing();
	}
	for (SceneNode* child : children) {
		child->draw();
	}
}

////////////////////////////////// SCENE GRAPH

SceneGraph::SceneGraph(Camera* camera) : root(new SceneNode()), camera(camera) {}

SceneGraph::~SceneGraph() {
	delete root;
}

void SceneGraph::init() {
	root->init();
}

void SceneGraph::draw() {
	camera->update();
	root->draw();
}

SceneNode* SceneGraph::createChild() {
	return root->createChild();
}

SceneNode* SceneGraph::getRoot() {
	return root;
}

Camera* SceneGraph::getCamera() {
	return camera;
}