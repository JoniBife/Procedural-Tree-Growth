#include "TextRenderer.h"
#include "../utils/OpenGLUtils.h"
#include "../math/Vec4.h"
#include "FontLoader.h"
#include "../view/Transformations.h"
#include "../Configurations.h"

#define VERTICES 0

TextRenderer::TextRenderer() {

	//Creating the buffers
	GL_CALL(glGenVertexArrays(1, &vaoId));
	GL_CALL(glGenBuffers(1, &vboId));
	{
		GL_CALL(glBindVertexArray(vaoId));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));

		// Dynamic draw because every time we draw a text, we will be sending data to this buffer, maybe consider stream draw
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec4) * 4, NULL, GL_DYNAMIC_DRAW));
		GL_CALL(glEnableVertexAttribArray(VERTICES));
		GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
	}
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindVertexArray(0));

	// Loading the text shaders
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/textVertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/textFragmentShader.glsl");
	textShader = new ShaderProgram(vs, fs);
	textShader->use();
	textShader->getUniformLocation("text");
	textShader->getUniformLocation("textColor");
	textShader->getUniformLocation("projection");
	textShader->stopUsing();

	// Creating the font loader
	fontLoader = new FontLoader();

	// Creating a special orthographic projection matrix that does not have the near and far planes
	projection = ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT);
}

TextRenderer::~TextRenderer() {
	// Bind the the vao so that we can disable the vertex attrib array
	GL_CALL(glBindVertexArray(vaoId));
	GL_CALL(glDisableVertexAttribArray(VERTICES));
	GL_CALL(glDeleteBuffers(1, &vboId));

	delete textShader;
	delete fontLoader;
}


TextRenderer* TextRenderer::getInstance() {
	if (instance)
		return instance;
	
	instance = new TextRenderer();
	return instance;
}

void TextRenderer::destroyInstance() {
	if (instance)
		delete instance;
}

void TextRenderer::renderText(const std::string& text, const std::string& font, const Vec2& position, float scale, const Vec4& color) {

	auto fontMap = fontLoader->loadFont(font);

	textShader->use();
	textShader->setUniform(textColorLocation, color);
	textShader->setUniform(projectionLocation, projection);

	GL_CALL(glBindVertexArray(vaoId));

	// We need to enable blending for the characters to be drawn transparent
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// The position of the current character being drawn onto the screen
	Vec2 currPosition = position;

	// We iterate over each of the characters and draw each character in a different quad
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character character = fontMap->at(*c);

		float xpos = currPosition.x + character.bearing.x * scale;
		float ypos = currPosition.y - (character.height - character.bearing.y) * scale;

		float w = character.width * scale;
		float h = character.height * scale;

		// Different characters have quads of different sizes so we ajust the vertices for each character
		Vec4 vertices[6] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Render glyph texture over quad
		character.texture->bind(GL_TEXTURE0);

		// Update content of VBO memory
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// Drawing quad
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		currPosition.x += (character.advanceX >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}

	GL_CALL(glDisable(GL_BLEND));

	GL_CALL(glBindVertexArray(0));

	textShader->stopUsing();
}