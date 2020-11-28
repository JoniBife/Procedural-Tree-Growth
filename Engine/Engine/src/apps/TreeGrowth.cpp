#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeGrowth.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/OrbitCameraController.h"
#include "../textures/Texture2D.h"
#include "../utils/ColorRGBA.h"

static Mesh* cube;
static ShaderProgram* sp;
static ICameraController* cameraController;
static Texture2D* woodTexture;

void TreeGrowth::start() {
	cube = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderCGJ.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderCGJ.glsl");
	sp = new ShaderProgram(vs, fs);

	woodTexture = new Texture2D("../Engine/textures/Seamless_tree_bark_texture.jpg");

	// Uniform buffer object binding point
	const GLuint uboBp = 0;

	// Associating the shared matrix index with the binding point 0
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, uboBp);

	cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	
	getCamera()->addCameraController(cameraController);

	getSceneGraph()->getRoot()->setShaderProgram(sp);
	SceneNode* node = getSceneGraph()->getRoot()->createChild(cube, Mat4::IDENTITY);
	node->setBeforeDrawFunction([=](ShaderProgram* sp) {
		//sp->setUniform(sp->getUniformLocation("color"), ColorRGBA::BLUE);
		woodTexture->bind(0);
	});
	node->setAfterDrawFunction([=]() {
		woodTexture->unBind();
	});
};

void TreeGrowth::update() {
	cameraController->processInputAndMove(getElapsedTime());
};

void TreeGrowth::end() {
	delete cameraController;
	delete sp;
	delete cube;
	delete woodTexture;
};