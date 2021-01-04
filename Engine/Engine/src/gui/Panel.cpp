#include "UIRenderer.h"
#include "Panel.h"


Panel::Panel(const Vec2& position, float width, float height) : position(position), width(width), height(height) {
	texture = new Texture2D("../Engine/textures/panels/defaultTexture.png");

	panelQuad = Mesh::rectangle(1.0f, 1.0f);
	panelQuad->init();
}
Panel::~Panel() {
	delete texture;
}

void Panel::setWidth(float width) {
	this->width = width;
}
void Panel::setHeight(float height) {
	this->height = height;
}
void Panel::setPosition(const Vec2& position) {
	this->position = position;
}

float Panel::getWidth() const { return width; }
float Panel::getHeight() const { return height; }
Vec2 Panel::getPosition() const { return position; }

void Panel::draw() {
	UIRenderer::getInstance()->renderUI(panelQuad, texture, position, { width, height });
}