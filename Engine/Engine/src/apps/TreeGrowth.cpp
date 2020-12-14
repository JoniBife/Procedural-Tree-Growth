#define _USE_MATH_DEFINES
#include <math.h>
#include "TreeGrowth.h"
#include "../meshes/Mesh.h"
#include "../shaders/ShaderProgram.h"
#include "../scene/SceneGraph.h"
#include "../view/Transformations.h"
#include "../controllers/SphericalCameraController.h"
#include "../controllers/FreeCameraController.h"
#include "../textures/Texture2D.h"
#include "../utils/ColorRGBA.h"
#include "BranchModule.h"
#include "GrowthParameters.h"
#include "Equations.h"

static Mesh* cylinder;
static ShaderProgram* sp;
static FreeCameraController* cameraController;
static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static GrowthParameters* growthParameters;
static BranchModule* module;

static GLint cameraPosL;
static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

static float currTime = 0.0f;

static float scaleLength = 1.5f;

BranchNode* createBranch(SceneGraph* sceneGraph, BranchNode* parent, Vec3 orientation)
{
	BranchNode* child = parent->createChild(orientation * scaleLength);
	child->sceneGraphNode = sceneGraph->getRoot()->createChild(cylinder, Mat4::ZERO);
	child->growthParameters = growthParameters;
	child->sceneGraphNode->addTexture(woodTexture);
	child->sceneGraphNode->addTexture(woodTextureNormalMap);

	return child;
}

void setupTree(SceneGraph* sceneGraph) {
	growthParameters = new GrowthParameters();
	growthParameters->gP = 0.12f;
	growthParameters->scalingCoefficient = 1.29f;
	growthParameters->vRootMax = 900;
	growthParameters->thickeningFactor = 0.5f; // original 1.41
	float vMin = 0;

	module = new BranchModule();
	module->growthRate = eqt::growthRate(growthParameters->vRootMax, vMin, growthParameters->vRootMax, growthParameters->gP);
	module->root = new BranchNode();
	module->root->relativePosition = { 0.0f, -10.0f, 0.0f };

	// module 1
	/*/
	BranchNode* child = createBranch(sceneGraph, module->root, { 0.0f, 18.0f, 0.0f });

	{

		BranchNode* childA = createBranch(sceneGraph, child, { 9.0f, 9.0f, -4.5f });
		{
			BranchNode* childA1 = createBranch(sceneGraph, childA, { 7.0f, 4.0f, 2.0f });
			{
				BranchNode* childA1a = createBranch(sceneGraph, childA1, { 2.0f, 2.0f, -1.0f });

				BranchNode* childA1b = createBranch(sceneGraph, childA1, { 4.0f, 1.0f, 0.0f });
				{
					BranchNode* childA1b1 = createBranch(sceneGraph, childA1b, { 2.0f, -0.5f, 1.0f });
					BranchNode* childA1b2 = createBranch(sceneGraph, childA1b, { 2.0f, 3.0f, -1.0f });

				}
			}

			BranchNode* childA2 = createBranch(sceneGraph, childA, { 2.0f, 7.0f, -3.0f });
			{
				BranchNode* childA2a = createBranch(sceneGraph, childA2, { 0.5f, 4.0f, -2.0f });

			}

		}

		BranchNode* childB = createBranch(sceneGraph, child, { 0.0f, 12.0f, 0.0f });
		{
			BranchNode* childB1 = createBranch(sceneGraph, childB, { 6.0f, 12.0f, 2.0f });
			{
				BranchNode* childB1a = createBranch(sceneGraph, childB1, { 2.0f, 3.5f,0.0f });
				{
					BranchNode* childB1a1 = createBranch(sceneGraph, childB1a, { 1.0f, 6.0f, 0.0f });
					{
						BranchNode* childB1a1a = createBranch(sceneGraph, childB1a1, { 1.5f, 3.0f,0.0f });
						BranchNode* childB1a1b = createBranch(sceneGraph, childB1a1, { -1.0f, 3.0f,0.0f });

					}
				}

				BranchNode* childB1b = createBranch(sceneGraph, childB1, { -0.50f, 3.5f,0.0f });
				{
					BranchNode* childB1b1 = createBranch(sceneGraph, childB1b, { 0.0f, 4.0f,0.0f });

				}
			}

			BranchNode* childB2 = createBranch(sceneGraph, childB, { -4.0f, 6.0f, -2.0f });
			{
				BranchNode* childB2a = createBranch(sceneGraph, childB2, { -3.0f, 6.0f, -2.0f });

				BranchNode* childB2b = createBranch(sceneGraph, childB2, { -1.5f, 1.0f,1.0f });
				{
					BranchNode* childB21 = createBranch(sceneGraph, childB2b, { 0.5f, 2.0f,0.5f });
					BranchNode* childB22 = createBranch(sceneGraph, childB2b, { -2.0f, -1.0f, 1.0f });
				}
			}
		}
		
		BranchNode* childC = createBranch(sceneGraph, child, { -9.0f, 9.0f, -5.0f });
		{
			BranchNode* childC1 = createBranch(sceneGraph, childC, { -3.0f, 12.0f,0.0f });
			{
				BranchNode* childC1a = createBranch(sceneGraph, childC1, { 0.0f, 3.0f,0.0f });
				BranchNode* childC1b = createBranch(sceneGraph, childC1, { -2.0f, 3.5f,0.0f });

			}

			BranchNode* childC2 = createBranch(sceneGraph, childC, { -6.0f, 1.0f,0.0f });
			{
				BranchNode* childC2a = createBranch(sceneGraph, childC2, { -5.0f, 2.0f,0.0f });
				BranchNode* childC2b = createBranch(sceneGraph, childC2, { -2.0f, -1.0f,0.0f });
			}
		}
		
	}
	/**/
	//module 2
	/**/
	BranchNode* child = createBranch(sceneGraph, module->root, { 0.0f, 12.0f, 0.0f });
	{
		BranchNode* childA = createBranch(sceneGraph, child, { 5.0f, 5.0f, -2.0f });
		{
			BranchNode* childA1 = createBranch(sceneGraph, childA, { 3.0f, 1.0f, 1.0f });
			BranchNode* childA2 = createBranch(sceneGraph, childA, { 4.0f, 5.0f, -2.0f });
			BranchNode* childA3 = createBranch(sceneGraph, childA, { 1.5f, 2.5f, -1.5f });
		}

		BranchNode* childB = createBranch(sceneGraph, child, { 0.0f, 7.0f, 0.0f });
		{
			BranchNode* childB1 = createBranch(sceneGraph, childB, { 5.0f, 9.5f, 0.5f });
			BranchNode* childB2 = createBranch(sceneGraph, childB, { 0.0f, 15.0f, 0.0f });
			{
				BranchNode* childB2a = createBranch(sceneGraph, childB2, { 2.0f, 4.5f, 1.5f });
				BranchNode* childB2b = createBranch(sceneGraph, childB2, { 0.0f, 6.5f, 0.0f });
				{
					BranchNode* childB2b1 = createBranch(sceneGraph, childB2b, { 3.5f, 6.0f, -0.5f });
					BranchNode* childB2b2 = createBranch(sceneGraph, childB2b, { 0.0f, 6.5f, 0.0f });
					BranchNode* childB2b3 = createBranch(sceneGraph, childB2b, { -3.0f, 6.0f, 0.5f });
				}
				BranchNode* childB2c = createBranch(sceneGraph, childB2, { -2.5f, 6.0f, -2.0f });
			}

			BranchNode* childB3 = createBranch(sceneGraph, childB, { -3.0f, 4.5f, 1.5f });
			{
				BranchNode* childB3a = createBranch(sceneGraph, childB3, { -1.5f, 3.0f, -0.5f });
				BranchNode* childB3b = createBranch(sceneGraph, childB3, { -2.0f, 1.5f, 1.0f });
				BranchNode* childB3c = createBranch(sceneGraph, childB3, { -1.0f, -0.5f, 0.2f });
			}
		}

		BranchNode* childC = createBranch(sceneGraph, child, { -5.0f, 5.0f, -2.0f });
		{
			BranchNode* childC1 = createBranch(sceneGraph, childC, { -0.2f, 3.5f, -1.0f });
			BranchNode* childC2 = createBranch(sceneGraph, childC, { -2.5f, 3.0f, -1.5f });
			{
				BranchNode* childC2a = createBranch(sceneGraph, childC2, { -1.0f, 3.0f, -0.5f });
				BranchNode* childC2b = createBranch(sceneGraph, childC2, { -2.0f, 2.0f, 0.0f });
				BranchNode* childC2c = createBranch(sceneGraph, childC2, { -1.0f, 0.1f, 0.5f });
			}
			BranchNode* childC3 = createBranch(sceneGraph, childC, { -3.0f, 0.5f, 1.5f });
		}
	}
	/**/
}

void TreeGrowth::start() {
	// Loading the cylinder 
	cylinder = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	cylinder->calculateTangents();
	//cylinder->transform(Mat4::translation({0,0,-20}));

	// Loading the shaders and creating the shader program
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderNM.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderNM.glsl");
	sp = new ShaderProgram(vs, fs);
	getSceneGraph()->getRoot()->setShaderProgram(sp);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = sp->getUniformBlockIndex("SharedMatrices");
	sp->bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	// Loading textures
	/**/
	woodTexture = new Texture2D("../Engine/textures/barkTexture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/barkNormalMap.jpg");

	setupTree(getSceneGraph());

	GLint textureID = sp->getUniformLocation("diffuseMap");
	GLint normalMapID = sp->getUniformLocation("normalMap");

	sp->use();
	sp->setUniform(textureID, 0);
	sp->setUniform(normalMapID, 1);


	/*/
	woodTexture = new Texture2D("../Engine/textures/tileable_wood_planks_texture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/tileable_wood_planks_texture_NORMAL.jpg");
	/**/

	// Adding a spherical camera controller
	float cameraMovementSpeed = 10.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 0.0f, 20.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 50.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 100.0f);
	Mat4 perspectiveProj = perspective(float(M_PI) / 2.0f, float(getWindowWidth() / getWindowHeight()), 0.001f, 100.0f);

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, getWindow());

	//cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -5.0f);
	getCamera()->addCameraController(cameraController);
	getCamera()->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

	//node->setModel(Mat4::scaling({ 0.0f, 5.0f, 0.0f }));

	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.1f;

	Vec4 lightPosition(1.5f, 5.0f, 5.0f, 1.0f);

	float specularStrength = 1.0f;
	unsigned int shininess = 4;

	GLint lightColorL = sp->getUniformLocation("lightColor");
	GLint ambientStrenghtL = sp->getUniformLocation("ambientStrength");
	GLint lightPositionL = sp->getUniformLocation("lightPosition");
	GLint specularStrengthL = sp->getUniformLocation("specularStrength");
	GLint shininessL = sp->getUniformLocation("shininess");
	GLint objectColorL = sp->getUniformLocation("color");
	normalMappingL = sp->getUniformLocation("normalMapping");
	cameraPosL = sp->getUniformLocation("viewPos");

	sp->use();
	sp->setUniform(lightColorL, lightColor);
	sp->setUniform(ambientStrenghtL, ambientStrength);
	sp->setUniform(lightPositionL, lightPosition * 2);
	sp->setUniform(specularStrengthL, specularStrength);
	sp->setUniform(shininessL, shininess);
	sp->setUniform(normalMappingL, normalMapping);
	sp->setUniform(objectColorL, { 1.0f, 0.5f, 0.31f });
	sp->stopUsing();
};

bool adapted = false;

void TreeGrowth::update() {

	module->updateModule((float)getElapsedTime());

	sp->use();
	sp->setUniform(cameraPosL, cameraController->position);

	static bool isReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_M) == GLFW_PRESS && isReleased) {
		isReleased = false;
		normalMapping = (normalMapping == GL_TRUE) ? GL_FALSE : GL_TRUE;
		sp->setUniform(normalMappingL, normalMapping);
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_M) == GLFW_RELEASE) {
		isReleased = true;
	}

	sp->stopUsing();

	//if (module->physiologicalAge > 20 && !adapted) {
		//module->adapt();
	//}
}

void TreeGrowth::end() {
	delete cameraController;
	delete sp;
	delete cylinder;
	delete woodTexture;
}

;
