#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <iostream>
#include "GL/glew.h"

/*
* Usage example:
*
* Texture2D tex = Texture2D::loadFromFile("image.png");
* 
* tex.bind();
* 
* // Draw stuff with texture
* 
* text.unBind();
* 
* delete tex;
*/
class Texture2D {

private:
	GLuint id = 0;
	int width = 0, height = 0, nrChannels = 0;

public:

	Texture2D(const std::string& textureFilePath);

	void bind();

	void unBind();
};

#endif
