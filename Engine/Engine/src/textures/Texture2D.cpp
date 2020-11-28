#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <assert.h>
#include "../utils/OpenGLUtils.h"

Texture2D::Texture2D(const std::string& textureFilePath) {
	// Loading image data from textureFilePath
	unsigned char* data = stbi_load(textureFilePath.c_str(), &width, &height, &nrChannels, 0);

	// If there is an error while reading the data from the image file this will be NULL
	assert(data != NULL);

	// Generating the texture
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	// texture wrapping/filtering options
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	//free image data, no longer necessary
	stbi_image_free(data);
}

void Texture2D::bind() {
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture2D::unBind() {
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}