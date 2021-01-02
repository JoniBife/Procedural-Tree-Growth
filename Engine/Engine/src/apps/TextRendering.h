#ifndef TEXT_RENDERING_H
#define TEXT_RENDERING_H

#include "../Engine.h"

class TextRendering : public Engine {

public:
	TextRendering() {}

	void start() override;

	void update() override;

	void end() override;
};

#endif
