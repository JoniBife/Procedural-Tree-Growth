#include "../Engine.h"

class Blending : public Engine {

public:
	Blending() {}

	void start() override;

	void update() override;

	void end() override;
};
