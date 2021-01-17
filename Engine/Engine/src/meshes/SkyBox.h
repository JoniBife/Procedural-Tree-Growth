#ifndef SKYBOX_H
#define SKYBOX_H

#include "IDrawable.h"
#include "Mesh.h"
#include "../textures/CubeMap.h"
#include "../shaders/ShaderProgram.h"

class SkyBox : IDrawable {

private:
	Mesh* cube;
	CubeMap* cubemap;
	ShaderProgram* skyboxShader;

public:
	SkyBox();
	~SkyBox();
	void init() override;
	void bind() override;
	void unBind() override;
	void draw() override;
};


#endif
