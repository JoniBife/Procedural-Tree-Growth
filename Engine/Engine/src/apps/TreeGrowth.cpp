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
static Texture2D* woodTextureNormalMap;

void ComputeTangentsBitangents(std::vector<Vec4>& vertices,
	std::vector<Vec2>& textCoords,
	std::vector<Vec3>& normals,
	std::vector<Vec3>& tangents,
	std::vector<Vec3>& bitangents) {

	for (int i = 0; i < vertices.size(); i += 3) {
		// Shortcuts for vertices
		Vec4& v0 = vertices[i + 0];
		Vec4& v1 = vertices[i + 1];
		Vec4& v2 = vertices[i + 2];

		// Shortcuts for UVs
		Vec2& uv0 = textCoords[i + 0];
		Vec2& uv1 = textCoords[i + 1];
		Vec2& uv2 = textCoords[i + 2];

		// Edges of the triangle : position delta
		Vec4 deltaPos1 = v1 - v0;
		Vec4 deltaPos2 = v2 - v0;

		// UV delta
		Vec2 deltaUV1 = uv1 - uv0;
		Vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vec4 tangentAux = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		Vec4 bitangentAux = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		Vec3 tangent = Vec3(tangentAux.x, tangentAux.y, tangentAux.z);
		Vec3 bitangent = Vec3(bitangentAux.x, bitangentAux.y, bitangentAux.z);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for bitangents
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}


}

void TreeGrowth::start() {
	cube = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");

	std::vector<Vec3> tangents, bitangents;
	
	ComputeTangentsBitangents(cube->vertices, cube->textCoords, cube->normals, cube->tangents, cube->bitangents);

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShader2.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShader2.glsl");
	sp = new ShaderProgram(vs, fs);

	woodTexture = new Texture2D("../Engine/textures/Seamless_tree_bark_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/Seamless_tree_bark_texture_NORMAL.jpg");

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
		sp->setUniform(sp->getUniformLocation("lightPos"), Vec3(1.0f, 2.0f, 2.0f));
		sp->setUniform(sp->getUniformLocation("viewPos"), Vec3(0.0f, 0.0f, 5.0f));
		woodTexture->bind(0);
		woodTextureNormalMap->bind(1);
	});
	node->setAfterDrawFunction([=]() {
		woodTexture->unBind();
		woodTextureNormalMap->unBind();
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


