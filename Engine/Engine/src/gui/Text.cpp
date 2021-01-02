#include "Text.h"
#include "../utils/ColorRGBA.h"
#include <iostream>
#include "TextRenderer.h"



Text::Text(const std::string& content, const Vec2& position) : Text(content, ARIAL, position, 15, ColorRGBA::BLACK) {}

Text::Text(const std::string& content, const std::string& font, const Vec2& position, float size, const Vec4& color): content(content), font(font), color(color)
{
	if (size < 0.0f || size > 100.0f) {
		std::cout << "Text GUI WARNING [ Text smaller than 0 or larger than 100 so it has been clamped ]." << std::endl;
	}

	// TODO Clamp
	this->size = size;
	this->position = position;
}

void Text::draw() {
	TextRenderer::getInstance()->renderText(content, font, position, size, color);
}

/////////////// SETTERS //////////////////
void Text::setFont(const std::string& font) {
	this->font = font;
}
void Text::setColor(const Vec4& color) {
	this->color = color;
}
void Text::setPosition(const Vec2& position) {
	// TODO Clamp
	this->position = position;
}
void Text::setSize(float size) {
	// TODO Clamp
	this->size = size;
}
void Text::setContent(const std::string& content) {
	this->content = content;
}

/////////////// GETTERS //////////////////
std::string Text::getFont() const {
	return font;
}
Vec4 Text::getColor() const {
	return color;
}
Vec2 Text::getPosition() const {
	return position;
}
float Text::getSize() const {
	return size;
}
std::string Text::getContent() const {
	return content;
}