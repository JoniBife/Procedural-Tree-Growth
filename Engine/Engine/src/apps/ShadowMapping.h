#include "../Engine.h"

class ShadowMapping : public Engine {

public:
	ShadowMapping() {}

	void start() override;

	void update() override;

	void end() override;
};