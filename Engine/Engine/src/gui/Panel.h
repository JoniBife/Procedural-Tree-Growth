#ifndef PANEL_H
#define PANEL_H

#include "GUIComponent.h"
#include "../math/Vec2.h"
#include "../textures/Texture2D.h"

class Panel : public GUIComponent {

public:
	float width, height;
	Vec2 position;
	Texture2D* texture;
	Mesh* panelQuad;

public:
	Panel(const Vec2& position, float width, float height);
	~Panel();

	void setWidth(float width);
	void setHeight(float height);
	void setPosition(const Vec2& position);

	float getWidth() const;
	float getHeight() const;
	Vec2 getPosition() const;

	void draw() override;
};

#endif
