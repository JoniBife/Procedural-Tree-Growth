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

public:
	GLuint id = 0;
private:
	int width = 0, height = 0, nrChannels = 0; // In RGB there are 3 channels while in RGBA there are 4 for example

public:

	Texture2D(const std::string& textureFilePath);

	void bind(unsigned int unitNumber);

	void unBind(unsigned int unitNumber);
};

#endif
